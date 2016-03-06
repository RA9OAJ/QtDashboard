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

        function isLoaded(src_id) {
            console.log("Next slide is Load!")
        }

        function isError(src_id,err_text) {
            src_id = typeof src_id != "undefined" ? src_id : -1
            err_text = typeof err_text != "undefined" ? err_text : "Undefined error"

            console.log("Next slide not Load. Error: " + err_text)

            srcmanager.sourceError(src_id)

            var num = internal.busyWidgets.length
            var cur_item = -1

            for(var i = 0; i < num; i++)
                if(internal.busyWidgets[i].src_id === src_id)
                    cur_item = i

            if(cur_item === -1)
                return

            if(!cur_item) {
                if(num > 1)
                    parent.next()

                else {
                    delete internal.busyWidgets[0]
                    internal.busyWidgets.splice(0,1)

                    if(srcmanager.current === src_id)
                        srcmanager.goNext()

                    var cur = srcmanager.current
                    parent.createObject()

                    if(srcmanager.current === cur)
                        srcmanager.goPrev()
                }
            }
            else {
                delete internal.busyWidgets[cur_item]
                internal.busyWidgets.splice(cur_item,1)
                srcmanager.goNext()
                createObject()
            }
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
                childRec = internal.getFreeWidget("VIDEO")
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
                /*var array = new Array
                if(internal.busyWidgets.length != 0)
                    array = internal.busyWidgets*/
                var num = internal.busyWidgets.length
                internal.busyWidgets[num] = childRec;

                if(num == 0) {
                    console.log("Vol: " + srcmanager.volume + " " + srcmanager.mute )
                    internal.busyWidgets[0].src_id = srcmanager.current
                    internal.busyWidgets[0].volume = srcmanager.volume
                    internal.busyWidgets[0].muted = srcmanager.mute
                    internal.busyWidgets[0].timeout = srcmanager.timer * 1000
                    internal.busyWidgets[0].ended.connect(ended)
                    internal.busyWidgets[0].loaded.connect(loaded)
                    internal.busyWidgets[0].started.connect(started)
                    internal.busyWidgets[0].__error.connect(internal.isError)
                    internal.busyWidgets[0].setSource(srcmanager.source)
                    srcmanager.goNext()
                }
                else {
                    internal.busyWidgets[num].visible = false
                    internal.busyWidgets[num].src_id = srcmanager.current
                    internal.busyWidgets[num].volume = srcmanager.volume
                    internal.busyWidgets[num].muted = srcmanager.mute
                    internal.busyWidgets[num].timeout = srcmanager.timer * 1000
                    internal.busyWidgets[num].ended.disconnect(ended)
                    internal.busyWidgets[num].loaded.disconnect(loaded)
                    internal.busyWidgets[num].started.disconnect(started)
                    internal.busyWidgets[num].loaded.connect(internal.isLoaded)
                    internal.busyWidgets[num].__error.connect(internal.isError)
                    internal.busyWidgets[num].setSource(srcmanager.source)
                }

                //internal.busyWidgets = array
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
            /*var array = new Array
            var free_windgets = new Array
            array = internal.busyWidgets
            free_windgets = internal.freeWidgets*/

            if(internal.busyWidgets.length > 0) {
                internal.busyWidgets[0].visible = false
                internal.busyWidgets[0].src_id = -1
                internal.busyWidgets[0].ended.disconnect(ended)
                internal.busyWidgets[0].loaded.disconnect(loaded)
                internal.busyWidgets[0].started.disconnect(started)
                internal.busyWidgets[0].__error.disconnect(internal.isError)
                internal.busyWidgets[0].stop()
                internal.freeWidgets.push(internal.busyWidgets.shift())
                //internal.freeWidgets = free_windgets
            }

            if(internal.busyWidgets.length > 0) {
                internal.busyWidgets[0].ended.connect(ended)
                internal.busyWidgets[0].loaded.connect(loaded)
                internal.busyWidgets[0].started.connect(started)
                internal.busyWidgets[0].__error.disconnect(internal.isError)
                internal.busyWidgets[0].loaded.disconnect(internal.isLoaded)
                internal.busyWidgets[0].visible = true
                srcmanager.goNext()
                loaded()
            }
            //internal.busyWidgets = array
        }
    }

    function run() {
        createObject()
    }
}
