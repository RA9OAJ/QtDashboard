#include "processsharedbuffer.h"

ProcessSharedBuffer::ProcessSharedBuffer(QObject *parent) :
    QObject(parent)
{
    _buffer = 0;
    _last_upd = 0;
    _enable_flag = false;
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
        {
            bool result = _buffer->attach(QSharedMemory::ReadWrite);
            if(result)
            {
                _enable_flag = true;
                scheduler();
            }
            return result;
        }
        else
        {
            _enable_flag = true;
            scheduler();
            return true;
        }
    }
    return false;
}

bool ProcessSharedBuffer::disconnectFromBuffer()
{
    if(_buffer && _buffer->isAttached())
    {
        bool result = _buffer->detach();
        if(!result) _enable_flag = false;
        return result;
    }
    return false;
}

QMap<QString, QVariant> ProcessSharedBuffer::getData()
{
    QMap<QString, QVariant> inmap;

    if(_buffer && _buffer->isAttached())
    {
        if(!_buffer->lock())
            return inmap;

        QByteArray inbuf;
        inbuf.resize(_buffer->size());
        memcpy(inbuf.data(),_buffer->data(),_buffer->size());

        QDataStream in(&inbuf,QIODevice::ReadOnly);

        int count;
        in >> count;
        in >> count;

        for(int i = 0; i < count; ++i)
        {
            QString key;
            int type_id = QMetaType::UnknownType;

            in >> key;
            in >> type_id;
            switch (type_id) {
            case QMetaType::Int:
            {
                int tmpval = 0;
                in >> tmpval;
                inmap.insert(key,QVariant(tmpval));
                break;
            }

            case QMetaType::Bool:
            {
                bool boolval = false;
                in >> boolval;
                inmap.insert(key,QVariant(boolval));
                break;
            }

            case QMetaType::UInt:
            {
                unsigned int uintval = 0;
                in >> uintval;
                inmap.insert(key,QVariant(uintval));
                break;
            }

            case QMetaType::Long:
            case QMetaType::LongLong:
            {
                long long llval = 0;
                in >> llval;
                inmap.insert(key,QVariant(llval));
                break;
            }

            case QMetaType::Float:
            {
                float fval = 0.0;
                in >> fval;
                inmap.insert(key,QVariant(fval));
                break;
            }

            case QMetaType::Double:
            {
                double dval = 0.0;
                in >> dval;
                inmap.insert(key,QVariant(dval));
                break;
            }

            case QMetaType::QString:
            {
                QString str;
                in >> str;
                inmap.insert(key,QVariant(str));
                break;
            }

            default:
                continue;
            }
        }

        _buffer->unlock();
        return inmap;
    }
    return inmap;
}

bool ProcessSharedBuffer::writeToBuffer(const QString &key, const QVariant &val)
{
    if(_buffer && _buffer->isAttached())
    {
        QMap<QString,QVariant> data;
        data = getData();
        data.insert(key,val);

        return writeData(data);
    }
    return false;
}

bool ProcessSharedBuffer::deleteData(const QString &key)
{
    if(_buffer && _buffer->isAttached())
    {
        QMap<QString,QVariant> data;
        data = getData();
        data.remove(key);

        return writeData(data);
    }
    return false;
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
    scanBuffer();
    if(_enable_flag)
        QTimer::singleShot(500,this,SLOT(scheduler()));
}

bool ProcessSharedBuffer::writeData(const QMap<QString,QVariant> &data)
{
    if(_buffer && _buffer->isAttached())
    {
        clearData();

        bool need_emit = false;

        if(!_buffer->lock())
            return false;

        QByteArray outbuf;
        outbuf.resize(_buffer->size());
        memcpy(outbuf.data(),_buffer->data(),_buffer->size());

        QDataStream out(&outbuf,QIODevice::ReadWrite);
        int buf_cnt = 0;

        out >> buf_cnt;
        out.device()->seek(0);
        if(buf_cnt > _last_upd)
        {
            need_emit = true;
            _last_upd = buf_cnt;
        }

        if(_last_upd == std::numeric_limits<int>::max())
            _last_upd = 1;
        else
            ++_last_upd;

        out << _last_upd << data.size();

        foreach (QString key, data.keys()) {
            out << key;

            switch (data.value(key).type()) {
            case QMetaType::Bool:
                out << data.value(key).type();
                out << data.value(key).toBool();
                break;

            case QMetaType::Int:
                out << data.value(key).type();
                out << data.value(key).toInt();
                break;

            case QMetaType::UInt:
                out << data.value(key).type();
                out << data.value(key).toUInt();
                break;

            case QMetaType::LongLong:
                out << data.value(key).type();
                out << data.value(key).toLongLong();
                break;

            case QMetaType::Double:
                out << data.value(key).type();
                out << data.value(key).toDouble();
                break;

            case QMetaType::QString:
                out << data.value(key).type();
                out << data.value(key).toString();
                break;

            default:
                break;
            }
        }

        memcpy(_buffer->data(),outbuf.data(),outbuf.size());
        _buffer->unlock();

        if(need_emit)
            emit dataAvailable();
        return true;
    }
    return false;
}

void ProcessSharedBuffer::scanBuffer()
{
    if(_buffer && _buffer->isAttached())
    {
        _buffer->lock();
        QByteArray buf;
        buf.resize(8);
        memcpy(buf.data(),_buffer->data(),buf.size());

        QDataStream in(&buf,QIODevice::ReadOnly);
        int new_cnt = 0;
        in >> new_cnt;
        _buffer->unlock();

        if(new_cnt != _last_upd)
            emit dataAvailable();
    }
}
