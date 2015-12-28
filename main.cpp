#include <QApplication>
#include <QtQml>
#include <QQmlApplicationEngine>
#include <QDateTime>
#include <QMap>
#include <QThread>
#include <QDebug>

#include "processsharedbuffer.h"

#include "sourcemanager.h"

bool isFirstProc(ProcessSharedBuffer *buf = 0)
{
    if(buf)
    {
        QMap<QString,QVariant> params = buf->getData();
        int cur_pid = qApp->applicationPid();

        if(params.size() && params.contains("PID") && params.contains("LastPing"))
        {
            if(params.value("PID") != cur_pid && QDateTime::fromString((params.value("LastPing",QString("01.01.1990 01:01:01")).toString()),"dd.MM.yyyy hh:mm:ss").secsTo(QDateTime::currentDateTime()) < 10)
                return false;
            else
            {
                buf->writeToBuffer("PID",cur_pid);
                buf->writeToBuffer("LastPing",QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss"));
                return true;
            }
        }
        else
        {
            buf->writeToBuffer("PID",cur_pid);
            buf->writeToBuffer("LastPing",QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss"));
            return true;
        }
    }
    return false;
}

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);

    ProcessSharedBuffer *shared_memory = new ProcessSharedBuffer();
    shared_memory->connectToBuffer(QString("QtDashboard"));
    if(!isFirstProc(shared_memory))
    {
        qDebug()<<"It's not first process! Exit now.";
        return 0;
    }

    qmlRegisterType<SourceManager>("SourceManager",1,0,"SourceManager");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));

    return app.exec();
}
