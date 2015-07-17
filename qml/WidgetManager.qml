import QtQuick 2.0
import SourceManager 1.0

Item {
    width: parent.width
    height: parent.height

    property var objectsOnCache: 2

    signal loaded //генерится при полной загрузке материала
    signal ended //генерится при таймауте тамера времени показа материала
    signal started //генерится в момент начала показа материала
    signal load //генерится в начале загрузки материала

    Component.onCompleted: {
        var component = Qt.createComponent("WebWidget.qml");
        main.title = component.errorString()
        if (component.status === Component.Ready) {
            var childRec = component.createObject(frame);
            web = childRec;
            web.setSource("http://roskazna.ru/");
            web.ended.connect(ended);
            web.loaded.connect(loaded);
        }
    }
}
