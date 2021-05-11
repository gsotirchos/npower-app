import QtQuick 2.0
import QtQuick.Controls 2.5


Button {
    id: selectButton

    contentItem: Item {
        id: buttonContents
        anchors.centerIn: parent

        Label {
            id: name
            text: challengeName
            anchors.verticalCenterOffset: -height
            anchors.centerIn: parent
            font.pixelSize: Qt.application.font.pixelSize * 2
        }
        Label {
            id: description
            text: challengeDescription
            anchors.horizontalCenter: name.horizontalCenter
            anchors.top: name.bottom
            anchors.topMargin: 20
        }
    }

    onClicked: {
        stackView.selectedChallenge = challengeName
        controller.challengeType = challengeType

        if (challengeType == "max power") {
            controller.remainingTime = 30
            controller.energyTarget = 0
            stackView.mainIndex = 0
            stackView.targetIndex = 2
            stackView.infoOrder = [2, 3, 4, 5]
        } else if (challengeType == "max speed") {
            controller.remainingTime = 15
            controller.energyTarget = 0
            stackView.mainIndex = 0
            stackView.targetIndex = 6
            stackView.infoOrder = [6, 4, 2, 3]
        } else if (challengeType == "lightning fast") {
            controller.remainingTime = 120
            controller.energyTarget = 10
            stackView.mainIndex = 1
            stackView.targetIndex = 1
            stackView.infoOrder = [2, 3, 4, 5]
        }

        controller.time = 0
        controller.steps = 0
        controller.speed = 0
        controller.maxSpeed = 0
        controller.energy = 0
        controller.power = 0
        stackView.push("ChallengeForm.qml")
    }
}
/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
