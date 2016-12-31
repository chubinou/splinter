import re
import socket
import os
import time

class TCPClient:
	def __init__(self, host, port):
		self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.sock.connect((host, port))
		self.data = b""

	def send(self,  *args):
		print("send command {}".format(" ".join(args)))
		msg = b" ".join(args) + b"\r\n"
		self.sock.send(msg)


class PrintClient:
	def send(self, *args):
		print(" ".join(args))

class SplinterBaseParser:
	def __init__(self, client):
		self._running = False
		self._client = client
		self._state = "NotRunning"

	def startup(self):
		return False

	def update(self):
		return False

	def start(self):
		return False

	def reset(self):
		return False

	def gameTime(self):
		return False

	def split(self):
		return False

	def isPaused(self):
		return False

	def run(self):
		self.startup()
		while True:
			ret = self.update()
			if not ret:
				continue
			if self._state in ["Running", "Paused"]:
				if self.reset():
					self._client.send("reset")
				elif self.split():
					self._client.send("split")
			elif self._state == "NotRunning":
				if self.start():
					self._client.send("starttimer")
					self._state = "Running"
			time.sleep(0.005)

class TalosParser(SplinterBaseParser):
	def __init__(self, client, settings = {}):
		SplinterBaseParser.__init__(self, client)
		self.settings = {
			"Game Log path": os.path.expanduser("~/.steam/steam/steamapps/common/The Talos Principle/Log/Talos.log"),
			"Split on return to Nexus": False,
			"Split on tetromino collection": True,
			"Split on star collection": False,
			"Split on item unlocks": True,
			"Split on tetromino world doors": True,
			"Split on tetromino star doors": False,
			"Split on tetromino tower doors": True,
			"Split on star collection in the Nexus": False,
			"Split on completing C*": False,
			"(DLC) Split on return to Hub": True,
			"(DLC) Split on puzzle doors": False,
		}
		self.settings.update(settings)
		self.reader = open(self.settings["Game Log path"], "w+")

	def startup(self):
		self.currentWorld = ""
		self.gameStarting = False
		self.introCutscene = False
		self.lastLines = 0
		self.doubleSplit = False
		self.totalLoads = 0#TimeSpan.FromSeconds(0)
		self.adminEnding = False

	def update(self):
		self.line = self.reader.readline()

		if self.line != None:
			self.line = self.line[15:] # Removes the date and log level from the line
		else:
			self.line = ""
		self.line = self.line.strip()

		# Intro cutscene (booting up sequence) is excluded from IGT
		if self.introCutscene and self.line.startswith("Player profile saved"):
			#self.totalLoads = timer.CurrentTime.RealTime
			self.introCutscene = False

			regex = re.compile("^Started simulation on '.*?' in ([\d\.]*) seconds")
			match = regex.match(self.line)
			# Not removing the initial load (A1) because the run hasn't started yet
			#if match and timer.CurrentTime.RealTime != TimeSpan.FromSeconds(0):
			#	load = TimeSpan.FromSeconds(Single.Parse(match.Groups[1].Value))
			#	self.totalLoads += load
		return True

	def start(self):
		# Only start for A1, since restore backup / continue should mostly be on other worlds.
		if self.line.startswith("Started simulation on 'Content/Talos/Levels/Cloud_1_01.wld'"):
			self.gameStarting = True

		# Ditto for Gehenna with the intro world.
		if self.line.startswith("Started simulation on 'Content/Talos/Levels/DLC_01_Intro.wld'"):
			self.gameStarting = True

		if self.gameStarting and "sound channels reinitialized." in self.line:
			self.gameStarting = False
			self.introCutscene = True
			return True



	def reset(self):
		if self.line == "Saving talos progress upon game stop.":
			self.currentWorld = ""
			self.gameStarting = False
			self.introCutscene = False
			self.lastLines = 0
			self.doubleSplit = False
			self.totalLoads = 0#TimeSpan.FromSeconds(0)
			self.adminEnding = False
			return True



	def gameTime(self):
		if self.introCutscene:
			return 0 #TimeSpan.FromSeconds(0)

		return timer.CurrentTime.RealTime - self.totalLoads


	def split(self):
		# Map changes
		if self.line.startswith("Changing over to"):
			mapName = self.line[17:]
			print("map name: '{}'".format(mapName))
			# Ensure 'restart checkpoint' doesn't trigger map change
			if mapName != self.currentWorld:
				self.currentWorld = mapName
				if mapName == "Content/Talos/Levels/Nexus.wld":
					print("goto nexus")
					return self.settings["Split on return to Nexus"]

				if mapName == "Content/Talos/Levels/DLC_01_Hub.wld":
					print("goto hub")
					return self.settings["(DLC) Split on return to Hub"]

				if mapName == "Content/Talos/Levels/Cloud_3_08.wld":
					print("goto cloud")
					self.cStar = 0


		# Sigil and star collection
		if self.line.startswith("Picked:"):
			if self.doubleSplit:
				return False

			self.doubleSplit = True
			if self.currentWorld == "Content/Talos/Levels/Cloud_3_08.wld":
				self.cStar += 1 # Sigils collected while in C Star
				if self.cStar == 3:
					print("C*")
					return self.settings["Split on completing C*"]


			if self.line[8:10] == "**":
				if self.settings["Split on star collection"]:
					print("star")
					return True
				else:
					if self.currentWorld == "Content/Talos/Levels/Nexus.wld":
						print("nexus start")
						return self.settings["Split on star collection in the Nexus"]


			else:
				print("tetromino")
				return self.settings["Split on tetromino collection"]

		else:
			self.doubleSplit = False # DLC Double-split prevention


		# Arranger puzzles
		if self.line.startswith("Puzzle \"") and "\" solved" in self.line:
			puzzle = self.line[8:]
			if puzzle.startswith("Mechanic"):
				print("Mechanic")
				return self.settings["Split on item unlocks"]

			if puzzle.startswith("Door"):
				print("door")
				return self.settings["Split on tetromino world doors"]

			if puzzle.startswith("SecretDoor"):
				print("secret door")
				return self.settings["Split on tetromino star doors"]

			if puzzle.startswith("Nexus"):
				print("nexus")
				return self.settings["Split on tetromino tower doors"]

			if puzzle.startswith("DLC_01_Secret"):
				print("DLC_01_Secret")
				return self.settings["(DLC) Split on puzzle doors"]

			if puzzle.startswith("DLC_01_Hub"):
				print("DLC_01_Hub")
				self.adminEnding = True # Admin puzzle door solved, so the Admin is saved.
				return self.settings["(DLC) Split on puzzle doors"]

		if self.currentWorld == "Content/Talos/Levels/Islands_03.wld":  # Base game Messenger Ending
			return self.line.startswith("USER:") # Line differs in languages.

		if self.currentWorld == "Content/Talos/Levels/Nexus.wld":  # Base game Transcendence and Eternalize Ending
			return (self.line == "USER: /transcend" or self.line == "USER: /eternalize")

		if self.currentWorld == "Content/Talos/Levels/DLC_01_Hub.wld":  # Any DLC ending
			if self.line == "Save Talos Progress: entered terminal":
				self.lastLines = 0

			if self.line.startswith("USER:"):
				self.lastLines += 1
				if self.adminEnding:  # If admin is saved, it takes 5 lines to end the game
					return (self.lastLines == 5)
				else:  # In all other endings, game ends on the 4th dialogue after entering the terminal
					return (self.lastLines == 4)

def main():
	#client = PrintClient()
	client = TCPClient("127.0.0.1", 16834)
	parser = TalosParser(client)
	parser.run()

if __name__ == "__main__":
	main()
