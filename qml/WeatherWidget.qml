import QtQuick 2.0
import QtGraphicalEffects 1.0

Rectangle {
    id: weatherWidget

    width: parent.width - parent.border.width * 2 - 1
    height: parent.height  - parent.border.width * 2 - 1
    anchors.fill: parent

    radius: 20
    border.color: "#AAAAAA"
    border.width: 2

    anchors.topMargin: 5
    color: "#99FFFFFF"
    clip: true

    Image {
        id: bg
        width: parent.width
        height: parent.height
        fillMode: Image.TileHorizontally
        anchors.fill: parent
        visible: false

        source: "qrc:///rain.jpg"
    }

    OpacityMask {
        anchors.fill: parent
        source: bg
        maskSource: weatherWidget
    }
}
