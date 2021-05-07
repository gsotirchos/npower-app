import QtQuick 2.0
import QtQuick.Controls 2.5


Button {
    id: startButton
    property string time
    text: qsTr("Start")

    onClicked: {
        if (text == "Finish") {
            controller.stopChallenge()
            controller.openLeaderboard();
            stackView.replace("LeaderboardForm.ui.qml")
        } else {
            text = qsTr("Finish")
            controller.startChallenge()
        }
    }
}
