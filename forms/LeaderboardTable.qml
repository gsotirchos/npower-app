import QtQuick 2.0
import QtQuick.Controls 2.5

Item { // size controlled by width
    id: root

//    Component.onCompleted: {
//        for (var i = 0; i < controller.scores.length; ++i) {
//            console.log(
//                controller.scores[i][0] + " "
//                + controller.scores[i][1] + " "
//                + controller.scores[i][2]
//            );
//        }
//    }

// public
    property variant headerModel: [{
            "text": "Rank",
            "width": 0.20
        }, {
            "text": "Name",
            "width": 0.60
        }, {
            "text": "Score",
            "width": 0.20
    }]

    property variant dataModel: []

// private
    width: parent.width
    height: parent.height

    Rectangle {
        id: header
        width: root.width
        height: 30
        color: "gold"
        radius: 0.02 * root.width

        Rectangle { // half height to cover bottom rounded corners
            width: parent.width
            height: 0.5 * parent.height
            color: parent.color
            anchors.bottom: parent.bottom
        }

        ListView { // header
            anchors.fill: parent
            orientation: ListView.Horizontal
            interactive: false

            model: headerModel

            delegate: Item { // cell
                width: modelData.width * header.width;  height: header.height

                Text {
                    x: 0.03 * header.width
                    y: 0.03 * header.height
                    font.bold: true
                    text: modelData.text
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 0.8 * header.height
                    color: "white"
                }
            }
        }
    }

    ListView { // data
        anchors{fill: parent;  topMargin: header.height}
        interactive: contentHeight > height
        clip: true

        model: dataModel

        delegate: Item { // row
            id: row
            width: root.width
            height: header.height

            property int row: index // outer index
            property variant rowData: modelData // much faster than listView.model[row]

            Row {
                anchors.fill: parent

                Repeater { // index is column
                    model: rowData // headerModel.length

                    delegate: Item { // cell
                        width: headerModel[index].width * root.width;  height: header.height

                        Text {
                            x: 0.03 * row.width
                            y: 0.03 * row.hight
                            text: modelData
                            anchors.verticalCenter: parent.verticalCenter
                            font.pixelSize: 0.8 * row.height
                        }
                    }
                }
            }


        }

        ScrollBar{}
    }
}
