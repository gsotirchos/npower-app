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
            challengeName: qsTr("Challenge A")
            challengeDescription: qsTr("You are on Challenge A.")
            remainingTime: 30
            targetIndex: 0
            infoOrder: [2, 3, 4]
        }

        ChallengeInfoForm {
            id: challengeB
            challengeName: qsTr("Challenge B")
            challengeDescription: qsTr("You are on Challenge B.")
            remainingTime: 10
            targetIndex: 0
            infoOrder: [4, 3, 2]
        }

        ChallengeInfoForm {
            id: challengeC
            challengeName: qsTr("Challenge C")
            challengeDescription: qsTr("You are on Challenge C.")
            remainingTime: 90
            targetIndex: 1
            infoOrder: [2, 3, 4]
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



