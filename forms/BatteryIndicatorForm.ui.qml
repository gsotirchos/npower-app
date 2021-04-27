import QtQuick 2.4
import QtQuick.Controls 2.5

Item {
    id: root
    property int percentage: 75

    Label {
        id: batteryIndicatorText
        anchors.right: root.right
        anchors.rightMargin: 0
        anchors.verticalCenter: root.verticalCenter
        anchors.verticalCenterOffset: -batteryIndicatorText.height * 0.1
        height: root.height * 0.85
        width: height * 2
        font.pixelSize: height
        text: root.percentage + "%"
    }

    Item {
        id: batteryIndicatorIcon
        anchors.left: root.left
        anchors.leftMargin: 0
        anchors.right: batteryIndicatorText.left
        anchors.rightMargin: batteryIndicatorText.height * 0.5
        anchors.verticalCenter: root.verticalCenter
        height: root.height

        property color fillColor: "green"
        property color emptyColor: "lightgrey"
        property int radius: 4

        Rectangle {
            id: batteryPole
            anchors.left: batteryBody.right
            anchors.leftMargin: -width * 0.5
            anchors.verticalCenter: batteryIndicatorIcon.verticalCenter
            height: batteryIndicatorIcon.height * 0.5
            width: height * 0.5
            radius: width * 0.2
            color: batteryIndicatorIcon.emptyColor
        }

        ProgressBar {
            id: batteryBody
            value: root.percentage
            to: 100
            width: batteryIndicatorIcon.width - batteryPole.width * 0.5
            height: batteryIndicatorIcon.height

            background: Rectangle {
                anchors.fill: batteryBody
                color: batteryIndicatorIcon.emptyColor
                radius: batteryIndicatorIcon.radius
            }

            contentItem: Item {
                anchors.fill: batteryBody

                Rectangle {
                    property int padding: 2
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: padding
                    width: batteryBody.visualPosition * batteryBody.width - 2 * padding
                    height: batteryBody.height - 2 * padding
                    color: batteryIndicatorIcon.fillColor
                    radius: batteryBody.background.radius * 0.75
                }
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/

