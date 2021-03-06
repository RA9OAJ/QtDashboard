import QtQuick 2.1
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
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

    property var widget_manager: null
    property bool flag: false

    Image {
        width: parent.width
        height: parent.height
        source: "./images/back.jpg"

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
            //border.color: "#000000"
            //border.width: 1
            //color: "#009900FF"
            color: "transparent"

            Component.onCompleted: {
                var component = Qt.createComponent("qrc:/WidgetManager.qml");
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

            radius: 20
            border.color: "#000000"
            border.width: 1

            anchors.leftMargin: 10
            anchors.left: main_frame.right
            color: "#CCFFFFFF"

            Column {
                spacing: 5
                x: 1
                y: 7
                width: parent.width - 2
                height: parent.height - y

                Text{text: "Руководитель УФК по Новосибирской области"; wrapMode: Text.WordWrap; horizontalAlignment: Text.Center; width: parent.width; font.pixelSize: 16}

                Image {
                    id: ruk
                    x: (parent.width - width) / 2
                    width: parent.width * 0.47

                    fillMode: Image.PreserveAspectFit
                    source: "images/ruk.jpg"
                }

                Text{text: "<b><i>Гаврилюк Валерий Геннадьевич</i></b>"; wrapMode: Text.WordWrap; horizontalAlignment: Text.Center; width: parent.width; font.pixelSize: 16;}
                Text{text: "<i>тел. +7 (383) 269-51-05</i>"; wrapMode: Text.WordWrap; horizontalAlignment: Text.Center; width: parent.width; font.pixelSize: 14;}
                Text{text: "<br/><b>График личного приема граждан руководителем и заместителями руководителя</b>"; wrapMode: Text.WordWrap; horizontalAlignment: Text.Center; width: parent.width - 10; font.pixelSize: 13; x: 5}
                Text{text: "<ul><li>Вт.-Чт. с 14-00 до 16-00</li></ul>
                            Запись на прием по телефонам: <ul><li><i>+7 (383) 269-60-91</i></li><li><i>+7 (383) 218-08-07</i></li><li><i>+7 (383) 269-61-05</i></li></ul>"; wrapMode: Text.WordWrap; width: parent.width; font.pixelSize: 13; x: 6}

            }

        }

        Rectangle {
            id: right_frame2

            x: parent.width / 2 - width / 2
            y: 10

            width: parent.width * 21 / 100
            height: parent.height * 48 / 100

            radius: 20
            border.color: "#000000"
            border.width: 1

            anchors.leftMargin: 10
            anchors.topMargin: 10
            anchors.left: main_frame.right
            anchors.top: right_frame1.bottom
            color: "#CCFFFFFF"

            Calendar {
                x: 1
                y: 1
                anchors.centerIn: parent
                width: parent.width - 20
                height: parent.height - 20

                style: CalendarStyle {
                    //gridVisible: false
                    gridColor: "white"
                    background: Rectangle {
                        color: "transparent"
                        border.color: "transparent"
                        border.width: 0
                    }

                    dayDelegate: Rectangle{
                        color: styleData.selected ? "#5599FF" : "transparent"
                        radius: 5

                        Label {
                            text: styleData.date.getDate()
                            anchors.centerIn: parent
                            color: thisMonth() ? styleData.selected ? "white" : "black" : "#666666"
                            font.pixelSize: styleData.selected ? 29 : 22
                        }

                        function thisMonth() {
                            var cur_month = new Date().getMonth()
                            var month = new Date(styleData.date).getMonth()
                            return cur_month == month
                        }
                    }
                }
            }
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
