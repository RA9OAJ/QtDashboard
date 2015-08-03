import QtQuick 2.0

Item {
    id: imageWidget
    width: parent.width - parent.border.width * 2 - 1
    height: parent.height  - parent.border.width * 2 - 1
    anchors.centerIn: parent

    readonly property string widget_type: "IMAGE"
    property bool _paused: true
    property bool muted: true
    property real volume: 0.0
    property alias source: img.source
    property alias timeout: tmr.interval

    signal loaded
    signal ended
    signal __error(int _error, string _errorString)
    signal started

    Image {
        id: img
        fillMode: Image.PreserveAspectFit
        anchors.fill: parent
        smooth: true

        onStatusChanged: {
            console.log(img.status)
            if(img.status == Image.Ready)
                loaded()
        }
    }

    Timer {
        id: tmr
        repeat: false
        interval: 30000
        running: false
        triggeredOnStart: false

        onTriggered: {ended()}
    }

    function start() {
        imageWidget.visible = true
        tmr.start()
        started()
    }

    function pause() {
        tmr.stop()
    }

    function stop() {
        tmr.stop()
        ended()
    }

    function setSource(src) {
        source = src
    }
}
