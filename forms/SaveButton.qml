import QtQuick 2.0
import QtQuick.Controls 2.5


Button {
    id: selectButton
    property string challengeForm
    text: qsTr("Save")

    onClicked: {
        stackView.pop("ChallengeSelectionForm.ui.qml")
    }
}
/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
