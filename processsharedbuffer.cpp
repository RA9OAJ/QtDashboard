#include "processsharedbuffer.h"

ProcessSharedBuffer::ProcessSharedBuffer(QObject *parent) :
    QObject(parent)
{
    _buffer = 0;
}

ProcessSharedBuffer::~ProcessSharedBuffer()
{
    if(_buffer)
        _buffer->deleteLater();
}

bool ProcessSharedBuffer::connectToBuffer(const QString &buffer_name, int buf_size)
{
    if(!_buffer)
    {
        _buffer = new QSharedMemory(buffer_name, this);
        if(!_buffer->create(buf_size,QSharedMemory::ReadWrite))
            return _buffer->attach(QSharedMemory::ReadWrite);
        else return true;
    }
    return false;
}

bool ProcessSharedBuffer::disconnectFromBuffer()
{
    if(_buffer && _buffer->isAttached())
        return _buffer->detach();
    return false;
}

QMap<QString, QVariant> ProcessSharedBuffer::getData()
{

}

bool ProcessSharedBuffer::writeToBuffer(const QString &key, const QVariant &val)
{

}

bool ProcessSharedBuffer::deleteData(const QString &key)
{

}

bool ProcessSharedBuffer::clearData()
{
    if(_buffer && _buffer->isAttached())
    {
        if(!_buffer->lock())
            return false;

        memset(_buffer->data(),0x0,_buffer->size());

        _buffer->unlock();
    }
    return false;
}

bool ProcessSharedBuffer::isConnected() const
{
    if(_buffer)
        return _buffer->isAttached();
    return false;
}

QSharedMemory::SharedMemoryError ProcessSharedBuffer::error() const
{
    if(_buffer)
        return _buffer->error();
    return QSharedMemory::NoError;
}

QString ProcessSharedBuffer::errorString() const
{
    if(_buffer)
        return _buffer->errorString();
    return QString();
}

int ProcessSharedBuffer::size() const
{
    if(_buffer)
        return _buffer->size();
    return 0;
}

void ProcessSharedBuffer::scheduler()
{

}

void ProcessSharedBuffer::scanBuffer()
{

}
