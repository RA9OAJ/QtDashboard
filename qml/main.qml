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

    property var widget_manager: Null
    property bool flag: false

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
            color: "transparent"
        }

        Rectangle {
            x: (-50 - (main_frame.x + main_frame.width))
            y: main_frame.y
            id: frame
            width: main_frame.width
            height: main_frame.height
            anchors.margins: 10
            border.color: "#000000"
            border.width: 1
            //color: "#009900FF"

            Component.onCompleted: {
                var component = Qt.createComponent("WidgetManager.qml");
                main.title = component.errorString()
                if (component.status === Component.Ready) {
                    var childRec = component.createObject(frame);
                    widget_manager = childRec;
                    widget_manager.ended.connect(setHello);
                    widget_manager.loaded.connect(showMain);
                }
            }

            onXChanged: {
                if(widget_manager != null) {
                    if(x == main_frame.x)
                        widget_manager.start()
                    else if(x <= -50 - (main_frame.x + main_frame.width) && flag) {
                        widget_manager.stop()
                        widget_manager.next()
                    }
                    else if(x <= -50 - (main_frame.x + main_frame.width) && !flag)
                        widget_manager.run()
                }
            }

            Behavior on x {
                SpringAnimation {
                    spring: 2
                    damping: 0.35

                }
            }

            Behavior on y {
                SpringAnimation {
                    spring: 2
                    damping: 0.35
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
        }
    }


    function eRR(_err, str) {
        console.log(_err, str)
    }

    function setHello() {
        flag = true
        frame.x = -50 - (frame.x + frame.width)
    }

    function showMain() {
        frame.x = main_frame.x
        flag = false
    }
}
