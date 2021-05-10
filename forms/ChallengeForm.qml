import QtQuick 2.12
import QtQuick.Controls 2.5

Page {
    title: stackView.selectedChallenge

    property int countdown: -1

    Label {
        id: target
        anchors.bottom: info.top
        anchors.bottomMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: Qt.application.font.pixelSize * 3
        padding: 10
        text: stackView.measurements.contents[stackView.mainIndex]
    }

    Label {
        id: info
        anchors.centerIn: parent
        font.pixelSize: Qt.application.font.pixelSize * 1.5
        text: stackView.measurements.names[stackView.infoOrder[0]]
              + ": " + stackView.measurements.contents[stackView.infoOrder[0]]
              + "\n" + stackView.measurements.names[stackView.infoOrder[1]]
              + ": " + stackView.measurements.contents[stackView.infoOrder[1]]
              + "\n" + stackView.measurements.names[stackView.infoOrder[2]]
              + ": " + stackView.measurements.contents[stackView.infoOrder[2]]
              + "\n" + stackView.measurements.names[stackView.infoOrder[3]]
              + ": " + stackView.measurements.contents[stackView.infoOrder[3]]
    }

    StartButton {
        id: startButton
        width: 170
        height: 75
        anchors.top: info.bottom
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Timer {
        id: countdownTimer
        interval: 1000
        running: countdown >= 0
        onTriggered: {
            if (--countdown > 0) {
                startButton.text = countdown
            } else {
                startButton.text = qsTr("GO")
                countdown = -1
                controller.startChallenge()
            }
        }
    }
}




/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
