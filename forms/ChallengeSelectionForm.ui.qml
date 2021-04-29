import QtQuick 2.12
import QtQuick.Controls 2.5

Page {
    title: qsTr("Select a challenge")

    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        ChallengeInfoForm {
            challengeName.text: qsTr("Challenge A")
            challengeDescription.text: qsTr("You are on Challenge A.")
            selectButton.challengeForm: "ChallengeAForm.ui.qml"
        }

        ChallengeInfoForm {
            challengeName.text: qsTr("Challenge B")
            challengeDescription.text: qsTr("You are on Challenge B.")
            selectButton.challengeForm: "ChallengeBForm.ui.qml"
        }

        ChallengeInfoForm {
            challengeName.text: qsTr("Challenge C")
            challengeDescription.text: qsTr("You are on Challenge C.")
            selectButton.challengeForm: "ChallengeCForm.ui.qml"
        }
    }

    footer: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex

        TabButton {
            text: qsTr("Challenge A")
        }
        TabButton {
            text: qsTr("Challenge B")
        }
        TabButton {
            text: qsTr("Challenge C")
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/

