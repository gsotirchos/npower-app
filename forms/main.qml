import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.VirtualKeyboard 2.4
import npower.backend.sensors 1.0

ApplicationWindow {
    id: window
    visible: true
    width: 800
    height: 480
    title: qsTr("Title")

    Sensors {
        id: sensors
    }

    header: ToolBar {
        id: toolBar
        contentHeight: toolButton.implicitHeight

        ToolButton {
            id: toolButton
            text: stackView.depth > 1 ? "\u25C0" : "\u2630"
            font.pixelSize: Qt.application.font.pixelSize * 1.6
            onClicked: {
                if (stackView.depth > 1) {
                    stackView.pop()
                } else {
                    drawer.open()
                }
            }
        }

        Label {
            text: stackView.currentItem.title
            anchors.centerIn: parent
        }

        BatteryIndicatorForm {
            id: batteryIndicator
            width: 100
            height: parent.height * 0.5
            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    Drawer {
        id: drawer
        width: window.width * 0.33
        height: window.height

        Column {
            anchors.fill: parent

            ItemDelegate {
                text: qsTr("About")
                width: parent.width
                onClicked: {
                    stackView.push("AboutForm.ui.qml")
                    drawer.close()
                }
            }
            ItemDelegate {
                text: "hall value: " + sensors.value + "\n" +
                      "voltage: " + sensors.voltage + "\n" +
                      "current: " + sensors.current + "\n" +
                      "power: " + sensors.power + "\n"
            }
        }
    }

    StackView {
        id: stackView
        initialItem: "ChallengeSelectionForm.ui.qml"
        anchors.fill: parent

//        pushEnter: Transition {
//            PropertyAnimation {
//                property: "opacity"
//                from: 0
//                to: 1
//                duration: 200
//            }
//        }
//        pushExit: Transition {
//            PropertyAnimation {
//                property: "opacity"
//                from: 0
//                to: 1
//                duration: 200
//            }
//        }
//        popEnter: Transition {
//            PropertyAnimation {
//                property: "opacity"
//                from: 0
//                to: 1
//                duration: 200
//            }
//        }
//        popExit: Transition {
//            PropertyAnimation {
//                property: "opacity"
//                from: 0
//                to: 1
//                duration: 200
//            }
//        }
    }
}
