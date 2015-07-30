import QtQuick 2.0
import SourceManager 1.0

Item {
    width: parent.width
    height: parent.height

    property int objectsOnCache: 1

    signal loaded //генерится при полной загрузке материала
    signal ended //генерится при таймауте тамера времени показа материала
    signal started //генерится в момент начала показа материала
    signal load //генерится в начале загрузки материала

    Item {
        id: internal
        property  variant objectCache: []
        property variant o2: []

        function isLoaded() {
            console.log("Next is Load!")
        }

        function isError() {
            console.log("Mext not Load :(")
        }
    }

    SourceManager {
        id: srcmanager
        onSourceListRead: {
            createObject()
        }
    }

    Component.onCompleted: {
        srcmanager.readXmlSourceList("/home/user/list.xml")
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
                    var array = new Array
                    if(internal.objectCache.length != 0)
                        array = internal.objectCache
                    var num = array.length
                    array[num] = childRec;

                    if(num == 0) {
                        array[0].volume = srcmanager.volume;
                        array[0].muted = srcmanager.mute
                        array[0].ended.connect(ended);
                        array[0].loaded.connect(loaded);
                        array[0].started.connect(started)
                        array[0].setSource(srcmanager.source);
                        srcmanager.goNext()
                    }
                    else {
                        array[num].visible = false
                        array[num].volume = srcmanager.volume;
                        array[num].muted = srcmanager.mute
                        array[num].loaded.connect(internal.isLoaded)
                        array[num].__error.connect(internal.isError)
                        array[num].setSource(srcmanager.source)
                    }

                    internal.objectCache = array
                }
            }
        }
    }

    function start() {
        if(internal.objectCache.length) {
            internal.objectCache[0].start()
            createObject()
        }
    }

    function pause() {
        if(internal.objectCache.length) {
            internal.objectCache[0].pause()
        }
    }

    function stop() {
        if(internal.objectCache.length) {
            internal.objectCache[0].stop()
        }
    }

    function next() {
        if(internal.objectCache.length) {
            var array = new Array(0)
            array = internal.objectCache
            array.shift().destroy()

            if(array.length > 0) {
                array[0].ended.connect(ended)
                array[0].loaded.connect(loaded)
                array[0].started.connect(started)
                array[0].visible = true
                srcmanager.goNext()
                loaded()
            }
            internal.objectCache = array
        }
    }
}
