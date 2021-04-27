import QtQuick 2.12
import QtQuick.Controls 2.5

Page {
    title: "Challenge C"

    Label {
        id: speed
        anchors.bottom: stats.top
        anchors.bottomMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: Qt.application.font.pixelSize * 2
        padding: 10
        text: "0 rpm"
    }

    Label {
        id: stats
        anchors.centerIn: parent
        text: "Remaining Time: 30 sec\n                Power: 0 W\n                 Steps: 0"
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

