import QtQuick 2.0
import QtAV 1.6

Rectangle {
    id: mediabWidget
    width: parent.width - parent.border.width * 2
    height: parent.height  - parent.border.width * 2
    anchors.centerIn: parent

    property var source: ""
    property var _paused: true
    property var _muted: false

    signal loaded
    signal ended
    signal __error(int _error, string _errorString)
    signal started
    signal paused

    AVPlayer {
        id: player

        muted: false
        autoPlay: false

        onPaused: {
            _paused = true
        }

        onStopped: {
            _paused = true

            if(status === AVPlayer.EndOfMedia)
                ended()
        }

        onPlaying: {
            _paused = false
        }

        onError: {
            __error(error, errorString)
        }
    }

    VideoOutput2 {
            width: parent.width
            height: parent.height

            anchors.fill: parent

            source: player
    }

    MouseArea {
            id: playArea
            anchors.fill: parent
            onClicked: {
                if(player.error === AVPlayer.NoError) {
                    if(_paused === true)
                        player.play()
                    else player.pause()
                }
                else player.stop()
            }
    }

    Timer {
        id: tmr
        interval: 200
        repeat: false
        triggeredOnStart: false
        running: false

        onTriggered: {player.pause(); player.seek(0); player.muted = _muted; loaded()}
    }

    function setSource(_path) {
        source = _path
        player.source = _path

        player.muted = true
        player.play()
        tmr.start()

    }

    function muted(flag) {
        _muted = flag
        player.muted = flag
    }

    function start() {
        if(player.error === AVPlayer.NoError && _paused === true) {
            player.play()
            started()
        }
    }

    function pause() {
        if(player.error === AVPlayer.NoError && _paused != true)
            player.pause()
    }

    function stop() {
        player.stop()
        ended()
    }
}
