import QtQuick 2.12
import QtQuick.Controls 2.5

Page {
    title: qsTr("About")

    Item {
        id: element
        anchors.centerIn: parent
        width: parent.width * 0.8
        height: parent.height * 0.8

        Image {
            id: npowerImage
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            width: 0.60 * parent.width
            fillMode: Image.PreserveAspectFit
            source: "images/NGK_N-POWER.jpg"
            mipmap: true
        }

        Label {
            id: ngkText
            anchors.top: npowerImage.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: -40

            horizontalAlignment: Text.AlignHCenter
            color: "black"
            text: qsTr("Next Generation Kids - N-POWER\n2021 FIRST LEGO League")
        }

        Label {
            anchors.top: ngkText.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 10

            horizontalAlignment: Text.AlignHCenter
            text: qsTr("App version: 1.0.0\nSotirchos George")
        }
    }

    Image {
        id: ngkIimage
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        width: 0.2 * parent.width
        fillMode: Image.PreserveAspectFit
        source: "images/NGK.png"
        mipmap: true

        MouseArea {
            anchors.fill: parent
            property int cnt: 10
            onClicked: {
                if (--cnt == 0) {
                    controller.deleteDatabases()
                    cnt = 10
                }
            }
        }
    }
}
