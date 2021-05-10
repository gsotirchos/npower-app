import QtQuick 2.12
import QtQuick.Controls 2.5

Page {
    id: leaderboardForm
    title: stackView.selectedChallenge + ": High Scores"

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: forceActiveFocus()

        Label {
            id: insertNameLabel
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.horizontalCenterOffset: 0
            anchors.horizontalCenter: parent.horizontalCenter

            font.pixelSize: Qt.application.font.pixelSize * 1.5
            text: controller.canSaveScore ? qsTr("New High Score! Insert your name:") : qsTr("No new High Score... Try Again!")
        }

        Label {
            id: finalScore
            anchors.verticalCenter: nameField.verticalCenter
            anchors.right: nameField.left
            anchors.rightMargin: 30

            font.pixelSize: Qt.application.font.pixelSize * 2
            text: stackView.measurements.contents[stackView.targetIndex]
            color: controller.canSaveScore ? "green" : "red"
        }

        TextField {
            id: nameField
            width: 270
            height: 50
            anchors.top: insertNameLabel.bottom
            anchors.topMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter

            enabled: controller.canSaveScore
            font.pixelSize: Qt.application.font.pixelSize * 1.5
            text: qsTr("")
            placeholderText: "Enter your name"

            onActiveFocusChanged: {
               if(activeFocus) {
                    Qt.inputMethod.show();
                   //Qt.inputMethod.update(Qt.ImQueryInput)
               } else
                    Qt.inputMethod.hide();
            }

            onAccepted: {
                Qt.inputMethod.commit();
                Qt.inputMethod.hide();
            }
        }

        SaveButton {
            height: nameField.height
            width: height*2
            anchors.verticalCenter: nameField.verticalCenter
            anchors.left: nameField.right
            anchors.leftMargin: 10
        }

        LeaderboardTable {
            width: parent.width * 0.90
            anchors.horizontalCenterOffset: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 20
            anchors.top: nameField.bottom
            anchors.topMargin: 20
            anchors.horizontalCenter: parent.horizontalCenter

            dataModel: controller.scores
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/

