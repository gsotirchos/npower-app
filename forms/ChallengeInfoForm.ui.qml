import QtQuick 2.12
import QtQuick.Controls 2.5

Page {
    id: page
    property string challengeName: ""
    property string challengeDescription: ""
    property string challengeType: ""
    property int mainIndex: 0
    property var infoOrder: []

    Item {
        width: 430
        height: 270
        anchors.centerIn: parent

        SelectButton {
            id: selectButton
            anchors.top: parent.top
            anchors.topMargin: 0
            width: parent.width
            anchors.bottom: highScoresButton.top
            anchors.bottomMargin: 15
        }

        HighScoresButton {
            id: highScoresButton
            width: 200
            height: 50
            anchors.bottom: parent.bottom
            anchors.topMargin: 0
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/

