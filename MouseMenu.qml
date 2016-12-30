import QtQuick 2.6
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2


MouseArea {
	FileDialog {
		id: fileDialog
		title: "Please choose a file"
		selectExisting: true
		selectFolder:  false
		selectMultiple: false
		onAccepted: {
			console.log("You chose: " + fileDialog.fileUrls)
			runner.load(fileDialog.fileUrl)
		}
	}

	Menu {
		id: menu
		MenuItem {
			text: "Save"
			onTriggered: {
				runner.save();
			}
		}
		MenuItem {
			text: "Load"
			onTriggered: {
				fileDialog.visible = true
			}
		}
		MenuItem {
			text: "Quit"
			onTriggered: {
				app.close();
			}
		}
	}

	anchors.fill: parent
	acceptedButtons: Qt.RightButton
	onClicked: {
		menu.popup();
	}
}
