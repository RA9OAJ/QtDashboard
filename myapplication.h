#ifndef MYAPPLICATION_H
#define MYAPPLICATION_H

#include <QApplication>
#include <QDir>
#include <QTimer>
#include <QDebug>
#include "processsharedbuffer.h"

class MyApplication : public QApplication
{
    Q_OBJECT

public:
    explicit MyApplication(int &argc, char **argv, int flags = ApplicationFlags);
    ~MyApplication();

    void setOnlyOneProcess(bool flag, int timeout_sec = 5);
    bool isFirstProcess();

signals:

public slots:
    void scheduler();
protected slots:

private:
    bool _one_proc;
    bool _first_proc;
    int _one_proc_timeout;

    ProcessSharedBuffer *_shared_memory;
    QString _pidstr, _pingstr;
};

#endif // MYAPPLICATION_H
