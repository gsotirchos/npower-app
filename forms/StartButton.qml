import QtQuick 2.0
import QtQuick.Controls 2.5


Button {
    id: startButton
    property string time
    text: qsTr("Start")

    onClicked: {
        text = (text == "Start") ? qsTr("Finish") : qsTr("Start")
        if (text == "Start") { stackView.replace("LeaderboardForm.ui.qml") }
    }
}
