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


class ServiceCore : public QCoreApplication
{
    Q_OBJECT

public:
    explicit ServiceCore(int & argc, char ** argv);
    ~ServiceCore();

signals:
    void thisParentProcess();
    void thisChildProcess();

public slots:
    void setDebugMode(bool enable = true);

public:
    bool isParentProcess() const;
    bool isChildProcess() const;

private slots:
    void scheduler();

protected:
    void createChildProcess();

private:
    ProcessSharedBuffer _buffer;
    ServiceLog *log;
    QUuid uuid;
    QUuid *parent_uuid;
    bool scheduler_flag;
    QMap<QString,QVariant> *smemdata;
};

#endif // SERVICECORE_H
