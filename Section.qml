import QtQuick 2.0
import org.chub 1.0

Component {
	id: splitDelegate
	Rectangle {
		id: splitEntry
		property int itemIndex: index
		width: parent.width
		height: 20
		color: "transparent"
		Text {
			id: titleTxt
			text: title
			elide: Text.ElideRight
			color: "#FFFFFF"
			font.bold: splitstate === SplitModel.SEGMENT_RUNNING
			anchors.left: splitEntry.left
			anchors.right: currentTxt.left
			anchors.leftMargin: 3
		}
		Text {
			id: currentTxt
			text: currentDiffBest
			visible: (splitstate === SplitModel.SEGMENT_DONE_BETTER || splitstate === SplitModel.SEGMENT_DONE_WORSE)
			color: {
				if (splitstate === SplitModel.SEGMENT_DONE_BETTER)
					return "green"
				else
					return "red"
			}
			anchors.rightMargin: 5
			anchors.right: bestCumulTxt.left
		}
		Text {
			id: bestCumulTxt
			text: bestCumul
			color: "#AAAAAA"
			anchors.right: splitEntry.right
			anchors.rightMargin: 3
		}
		function makeVisible() {

		}

		states: [
			State {
				name: "running"
				when: splitstate === SplitModel.SEGMENT_RUNNING
				StateChangeScript {
					name: "make visible"
					script: {
						splitView.positionViewAtIndex(itemIndex, ListView.Contain)
					}
				}
			}
		]
	}
}
