#ifndef SERVICECORE_H
#define SERVICECORE_H

#include <QCoreApplication>
#include <QTimer>
#include <QUuid>
#include <QStringList>
#include <QProcess>
#include <QRegExp>
#include <QMap>

#include "../QtDashboard/processsharedbuffer.h"
#include "servicelog.h"

#define qSce ((ServiceCore*)ServiceCore::instance())

class ServiceCore : public QCoreApplication
{
    Q_OBJECT

public:
    explicit ServiceCore(int & argc, char ** argv);
    ~ServiceCore();

    bool isParentProcess() const;
    bool isChildProcess() const;
    bool childProcessCreated() const;

signals:
    void thisParentProcess();
    void thisChildProcess();
    void childCreated(qint64 pid);
    void childProcessSuccess();
    void childProcessFailure(int error);

public slots:
    void log(const QString &title, const QString &text = QString(), const QString &logname = "default", ServiceLog::EventType type = ServiceLog::INFO);
    void error(const QString &title, const QString &text = QString(), const QString &logname = "default");
    void warning(const QString &title, const QString &text = QString(), const QString &logname = "default");
    void info(const QString &title, const QString &text = QString(), const QString &logname = "default");
    void setDebugMode(bool enable = true);
    void childProcessStartSuccess();
    void childProcessStartFailure(int error);

private slots:
    void scheduler();

private:
    void createChildProcess();
    void waitChildProcess(qint64 child_pid);

    ProcessSharedBuffer _buffer;
    ServiceLog *_log;
    QUuid uuid;
    QUuid *parent_uuid;
    bool scheduler_flag;
    bool child_created;
    QMap<QString,QVariant> *smemdata;
};

#endif // SERVICECORE_H
