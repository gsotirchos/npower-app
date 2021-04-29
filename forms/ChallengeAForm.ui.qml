import QtQuick 2.12
import QtQuick.Controls 2.5

Page {
    title: "Challenge A"

    Label {
        id: time
        anchors.bottom: stats.top
        anchors.bottomMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: Qt.application.font.pixelSize * 2
        padding: 10
        text: controller.remainingTime + " sec"
    }

    Label {
        id: stats
        anchors.centerIn: parent
        text: "Power: " + controller.power + " W\n " + "Steps: "
              + controller.steps + "\n" + "Speed: " + controller.speed.toFixed(
                  1) + " rpm"
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

