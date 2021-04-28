import QtQuick 2.12
import QtQuick.Controls 2.5
import npower.backend.challenge 1.0

Page {
    title: "Challenge A"

    Challenge {
        id: challenge
    }

    Label {
        id: time
        anchors.bottom: stats.top
        anchors.bottomMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: Qt.application.font.pixelSize * 2
        padding: 10
        text: "30 sec"
    }

    Label {
        id: stats
        anchors.centerIn: parent
        text: "Power: 0 W\n Steps: 0\nSpeed: 0 rpm"
    }

    StartButton {
        anchors.top: stats.bottom
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/

