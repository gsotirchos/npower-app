import QtQuick 2.0
import QtQuick.Controls 2.5


Button {
    id: selectButton

    contentItem: Item {
        id: buttonContents
        anchors.centerIn: parent

        Label {
            text: qsTr("High Scores")
            anchors.centerIn: parent
            font.pixelSize: Qt.application.font.pixelSize * 2
        }
    }

    onClicked: {
        stackView.selectedChallenge = challengeName
        controller.challengeType = challengeType

        controller.openLeaderboard()
        stackView.push("HighScores.qml")
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
