import QtQuick 2.6
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.0

MouseArea {
	Menu {
		id: menu
		MenuItem {
			text: "Save"
			onTriggered: {
				runner.save();
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
