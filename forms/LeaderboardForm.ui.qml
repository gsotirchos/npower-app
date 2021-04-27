import QtQuick 2.12
import QtQuick.Controls 2.5

Page {
    id: page
    title: "Leaderboard"

    Label {
        id: enterNameLabel
        anchors.bottom: nameFeld.top
        anchors.bottomMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: Qt.application.font.pixelSize * 1.5
        text: "Enter your name:"
        anchors.horizontalCenterOffset: 0
    }

    TextField {
        id: nameFeld
        width: 200
        height: 35
        anchors.top: parent.top
        anchors.topMargin: 75
        anchors.horizontalCenter: parent.horizontalCenter
        text: qsTr("")
        placeholderText: "Name"
        renderType: Text.QtRendering
        horizontalAlignment: Text.AlignHCenter
    }

    SaveButton {
        y: 148
        text: "Save"
        anchors.verticalCenter: nameFeld.verticalCenter
        anchors.left: nameFeld.right
        anchors.leftMargin: 10
    }

    LeaderboardTable {
        width: parent.width * 0.90
        anchors.horizontalCenterOffset: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        anchors.top: nameFeld.bottom
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

        dataModel: [["1", "Player17", 34.6], ["2", "Player3", 23.9], ["3", "Player21", 18.2], ["4", "Player7", 17.7], ["5", "Player11", 17.3], ["6", "Player22", 16.2], ["7", "Player5", 16.4], ["8", "Player9", 15.6], ["9", "Player13", 14.7], ["10", "Player12", 14.5], ["11", "Player23", 12.3], ["12", "Player4", 12.0], ["13", "Player2", 11.4], ["14", "Player14", 9.7], ["15", "Player28", 7.5]]
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:4;anchors_height:480}
}
##^##*/

