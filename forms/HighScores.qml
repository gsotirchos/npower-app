import QtQuick 2.12
import QtQuick.Controls 2.5

Page {
    title: qsTr(stackView.selectedChallenge + ": High Scores")

    LeaderboardTable {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.fill: parent
        anchors.margins: 25

        dataModel: controller.scores
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/

