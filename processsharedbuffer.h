#ifndef PROCESSSHAREDBUFFER_H
#define PROCESSSHAREDBUFFER_H

#include <QObject>
#include <QSharedMemory>
#include <QTimer>
#include <QMap>
#include <QVariant>
#include <QByteArray>
#include <QDateTime>
#include <QMetaType>
#include <QDataStream>
#include <QDebug>

class ProcessSharedBuffer : public QObject
{
    Q_OBJECT
public:
    explicit ProcessSharedBuffer(QObject *parent = 0);
    ~ProcessSharedBuffer();

    bool connectToBuffer(const QString &buffer_name, int buf_size = 4096);
    bool disconnectFromBuffer();
    QMap<QString,QVariant> getData(bool sysflag = false);
    bool writeToBuffer(const QString &key, const QVariant &val, bool sysflag = false);
    bool deleteData(const QString &key);
    bool clearData();

    bool isConnected() const;
    QSharedMemory::SharedMemoryError error() const;
    QString errorString() const;
    int size() const;

signals:
    void dataAvailable();

public slots:
    void scanBuffer();

protected slots:
    void scheduler();

protected:
    bool writeData(const QMap<QString,QVariant> &data, bool sysflag = false);

private:
    bool _enable_flag;
    int _last_upd;
    QSharedMemory *_buffer;
};

#endif // PROCESSSHAREDBUFFER_H
