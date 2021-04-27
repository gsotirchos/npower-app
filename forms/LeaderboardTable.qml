import QtQuick 2.0
import QtQuick.Controls 2.5

Item { // size controlled by width
    id: root

// public
    property variant headerModel: [ // widths must add to 1
//    {
//        "text"
//        : "Rank",
//        "width": 0.20
//    }, {
//        "text": "Name",
//        "width": 0.60
//    }, {
//        "text": "Score",
//        "width": 0.20
//    }
    ]

    property variant dataModel: [
//         ["1", "Player17", 34.6],
//         ["2", "Player3", 23.9],
//         ["3", "Player21", 18.2],
//         ["4", "Player7", 17.7],
//         ["5", "Player11", 17.3],
//         ["6", "Player22", 16.2],
//         ["7", "Player5", 16.4],
//         ["8", "Player9", 15.6],
//         ["9", "Player13", 14.7],
//         ["10", "Player12", 13.2],
//         ["11", "Player23", 12.3],
//         ["12", "Player4", 12.0],
//         ["13", "Player2", 11.4],
//         ["14", "Player14", 9.7],
//         ["15", "Player28", 7.5]
    ]

// private
    width: parent.width
    height: parent.height

    Rectangle {
        id: header
        width: root.width
        height: 30
        color: "orange"
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
