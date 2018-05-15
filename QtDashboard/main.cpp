#include <QtQml>
#include <QQmlApplicationEngine>
#include <QDateTime>
#include <QScopedPointer>
#include <QDesktopWidget>
#include <QMap>
#include <QThread>
#include <QDebug>
#include <QQuickWindow>
#include <QLoggingCategory>

#include "myapplication.h"
#include "mycoreapplication.h"
#include "processsharedbuffer.h"
#include "sourcemanager.h"


int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationName("QtDashboard");
    QCoreApplication::setOrganizationName("Sarvaritdinov Ravil' <ra9oaj@gmail.com>");
    QCoreApplication::setApplicationVersion("0.0.1-alpha");

    MyCoreApplication *capp = new MyCoreApplication(argc,argv);
    MyApplication *app = 0;
    capp->setOnlyOneProcess(true,5);

    if(!capp->isFirstProcess())
    {
        capp->parsingParameters();
        delete capp;
        qDebug()<<"It's not first process! Exit now.";
        return 0;
    }
    else
    {
        delete capp;
        app = new MyApplication(argc,argv);
        app->setOnlyOneProcess(true);
        app->parsingParameters();
        //QLoggingCategory::setFilterRules("*.debug=true; qt.*=false");
    }

    qmlRegisterType<SourceManager>("SourceManager",1,0,"SourceManager");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QString("%1/../share/QtDashboard/templates/DefaultTemplate/main.qml").arg(qApp->applicationDirPath())));
    app->addQMLRoot(engine.rootObjects()[0]);

    return app->exec();
}
