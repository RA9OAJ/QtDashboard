#ifndef SERVICELOG_H
#define SERVICELOG_H

#include <QObject>
#include <QCoreApplication>
#include <QDateTime>
#include <QFileInfo>
#include <QDir>
#include <QFile>
#include <QMap>
#include <QPair>
#include <QDebug>
#include "servicelog.h"

class ServiceLog : public QObject
{
    Q_OBJECT

    enum LogType {
        FILE,
        UNIX,
        WINDOWS,
        WINDOWS7
    };

    enum EventType {
        INFO,
        WARNING,
        ERROR
    };

public:
    explicit ServiceLog(QObject *parent = 0);

    bool createLog(const QString &name, const QString &path = QString());

signals:

public slots:
    void setDefaultLogFileName(const QString &name, const QString &filename);
    void log(const QString &title, const QString &text = QString(), const QString &logname = "default", EventType type = INFO);
    void error(const QString &title, const QString &text = QString(), const QString &logname = "default");
    void warning(const QString &title, const QString &text = QString(), const QString &logname = "default");
    void info(const QString &title, const QString &text = QString(), const QString &logname = "default");
    void setDebugMode(bool mode = false);

private:
    LogType log_type;
    QMap<QString,QPair<QString,QString> > logs;
    QString appname;
    bool debug_mode;

};

#endif // SERVICELOG_H
