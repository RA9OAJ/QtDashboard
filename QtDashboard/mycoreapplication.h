#ifndef MYCOREAPPLICATION_H
#define MYCOREAPPLICATION_H

#include <QCoreApplication>
#include <QDir>
#include <QTimer>
#include <QDebug>
#include "processsharedbuffer.h"

class MyCoreApplication : public QCoreApplication
{
    Q_OBJECT
public:
    explicit MyCoreApplication(int &argc, char** argv, int flags = ApplicationFlags);
    ~MyCoreApplication();

    void setOnlyOneProcess(bool flag, int timeout_sec = 5);
    void setSharedValue(const QString &name, const QVariant &val, bool sysflag = false);
    QMap<QString,QVariant> getSharedValue(bool sysflag = false) const;
    bool isFirstProcess();

signals:

public slots:
    void parsingParameters();
protected slots:
    void scheduler();

private:
    bool _one_proc;
    bool _first_proc;
    int _one_proc_timeout;

    ProcessSharedBuffer *_shared_memory;
    QString _pidstr, _pingstr, _argstr;
};

#endif // MYCOREAPPLICATION_H
