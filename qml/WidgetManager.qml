import QtQuick 2.0
import SourceManager 1.0

Item {
    width: parent.width
    height: parent.height

    signal loaded //генерится при полной загрузке материала
    signal ended //генерится при таймауте тамера времени показа материала
    signal started //генерится в момент начала показа материала
    signal load //генерится в начале загрузки материала

    Item {
        id: internal
        property  variant busyWidgets: []
        property variant freeWidgets: []

        function isLoaded() {
            console.log("Next is Load!")
        }

        function isError() {
            console.log("Mext not Load :(")
        }

        function getFreeWidget(widget_type) {
            if(!freeWidgets.length)
                return null

            var cur = null;
            for(var i = 0; i < freeWidgets.length; i++)
                if(freeWidgets[i].widget_type === widget_type) {
                    cur = freeWidgets.splice(i,1)[0]
                    break
                }

            return cur
        }
    }

    SourceManager {
        id: srcmanager
    }

    Component.onCompleted: {
        srcmanager.readXmlSourceList("list")
    }

    function createObject(cache){
        cache = typeof cache != "undefined" ? cache : false

        if(srcmanager.size > 0) {
            var component = null;
            var childRec = null;

            switch (srcmanager.sourceType) {
            case SourceManager.WEB:
                childRec = internal.getFreeWidget("WEB")
                if(childRec == null)
                    component = Qt.createComponent("WebWidget.qml")
                break
            case SourceManager.IMAGE:
                childRec = internal.getFreeWidget("IMAGE")
                if(childRec == null)
                    component = Qt.createComponent("ImageWidget.qml")
                break
            case SourceManager.AUDIO:
            case SourceManager.VIDEO:
                childRec = internal.getFreeWidget("WEB")
                if(childRec == null)
                    component = Qt.createComponent("MediaWidget.qml")
                break
            default:
                break
            }

            if(component != null && component.status === Component.Ready)
                childRec = component.createObject(parent)
            else if(component != null && component.status === Component.Error)
                console.log(component.errorString)

            if(childRec != null) {
                var array = new Array
                if(internal.busyWidgets.length != 0)
                    array = internal.busyWidgets
                var num = array.length
                array[num] = childRec;

                if(num == 0) {
                    array[0].volume = srcmanager.volume
                    array[0].muted = srcmanager.mute
                    array[0].timeout = srcmanager.timer * 1000
                    array[0].ended.connect(ended)
                    array[0].loaded.connect(loaded)
                    array[0].started.connect(started)
                    array[0].setSource(srcmanager.source)
                    srcmanager.goNext()
                }
                else {
                    array[num].visible = false
                    array[num].volume = srcmanager.volume
                    array[num].muted = srcmanager.mute
                    array[num].timeout = srcmanager.timer * 1000
                    array[num].ended.disconnect(ended)
                    array[num].loaded.disconnect(loaded)
                    array[num].started.disconnect(started)
                    array[num].loaded.connect(internal.isLoaded)
                    array[num].__error.connect(internal.isError)
                    array[num].setSource(srcmanager.source)
                }

                internal.busyWidgets = array
            }
        }
    }

    function start() {
        if(internal.busyWidgets.length) {
            internal.busyWidgets[0].start()
            createObject()
        }
    }

    function pause() {
        if(internal.busyWidgets.length) {
            internal.busyWidgets[0].pause()
        }
    }

    function stop() {
        if(internal.busyWidgets.length) {
            internal.busyWidgets[0].stop()
        }
    }

    function next() {
        if(internal.busyWidgets.length) {
            var array = new Array
            var free_windgets = new Array
            array = internal.busyWidgets
            free_windgets = internal.freeWidgets

            if(array.length > 0) {
                array[0].visible = false
                array[0].ended.disconnect(ended)
                array[0].loaded.disconnect(loaded)
                array[0].started.disconnect(started)
                array[0].stop()
                free_windgets.push(array.shift())
                internal.freeWidgets = free_windgets
            }

            if(array.length > 0) {
                array[0].ended.connect(ended)
                array[0].loaded.connect(loaded)
                array[0].started.connect(started)
                array[0].__error.disconnect(internal.isError)
                array[0].loaded.disconnect(internal.isLoaded)
                array[0].visible = true
                srcmanager.goNext()
                loaded()
            }
            internal.busyWidgets = array
        }
    }

    function run() {
        createObject()
    }
}
