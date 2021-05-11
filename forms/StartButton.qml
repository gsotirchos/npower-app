import QtQuick 2.0
import QtQuick.Controls 2.5


Button {
    id: startButton
    font.pixelSize: Qt.application.font.pixelSize * 2.5
    text: qsTr("START")

    contentItem: Text {
        text: startButton.text
        font: startButton.font
        opacity: enabled ? 1.0 : 0.5
        color: startButton.down ? "lightgray" : "white"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        radius: width * 0.2
        color: {
            switch (text) {
            case "3":
            case "2":
            case "1":
                "orange";
                break;
            case "GO":
                "green";
                break;
            case "High Scores":
                "gold";
                break;
            default:
                startButton.down ? "blue" : "lightblue";
            }
        }
    }

    onClicked: {
        if (text == "START") {
            countdown = 3
            text = countdown
            enabled = false
        } else {
            controller.openLeaderboard()
            if (controller.canSaveScore) {
                controller.saveScore()
            }
            stackView.replace("LeaderboardForm.qml")
        }
    }


    Connections {
        target: controller
        onChallengeFinished: {
            console.log("[LOG] FINISHED")
            width = 270
            text = "High Scores"
            enabled = true
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
