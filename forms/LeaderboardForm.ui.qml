import QtQuick 2.12
import QtQuick.Controls 2.5

Page {
    id: leaderboardForm
    title: stackView.selectedChallenge + " - High Scores"

    MouseArea {
        anchors.fill: parent
        onClicked: forceActiveFocus()

        Label {
            id: enterNameLabel
            anchors.bottom: nameField.top
            anchors.bottomMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: Qt.application.font.pixelSize * 1.5
            text: "Enter your name:"
            anchors.horizontalCenterOffset: 0
        }

        TextField {
            id: nameField
            width: 200
            height: 35
            anchors.top: parent.top
            anchors.topMargin: 75
            anchors.horizontalCenter: parent.horizontalCenter

            enabled: controller.canSaveScore
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
            y: 148
            text: "Save"
            anchors.verticalCenter: nameField.verticalCenter
            anchors.left: nameField.right
            anchors.leftMargin: 10
        }

        LeaderboardTable {
            width: parent.width * 0.90
            anchors.horizontalCenterOffset: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            anchors.top: nameField.bottom
            anchors.topMargin: 20
            anchors.horizontalCenter: parent.horizontalCenter

            headerModel: [{
                    "text"// widths must add to 1
                    : "Rank",
                    "width": 0.20
                }, {
                    "text": "Name",
                    "width": 0.60
                }, {
                    "text": "Score",
                    "width": 0.20
                }]

            dataModel: controller.scores
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/

