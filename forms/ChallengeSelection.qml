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
            id: challengeA
            challengeName: qsTr("Max Power")
            challengeType: "max power"
            challengeDescription: qsTr("You are on Challenge A.")
        }

        ChallengeInfoForm {
            id: challengeB
            challengeName: qsTr("Max Speed")
            challengeType: "max speed";
            challengeDescription: qsTr("You are on Challenge B.")
        }

        ChallengeInfoForm {
            id: challengeC
            challengeName: qsTr("Lightning Fast 1W")
            challengeType: "lightning fast"
            challengeDescription: qsTr("You are on Challenge C.")
        }
    }

    footer: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex

        TabButton {
            text: challengeA.challengeName
        }

        TabButton {
            text: challengeB.challengeName
        }

        TabButton {
            text: challengeC.challengeName
        }
    }
}



