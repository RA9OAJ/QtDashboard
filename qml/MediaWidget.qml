import QtQuick 2.0
import QtMultimedia 5.0

Rectangle {
    id: mediabWidget
    width: parent.width - parent.border.width * 2 - 1
    height: parent.height  - parent.border.width * 2 - 1
    anchors.centerIn: parent

    property var source: Null
    property var _paused: true

    signal loaded
    signal ended

    MediaPlayer {
        id: player

        onPaused: {
            _paused = true
        }

        onStopped: {
            _paused = true

            if(state === MediaPlayer.EndOfMedia)
                ended()
        }

        onPlaying: {
            _paused = false
        }


    }

    VideoOutput {
            width: parent.width
            height: parent.height

            anchors.fill: parent

            source: player
    }

    MouseArea {
            id: playArea
            anchors.fill: parent
            onClicked: {
                if(_paused === true && player.state === MediaPlayer.Buffered)
                    player.play()
                else player.pause()
            }
    }

    function setSource(_path) {
        source = _path
        player.source = _path
    }

}
