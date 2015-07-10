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

    property var source: ""

    signal loaded
    signal ended

    WebView {
            id: web1
            width: parent.width
            height: parent.height
            url: "http://yandex.ru/"
            onLoadingChanged: {
                if(contentHeight > height && loadRequest.status === WebView.LoadSucceededStatus) {
                    tmr.stop()
                    tmr.start()
                    web1.contentY = 0
                }
                else if(contentHeight <= height && loadRequest.status === WebView.LoadSucceededStatus) {
                    loaded()
                    ended()
                }
            }
        }


    Item {
        Timer {
            id: tmr
            interval: 100
            running: false
            repeat: true
            onTriggered: {
                web1.flick(0,-67)
                if(web1.contentY >= web1.contentHeight - height) {
                    tmr.stop()
                    ended()
                    tmr1.start()
                }
            }
        }

        Timer {
            id: tmr1
            interval: 30000
            running: false
            repeat:false
            triggeredOnStart: false
            onTriggered: {
                webWidget.visible = false
                web1.contentY = 0
            }
        }
    }

    function setSource(_path)
    {
        web1.url = _path;
        source = _path;
    }
}
