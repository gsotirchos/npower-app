import QtQuick 2.12
import QtQuick.Controls 2.5

Page {
    property alias challengeName: name
    property alias challengeDescription: description
    property alias selectButton: selectButton

    Label {
        id: name
        anchors.bottom: description.top
        anchors.bottomMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: Qt.application.font.pixelSize * 2
        padding: 10
    }

    Label {
        id: description
        anchors.centerIn: parent
    }

    SelectButton {
        id: selectButton
        anchors.top: description.bottom
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/

