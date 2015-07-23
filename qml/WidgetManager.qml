import QtQuick 2.0
import SourceManager 1.0

Item {
    width: parent.width
    height: parent.height

    property var objectsOnCache: 1

    signal loaded //генерится при полной загрузке материала
    signal ended //генерится при таймауте тамера времени показа материала
    signal started //генерится в момент начала показа материала
    signal load //генерится в начале загрузки материала

    Item {
        id: internal
        property  list<QtObject> objectCache: [Item{objectName: "empty";}]
    }

    SourceManager {
        id: srcmanager
    }

    Component.onCompleted: {
        createObject()
    }

    function createObject(cache){
        cache = typeof cache != "undefined" ? cache : false

        if(srcmanager.size > 0) {
            var component = null;
            switch (srcmanager.sourceType) {
            case SourceManager.WEB:
                component = Qt.createComponent("WebWidget.qml");
                break
            case SourceManager.AUDIO:
            case SourceManager.VIDEO:
                component = Qt.createComponent("MediaWidget.qml");
                break
            default:
                break
            }

            if(component != null){
                if (component.status === Component.Ready) {
                    var childRec = component.createObject(parent);
                    var array = new Array(0)
                    if(internal.objectCache[0].objectName != "empty")
                        array.push(internal.objectCache)
                    var num = array.length
                    array[num] = childRec;
                    array[num].setSource(srcmanager.source);
                    array[num].ended.connect(ended);
                    array[num].loaded.connect(loaded);
                    array[num].started.connect(started)
                    internal.objectCache = array
                    console.log(internal.objectCache[num])
                }
            }
        }
    }

    function start() {
        if(internal.objectCache[0].objectName != "empty") {
            internal.objectCache[0].start()
        }
    }

    function pause() {
        if(internal.objectCache[0].objectName != "empty") {
            internal.objectCache[0].pause()
        }
    }

    function stop() {
        if(internal.objectCache[0].objectName != "empty") {
            internal.objectCache[0].stop()
        }
    }
}
