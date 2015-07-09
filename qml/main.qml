import QtQuick 2.1
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import QtWebKit 3.0

ApplicationWindow {
    id: main
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")
    //visibility: "FullScreen"
    color: "#CCCCCC"

    property var web: Null

    Image {
        width: parent.width
        height: parent.height
        source: "qrc:///kazna.jpg"

        Rectangle {
            id: top_frame

            x: parent.width / 2 - width / 2
            y: 6

            width: parent.width * 99 / 100
            height: parent.height * 25 / 100

            radius: 20
            border.color: "#AAAAAA"
            border.width: 2

            anchors.topMargin: 5
            color: "#33FFFFFF"

        }

        Rectangle {
            id: main_frame
            width: parent.width * 99 / 100
            height: parent.height * 73 / 100
            anchors.top: top_frame.bottom
            anchors.horizontalCenter: top_frame.horizontalCenter
            anchors.margins: 10
            border.color: "#AAAAAA"
            border.width: 2
            color: "#009900FF"

            Component.onCompleted: {
                var component = Qt.createComponent("MediaWidget.qml");
                main.title = component.errorString()
                if (component.status === Component.Ready) {
                    var childRec = component.createObject(main_frame);
                    web = childRec;
                    //web.setSource("file:///home/rav/1.mp4");
                    web.ended.connect(setHello);
                }
            }
        }
    }

    function setHello() {
        main.title = "Hello!!!"
    }

}
