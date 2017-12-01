import QtQuick 2.0
import QtGraphicalEffects 1.0

Rectangle {
    id: dtWidget

    readonly property string widget_type: "WIDGET"
    readonly property string widget_name: "DateTime"
    property int src_id: -1
    property bool _paused: true
    property bool muted: true
    property real volume: 0.0
    property string source: ""
    property alias timeout: tmr.interval

    signal loaded
    signal ended
    signal __error(int _error, string _errorString)
    signal started
    signal paused

    width: parent.width - parent.border.width * 2 - 1
    height: parent.height  - parent.border.width * 2 - 1
    anchors.fill: parent

    radius: 20
    border.color: "#AAAAAA"
    border.width: 2

    anchors.topMargin: 5
    color: "#99FFFFFF"
    clip: true

    Timer {
        id: tmr
        running: false
        repeat: false

        onTriggered: {
            ended()
        }
    }

    function setSource(_path) {
        loaded()
    }

    function start() {
        started()
    }

    function stop() {
        ended()
    }

    function pause() {
        ended()
    }
}
