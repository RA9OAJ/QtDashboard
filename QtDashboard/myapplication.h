#ifndef MYAPPLICATION_H
#define MYAPPLICATION_H

#include <QApplication>
#include <QProcess>
#include <QDir>
#include <QTimer>
#include <QDebug>
#include <QQuickWindow>
#include "processsharedbuffer.h"

class MyApplication : public QApplication
{
    Q_OBJECT

public:
    explicit MyApplication(int &argc, char **argv, int flags = ApplicationFlags);
    ~MyApplication();

    void setOnlyOneProcess(bool flag, int timeout_sec = 5);
    bool isFirstProcess();
    bool addQMLRoot(QObject *root);

signals:

public slots:
    void parsingParameters();
protected slots:
    void scheduler();
    void activateQMLRootWindow();

protected:
    virtual bool isValidParams(const QStringList &args) const;

private:
    bool _one_proc;
    bool _first_proc;
    int _one_proc_timeout;

    ProcessSharedBuffer *_shared_memory;
    QString _pidstr, _pingstr, _argstr;
    QQuickWindow *_qml_root_win;
};

#endif // MYAPPLICATION_H
