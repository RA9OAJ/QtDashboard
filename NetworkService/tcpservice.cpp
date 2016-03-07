#include "tcpservice.h"

TcpService::TcpService(QObject *parent) :
    QTcpServer(parent)
{
    _shared_buffer = new ProcessSharedBuffer(this);
    _shared_buffer->connectToBuffer(QString("%1-%2").arg(qApp->applicationName(),QDir::homePath()));
}

TcpService::~TcpService()
{
    _shared_buffer->disconnectFromBuffer();
    _shared_buffer->deleteLater();
}

void TcpService::parsingParameters()
{

}
