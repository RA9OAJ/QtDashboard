#ifndef TCPSERVICE_H
#define TCPSERVICE_H

#include <QObject>
#include <QProcess>
#include <QHostAddress>
#include <QCoreApplication>
#include <QDir>
#include <QTcpServer>
#include <QSslSocket>
#include <QHash>
#include <QSettings>
#include <QDebug>

#include "servicecore.h"
#include "../QtDashboard/processsharedbuffer.h"

class TcpService : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpService(QObject *parent = 0);
    ~TcpService();

public slots:
    void parsingParameters();

protected slots:
    void readSettings();

private:
    ProcessSharedBuffer *_shared_buffer;
    QHash<QString,QVariant> _settings;
};

#endif // TCPSERVICE_H
