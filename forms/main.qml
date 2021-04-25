import QtQuick 2.12
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtQuick.VirtualKeyboard 2.4
import com.company.someclass 1.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    MyClass {
        id: someclass
    }

    Row {
        anchors.top: parent.top
        anchors.topMargin: 8
        anchors.horizontalCenterOffset: 0
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 10

        Button {
            id: btnSetVar
            text: "Set var"
            onClicked: {
                someclass.setToOne
            }
        }
    }
}
