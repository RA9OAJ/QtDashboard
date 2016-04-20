#ifndef SERVICECORE_H
#define SERVICECORE_H

#include <QCoreApplication>
#include <QTimer>
#include "../QtDashboard/processsharedbuffer.h"


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

public:
    bool isParentProcess() const;
    bool isChildProcess() const;

private slots:
    void scheduler();

private:
    ProcessSharedBuffer _buffer;
};

#endif // SERVICECORE_H
