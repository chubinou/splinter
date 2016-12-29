import QtQuick 2.6
import QtQuick.Window 2.2
import org.chub 1.0

OSD {
	width: 300;
	height: 400
	x: 0;
	y: 0; //  Screen.height - height


	Rectangle {
		id: background
		color: "#111111"
		anchors.fill: parent
		opacity: 0.8

		Rectangle {
			id: titleHeader
			width: parent.width
			anchors.top: parent.top
			height: 60
			color: "transparent"

			Column {
				anchors.fill: parent
				Text {
					id: title
					color: "#ffffff"
					text: game
					font.bold: true
					horizontalAlignment: Text.AlignHCenter
					font.pixelSize: 20
					width: parent.width
				}
				Text {
					id: cat
					color: "#ffffff"
					text: category
					horizontalAlignment: Text.AlignHCenter
					font.pixelSize: 15
					width: parent.width
				}
			}
		}

		ListView {
			id: splitView
			width: parent.width
			anchors {
				top: titleHeader.bottom
				bottom: timeFooter.top
				margins: 10
			}
			model: splits
			move: Transition {
					NumberAnimation { properties: "contentX,contentY"; duration: 1000 }
				}


			delegate: Section {
			}
		}

		Rectangle {
			id: timeFooter
			anchors.bottom: parent.bottom
			width: parent.width
			height: 40
			color: "transparent"

			Timer {
				interval: 10
				running: true
				repeat: true

				onTriggered: {
					chrono.text = runner.elapsed()
				}
			}

			Text {
				id: chrono
				anchors.fill: parent
				anchors.rightMargin: 5
				color: "#ffffff"
				text: "0.00"
				font.bold: true
				horizontalAlignment: Text.AlignRight
				verticalAlignment: Text.AlignBottom
				font.pixelSize: 32
			}
		}
	}
	Component.onCompleted: {
		visible = true
	}
}
