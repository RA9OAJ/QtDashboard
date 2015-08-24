#include <QApplication>
#include <QtQml>
#include <QQmlApplicationEngine>

#include "sourcemanager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    //app.setOverrideCursor(Qt::BlankCursor);

    qmlRegisterType<SourceManager>("SourceManager",1,0,"SourceManager");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));

    return app.exec();
}
