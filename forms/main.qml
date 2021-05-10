import QtQuick 2.12
import QtQuick.VirtualKeyboard 2.4
import QtQuick.Controls 2.5
import npower.backend.controller 1.0

ApplicationWindow {
    id: window
    visible: true
    visibility: "FullScreen"
    width: 800
    height: 480

    title: qsTr("npower-app")

    Controller {
        id: controller
    }

    Component.onCompleted: {
        controller.startBatteryMonitor()
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
                    controller.stopChallenge()
                    controller.closeLeaderboard()
                } else {
                    drawer.open()
                }
            }
        }

        Label {
            font.pixelSize: Qt.application.font.pixelSize * 1.5
            text: stackView.currentItem.title
            font.bold: true
            color: "white"
            anchors.centerIn: parent
        }

        BatteryIndicatorForm {
            id: batteryIndicator
            width: 100
            height: parent.height * 0.5
            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.verticalCenter: parent.verticalCenter
            percentage: controller.chargePercentage
        }
    }

    Drawer {
        id: drawer
        width: window.width * 0.33
        height: window.height

        Column {
            anchors.fill: parent

            ItemDelegate {
                width: parent.width
                height: width*0.2
                font.pixelSize: Qt.application.font.pixelSize * 1.5
                text: qsTr("About")
                onClicked: {
                    stackView.push("AboutForm.qml")
                    drawer.close()
                }
            }
        }
    }

    StackView {
        id: stackView
        initialItem: "ChallengeSelection.qml"
        anchors.fill: parent

        property string selectedChallenge: ""
        property int mainIndex: 0
        property int targetIndex: 0
        property var infoOrder: []
        property var measurements: {
            "names": [
                "Time",
                "Time",
                "Energy",
                "Power",
                "Steps",
                "Speed",
                "Max speed"
            ],
            "contents": [
                controller.remainingTime + " s",
                controller.time + " s",
                controller.energy.toFixed(2) + " mWh",
                controller.power.toFixed(2) + " W",
                controller.steps,
                controller.speed.toFixed(2) + " rpm",
                controller.maxSpeed.toFixed(2) + " rpm"
            ]
        }

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

    InputPanel {
       id: inputPanel
       y: Qt.inputMethod.visible ? parent.height - inputPanel.height : parent.height
       anchors.left: parent.left
       anchors.right: parent.right
    }
}
