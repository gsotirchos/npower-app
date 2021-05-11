import QtQuick 2.0
import QtQuick.Controls 2.5


Button {
    id: highScoresButton

    contentItem: Item {
        id: buttonContents
        anchors.centerIn: parent

        Label {
            text: qsTr("High Scores")
            color: highScoresButton.down ? "lightgray" : "white"
            anchors.centerIn: parent
            font.pixelSize: Qt.application.font.pixelSize * 2
        }
    }

    background: Rectangle {
        radius: height * 0.2
        color: highScoresButton.down ? "#E5BF00" : "#FFD400"
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
