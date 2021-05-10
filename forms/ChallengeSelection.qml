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
            challengeDescription: qsTr("You are on Challenge A.")
        }

        ChallengeInfoForm {
            id: maxSpeed
            challengeName: qsTr("Max Speed")
            challengeType: "max speed";
            challengeDescription: qsTr("You are on Challenge B.")
        }

        ChallengeInfoForm {
            id: lightningFast
            challengeName: qsTr("Lightning Fast 10mWh")
            challengeType: "lightning fast"
            challengeDescription: qsTr("You are on Challenge C.")
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
