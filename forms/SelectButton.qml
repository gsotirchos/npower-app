import QtQuick 2.0
import QtQuick.Controls 2.5


Button {
    id: selectButton
    text: qsTr("Select")

    onClicked: {
        stackView.selectedChallenge = challengeName
        controller.challengeType = challengeType

        if (challengeType == "max power") {
            controller.remainingTime = 20
            controller.powerTarget = 0
            stackView.targetIndex = 0
            stackView.infoOrder = [2, 4, 3]
        } else if (challengeType == "max speed") {
            controller.remainingTime = 15
            controller.powerTarget = 0
            stackView.targetIndex = 0
            stackView.infoOrder = [5, 2, 3]
        } else if (challengeType == "lightning fast") {
            controller.remainingTime = 120
            controller.powerTarget = 10
            stackView.targetIndex = 1
            stackView.infoOrder = [2, 4, 3]
        }

        controller.time = 0
        controller.steps = 0
        controller.speed = 0
        controller.maxSpeed = 0
        controller.power = 0
        stackView.push("ChallengeForm.ui.qml")
    }
}
/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
