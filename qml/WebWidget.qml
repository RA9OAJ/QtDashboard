import QtQuick 2.1
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import QtWebKit 3.0

//----------------------WebWidget--------------------------------
Item {
    id: webWidget
    width: parent.width - parent.border.width * 2 - 1
    height: parent.height  - parent.border.width * 2 - 1
    anchors.centerIn: parent

    readonly property string widget_type: "WEB"
    property bool _paused: true
    property bool muted: true
    property real volume: 0.0
    property alias source: web1.url
    property alias timeout: tmr2.interval

    signal loaded
    signal ended
    signal __error(int _error, string _errorString)
    signal started

    WebView {
            id: web1
            width: parent.width
            height: parent.height
            url: "http://yandex.ru/"
            onLoadingChanged: {
                if(loadRequest.status === WebView.LoadSucceededStatus && loadProgress === 100) {
                    loaded()
                    contentY = 0
                }
            }
        }

    MouseArea {
        id: mouse_area
        width: parent.width
        height: parent.height

        onPressed: {
            pause()
            visible = false
        }
    }

    Timer {
        id: tmr
        interval: 70
        running: false
        repeat: true
        onTriggered: {
            web1.flick(0,-67)
            if(web1.contentY >= web1.contentHeight - height) {
                tmr.stop()
                tmr2.start()
            }
        }
    }

    Timer {
        id: tmr1
        interval: 10000
        running: false
        repeat: false
        triggeredOnStart: false

        onTriggered: {
            if(web1.contentY >= web1.contentHeight - height) {
                tmr2.start()
            }
            else {
                parent.start()
                mouse_area.visible = true
            }
        }
    }

    Timer {
        id: tmr2
        interval: 30000
        running: false
        repeat: false
        triggeredOnStart: false

        onTriggered: ended()
    }

    function setSource(_path)
    {
        web1.url = _path;
    }

    function start()
    {
        if(web1.loadProgress === 100) {
            if(web1.contentHeight > height)
            {
                tmr.stop()
                tmr.start()
                tmr1.stop()
            }
            else {
                tmr2.start()
            }
        }

    }

    function pause()
    {
        tmr.stop()
        tmr2.stop()
        tmr1.start()
    }

    function stop()
    {
        pause()
        ended()
    }
}
