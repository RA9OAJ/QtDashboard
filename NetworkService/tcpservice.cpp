#include "tcpservice.h"

TcpService::TcpService()
{
    _shared_buffer = new ProcessSharedBuffer(this);
    _shared_buffer->connectToBuffer(QString("%1-%2").arg(QCoreApplication::applicationName(),QDir::homePath()));
}

TcpService::~TcpService()
{

}

void TcpService::parsingParameters()
{

}
