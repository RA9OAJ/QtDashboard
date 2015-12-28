#include <QtQml>
#include <QQmlApplicationEngine>
#include <QDateTime>
#include <QMap>
#include <QThread>
#include <QDebug>

#include "myapplication.h"
#include "processsharedbuffer.h"
#include "sourcemanager.h"


int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationName("QtDashboard");
    QCoreApplication::setOrganizationName("Sarvaritdinov Ravil' <ra9oaj@gmail.com>");
    QCoreApplication::setApplicationVersion("0.0.1-alpha");

    MyApplication app(argc, argv);
    app.setOnlyOneProcess(true,5);

    if(!app.isFirstProcess())
    {
        qDebug()<<"It's not first process! Exit now.";
        return 0;
    }

    qmlRegisterType<SourceManager>("SourceManager",1,0,"SourceManager");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));

    return app.exec();
}
