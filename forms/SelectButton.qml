import QtQuick 2.0
import QtQuick.Controls 2.5


Button {
    id: selectButton
    text: qsTr("Select")

    onClicked: {
        stackView.selectedChallenge = challengeName
        stackView.targetIndex = targetIndex
        stackView.infoOrder = infoOrder
        controller.remainingTime = remainingTime
        controller.time = 0
        controller.steps = 0
        controller.speed = 0
        controller.power = 0
        stackView.push("ChallengeForm.ui.qml")
    }
}
/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
