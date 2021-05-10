import QtQuick 2.0
import QtQuick.Controls 2.5


Button {
    id: selectButton
    property string challengeForm

    font.pixelSize: Qt.application.font.pixelSize * 1.5
    text: qsTr("Save")
    enabled: controller.canSaveScore

    onClicked: {
        controller.saveScore(nameField.text)
        stackView.pop()
        controller.closeLeaderboard()
        stackView.pop("ChallengeSelectionForm.ui.qml")
    }
}
/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
