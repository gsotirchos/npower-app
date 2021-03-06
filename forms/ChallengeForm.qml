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

    Grid {
        id: info
        anchors.verticalCenter: parent.verticalCenter
        columns: 2
        spacing: 5
        anchors.left: parent.left
        anchors.leftMargin: parent.width * 0.5 - leftWidth * 1.3
        property double fontScale: 1.5
        property int leftWidth: 100
        verticalItemAlignment: Grid.AlignVCenter

        Label {
            width: parent.leftWidth
            horizontalAlignment: Text.AlignRight
            font.pixelSize: Qt.application.font.pixelSize * parent.fontScale
            text: stackView.measurements.names[stackView.infoOrder[0]] + ": "
            color: "#707070"
        }
        Label {
            font.pixelSize: Qt.application.font.pixelSize * parent.fontScale
            text: stackView.measurements.contents[stackView.infoOrder[0]]
        }
        Label {
            width: parent.leftWidth
            horizontalAlignment: Text.AlignRight
            font.pixelSize: Qt.application.font.pixelSize * parent.fontScale
            text: stackView.measurements.names[stackView.infoOrder[1]] + ": "
            color: "#707070"
        }
        Label {
            font.pixelSize: Qt.application.font.pixelSize * parent.fontScale
            text: stackView.measurements.contents[stackView.infoOrder[1]]
        }
        Label {
            width: parent.leftWidth
            horizontalAlignment: Text.AlignRight
            font.pixelSize: Qt.application.font.pixelSize * parent.fontScale
            text: stackView.measurements.names[stackView.infoOrder[2]] + ": "
            color: "#707070"
        }
        Label {
            font.pixelSize: Qt.application.font.pixelSize * parent.fontScale
            text: stackView.measurements.contents[stackView.infoOrder[2]]
        }
        Label {
            width: parent.leftWidth
            horizontalAlignment: Text.AlignRight
            font.pixelSize: Qt.application.font.pixelSize * parent.fontScale
            text: stackView.measurements.names[stackView.infoOrder[3]] + ": "
            color: "#707070"
        }
        Label {
            font.pixelSize: Qt.application.font.pixelSize * parent.fontScale
            text: stackView.measurements.contents[stackView.infoOrder[3]]
        }
    }

//    Label {
//        id: info
//        anchors.centerIn: parent
//        font.pixelSize: Qt.application.font.pixelSize * 1.5
//        text: stackView.measurements.names[stackView.infoOrder[0]]
//              + ": " + stackView.measurements.contents[stackView.infoOrder[0]]
//              + "\n" + stackView.measurements.names[stackView.infoOrder[1]]
//              + ": " + stackView.measurements.contents[stackView.infoOrder[1]]
//              + "\n" + stackView.measurements.names[stackView.infoOrder[2]]
//              + ": " + stackView.measurements.contents[stackView.infoOrder[2]]
//              + "\n" + stackView.measurements.names[stackView.infoOrder[3]]
//              + ": " + stackView.measurements.contents[stackView.infoOrder[3]]
//    }

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
