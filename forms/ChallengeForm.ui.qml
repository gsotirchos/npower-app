import QtQuick 2.12
import QtQuick.Controls 2.5

Page {
    title: stackView.selectedChallenge

    Label {
        id: target
        anchors.bottom: info.top
        anchors.bottomMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: Qt.application.font.pixelSize * 2
        padding: 10
        text: stackView.measurements.contents[stackView.targetIndex]
    }

    Label {
        id: info
        anchors.centerIn: parent
        text: stackView.measurements.names[stackView.infoOrder[0]] + ": " + stackView.measurements.contents[stackView.infoOrder[0]] + "\n" +
              stackView.measurements.names[stackView.infoOrder[1]] + ": " + stackView.measurements.contents[stackView.infoOrder[1]] + "\n" +
              stackView.measurements.names[stackView.infoOrder[2]] + ": " + stackView.measurements.contents[stackView.infoOrder[2]]
    }

    StartButton {
        anchors.top: info.bottom
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/

