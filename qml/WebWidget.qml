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

    ScrollView {
        id: sv
        width: parent.width
        height: parent.height
        verticalScrollBarPolicy: Qt.ScrollBarAlwaysOff
        horizontalScrollBarPolicy: Qt.ScrollBarAlwaysOff

        WebView {
            id: web1
            width: parent.width
            height: parent.height
            url: "http://yandex.ru/"
            onLoadingChanged: {
                if(contentHeight > sv.height && loadRequest.status === WebView.LoadSucceededStatus) {
                    tmr.stop()
                    tmr.start()
                    sv.flickableItem.contentY = 0
                }
                else if(contentHeight <= sv.height && loadRequest.status === WebView.LoadSucceededStatus) {
                    loaded()
                    ended()
                }
            }
        }

    }

    Item {
        Timer {
            id: tmr
            interval: 70
            running: false
            repeat: true
            onTriggered: {
                sv.flickableItem.contentY = sv.flickableItem.contentY + 1
                if(sv.flickableItem.contentY >= web1.contentHeight - sv.height) {
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
                sv.flickableItem.contentY = 0
            }
        }
    }

    function setSource(_path)
    {
        web1.url = _path;
        source = _path;
    }
}
