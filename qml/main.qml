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
    visibility: "FullScreen"
    color: "#CCCCCC"

    property var web: Null

    Image {
        width: parent.width
        height: parent.height
        source: "qrc:///kazna.jpg"

        Rectangle {
            x: 10
            y: 10
            id: main_frame
            width: parent.width * 77 / 100
            height: parent.height * 98 / 100
            anchors.margins: 10
            border.color: "#000000"
            border.width: 1
            //color: "#009900FF"

            Component.onCompleted: {
                var component = Qt.createComponent("WebWidget.qml");
                main.title = component.errorString()
                if (component.status === Component.Ready) {
                    var childRec = component.createObject(main_frame);
                    web = childRec;
                    web.setSource("http://roskazna.ru/");
                    web.ended.connect(setHello);
                    //web.__error.connect(eRR);
                }
            }
        }

        Rectangle {
            id: right_frame1

            x: parent.width / 2 - width / 2
            y: 10

            width: parent.width * 21 / 100
            height: parent.height * 49 / 100

            //radius: 20
            border.color: "#000000"
            border.width: 1

            anchors.leftMargin: 10
            anchors.left: main_frame.right
            //color: "#99FFFFFF"

            /*Component.onCompleted: {
                var component = Qt.createComponent("WeatherWidget.qml");
                main.title = component.errorString()
                if (component.status === Component.Ready) {
                    var childRec = component.createObject(right_frame1);
                    //childRec.setSource("file:///home/user/1.mp4");
                }
            }*/
        }

        Rectangle {
            id: right_frame2

            x: parent.width / 2 - width / 2
            y: 10

            width: parent.width * 21 / 100
            height: parent.height * 48 / 100

            //radius: 20
            border.color: "#000000"
            border.width: 1

            anchors.leftMargin: 10
            anchors.topMargin: 10
            anchors.left: main_frame.right
            anchors.top: right_frame1.bottom
            //color: "#99FFFFFF"

            /*Component.onCompleted: {
                var component = Qt.createComponent("WeatherWidget.qml");
                main.title = component.errorString()
                if (component.status === Component.Ready) {
                    var childRec = component.createObject(right_frame2);
                    //childRec.setSource("file:///home/user/1.mp4");
                }
            }*/
        }
    }

    function eRR(_err, str) {
        console.log(_err, str)
    }

    function setHello() {
        main.title = "Hello!!!"
    }

}
