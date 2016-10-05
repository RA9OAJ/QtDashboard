#include "servicecore.h"

ServiceCore::ServiceCore(int &argc, char **argv) :
    QCoreApplication(argc,argv)
{
#ifdef Q_OS_UNIX
    _last_euid = _cur_euid = geteuid();
#elif
    _last_euid = _cur_euid = -1;
#endif

    parent_uuid = 0;
    child_created = false;
    scheduler_flag = false;
    uuid = QUuid::createUuid();
    _log = new ServiceLog(this);
    _log->setTitlePrefix(QString("%1 Pid=%2 ").arg(uuid.toString(),QString::number(applicationPid())));

    QString buf_name = (qSce->applicationName().isEmpty() ? qSce->applicationDirPath() : qSce->applicationName());
    if(!_buffer.connectToBuffer(buf_name))
    {
        _log->error(tr("Can't connect to shared buffer"),
                 tr("Can't connect to shared buffer '%1'. It's critical error, application stopped.").arg(buf_name),
                 "default");

        QTimer::singleShot(0,this,SLOT(quit()));
        return;
    }

    _log->info(tr("Connected to shared buffer '%1'.").arg(buf_name));
    smemdata = new QMap<QString,QVariant>(_buffer.getData(true));

    int id = arguments().indexOf(QRegExp("^GUID=\\{[0-9a-fA-F-]+\\}"));
    if(id != -1)
    {
        parent_uuid = new QUuid((arguments().value(id)).replace("GUID=",""));
        if(parent_uuid->isNull())
        {
            delete parent_uuid;
            parent_uuid = 0;
        }
    }

    if(isParentProcess())
    {
        _log->info(tr("This process is parent."));

        if(smemdata->contains("__PING"))
        {
            QDateTime last_ping = QDateTime::fromString(smemdata->value("__PING").toString(),"dd.MM.yyyyThh:mm:ss");
            if(last_ping.secsTo(QDateTime::currentDateTime()) > 3)
                _buffer.clearData();
            else
            {
                _log->warning(tr("Service can't start"),tr("Service already running (PID=%1)").arg(smemdata->value("__PID").toString()));
                QTimer::singleShot(0,this,SLOT(quit()));
                return;
            }
        }

        createChildProcess();
        emit thisParentProcess();
    }
    else if(isChildProcess())
    {
        _log->info(tr("This process is child."));
        //_buffer.writeToBuffer("__PID",applicationPid(),true);
        scheduler_flag = true;
        scheduler();
        emit thisChildProcess();
    }
}

ServiceCore::~ServiceCore()
{
    _log->info(tr("Process is completed."));

    scheduler_flag = false;

    if(parent_uuid)
        delete parent_uuid;

    if(_buffer.isConnected())
    {
        if(isChildProcess())
            _buffer.clearData();

        _buffer.disconnectFromBuffer();
    }

    if(smemdata)
        delete smemdata;
}

void ServiceCore::setDebugMode(bool enable)
{
    _log->setDebugMode(enable);
}

void ServiceCore::childProcessStartSuccess()
{
    if(isChildProcess())
        _buffer.writeToBuffer("__STARTED",applicationPid(),true);
}

void ServiceCore::childProcessStartFailure(int error)
{
    if(isChildProcess())
        _buffer.writeToBuffer("__FAILURE",error,true);
}

void ServiceCore::beginChangeUID()
{
#ifdef Q_OS_UNIX
    _last_euid = geteuid();
    _log->info(tr("Last uid='%1'.").arg(_last_euid));
#endif
}

void ServiceCore::endChangeUID()
{
#ifdef Q_OS_UNIX
    _cur_euid = geteuid();
    _log->info(tr("New uid='%1'.").arg(_cur_euid));
#endif
}

bool ServiceCore::isParentProcess() const
{
    if(!parent_uuid)
        return true;

    return false;
}

bool ServiceCore::isChildProcess() const
{
    if(smemdata && smemdata->contains("__PARENT_GUID") && parent_uuid)
        if(smemdata->value("__PARENT_GUID").toString() == parent_uuid->toString() && smemdata->value("__PID").toLongLong() == qSce->applicationPid())
            return true;

    return false;
}

bool ServiceCore::childProcessCreated() const
{
    return child_created;
}

void ServiceCore::log(const QString &title, const QString &text, const QString &logname, ServiceLog::EventType type)
{
    _log->log(title,text,logname,type);
}

void ServiceCore::error(const QString &title, const QString &text, const QString &logname)
{
    _log->error(title,text,logname);
}

void ServiceCore::warning(const QString &title, const QString &text, const QString &logname)
{
    _log->warning(title,text,logname);
}

void ServiceCore::info(const QString &title, const QString &text, const QString &logname)
{
    _log->info(title,text,logname);
}

void ServiceCore::scheduler()
{
    if(!scheduler_flag)
        return;

    _buffer.writeToBuffer("__PING",QDateTime::currentDateTime().toString("dd.MM.yyyyThh:mm:ss"),true);
    QTimer::singleShot(100,this,SLOT(scheduler()));
}

void ServiceCore::createChildProcess()
{
    QStringList param;
    param << QString("GUID=%1").arg(uuid.toString());

    _buffer.writeToBuffer("__PARENT_GUID",uuid.toString(),true);
    _buffer.writeToBuffer("__PING",QDateTime::currentDateTime().toString("dd.MM.yyyyThh:mm:ss"),true);

    qint64 child_pid = -1;

    if(!QProcess::startDetached(qSce->applicationFilePath(), param, QString(), &child_pid))
    {
        _log->error(tr("Can't create a child process."));
        QTimer::singleShot(0,this,SLOT(quit()));
    }
    else
    {
        child_created = true;
        _buffer.writeToBuffer("__PID",child_pid,true);
        _log->info(tr("Child process created"),tr("Child process PID=%1").arg(QString::number(child_pid)));
        emit childCreated(child_pid);
        waitChildProcess(child_pid);
    }
}

void ServiceCore::waitChildProcess(qint64 child_pid)
{
    while(_buffer.getData(true).value("__STARTED").toLongLong() != child_pid && !_buffer.getData(true).contains("__FAILURE"))
        qSce->thread()->msleep(10);

    QMap<QString,QVariant> smem = _buffer.getData(true);

    if(smem.contains("__FAILURE"))
    {
        _buffer.deleteData("__FAILURE");
        _log->error(tr("Child process start FAILURE"),tr("Error code %1").arg(smem.value("__FAILURE").toString()));
        emit childProcessFailure(smem.value("__FAILURE").toInt());
    }
    else
    {
        _buffer.deleteData("__STARTED");
        emit childProcessSuccess();
    }
}

