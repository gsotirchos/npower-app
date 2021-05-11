import QtQuick 2.12
import QtQuick.Controls 2.5

Page {
    id: challengeSelection
    title: qsTr("Select a challenge")

    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        ChallengeInfoForm {
            id: maxPower
            challengeName: qsTr("Max Power")
            challengeType: "max power"
            challengeDescription: qsTr("Generate as much power as you\ncan in half a minute!")
        }

        ChallengeInfoForm {
            id: maxSpeed
            challengeName: qsTr("Max Speed")
            challengeType: "max speed";
            challengeDescription: qsTr("Hit maximum speed\nin only 15 seconds!")
        }

        ChallengeInfoForm {
            id: lightningFast
            challengeName: qsTr("Lightning Fast 10mWh")
            challengeType: "lightning fast"
            challengeDescription: qsTr("See how fast you can generate\n10mWh of energy!")
        }
    }

    footer: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex

        TabButton {
            font.bold: true
            text: maxPower.challengeName
        }

        TabButton {
            font.bold: true
            text: maxSpeed.challengeName
        }

        TabButton {
            font.bold: true
            text: lightningFast.challengeName
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
