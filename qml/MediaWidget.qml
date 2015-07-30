import QtQuick 2.0
import QtAV 1.6

Rectangle {
    id: mediabWidget
    width: parent.width - parent.border.width * 2
    height: parent.height  - parent.border.width * 2
    anchors.centerIn: parent

    property alias source: player.source
    property bool _paused: true
    property alias muted: player.muted
    property alias volume: player.volume

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
        interval: 20
        repeat: false
        triggeredOnStart: false
        running: false

        onTriggered: {player.pause(); player.seek(0); loaded()}
    }

    Item {
        id: internal

        property bool _mute: false
        property real _volume: 1.0
    }

    function setSource(_path) {
        source = _path
        player.source = _path

        internal._mute = muted
        internal._volume = player.volume
        player.volume = 0.0
        player.muted = true
        player.play()
        tmr.start()
    }

    function start() {
        if(player.error === AVPlayer.NoError && _paused === true) {
            player.play()
            player.muted = internal._mute
            player.volume = internal._volume
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
