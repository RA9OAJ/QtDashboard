#ifndef TCPSERVICE_H
#define TCPSERVICE_H

#include <QObject>
#include <QProcess>
#include <QHostAddress>
#include <QCoreApplication>
#include <QDir>
#include <QTcpServer>
#include <QSslSocket>
#include <QSettings>

#include "../QtDashboard/processsharedbuffer.h"

class TcpService : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpService(QObject *parent = 0);
    ~TcpService();

public slots:
    void parsingParameters();

private:
    QString litening_ip;
    qint16 listening_port;
    ProcessSharedBuffer *_shared_buffer;
};

#endif // TCPSERVICE_H
