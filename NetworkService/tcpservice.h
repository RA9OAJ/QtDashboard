#ifndef TCPSERVICE_H
#define TCPSERVICE_H

#include <QObject>
#include <QProcess>
#include <QHostAddress>
#include <QSslSocket>
#include <QSettings>

#include "../QtDashboard/processsharedbuffer.h"

class TcpService : public QTcpServer
{
public:
    TcpService();
    ~TcpService();

public slots:
    void parsingParameters();

private:
    QString litening_ip;
    qint16 listening_port;
    ProcessSharedBuffer *_shared_buffer;
};

#endif // TCPSERVICE_H
