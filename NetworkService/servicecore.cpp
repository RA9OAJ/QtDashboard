#include "servicecore.h"

ServiceCore::ServiceCore(int &argc, char **argv) :
    QCoreApplication(argc,argv)
{
    parent_uuid = 0;
    scheduler_flag = false;
    uuid = QUuid::createUuid();
    log = new ServiceLog(this);
    log->setTitlePrefix(QString("%1 Pid=%2 ").arg(uuid.toString(),QString::number(applicationPid())));

    QString buf_name = (qApp->applicationName().isEmpty() ? qApp->applicationDirPath() : qApp->applicationName());
    if(!_buffer.connectToBuffer(buf_name))
    {
        log->error(tr("Can't connect to shared buffer"),
                 tr("Can't connect to shared buffer '%1'. It's critical error, application stopped.").arg(buf_name),
                 "default");

        QTimer::singleShot(0,this,SLOT(quit()));
        return;
    }

    log->info(tr("Connected to shared buffer '%1'.").arg(buf_name));
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
        log->info(tr("This process is parent."));

        if(smemdata->contains("__PING"))
        {
            QDateTime last_ping = QDateTime::fromString(smemdata->value("__PING").toString(),"dd.MM.yyyyThh:mm:ss");
            if(last_ping.secsTo(QDateTime::currentDateTime()) > 3)
                _buffer.clearData();
            else
            {
                log->warning(tr("Service can't start"),tr("Service already running (PID=%1)").arg(smemdata->value("__PID").toString()));
                QTimer::singleShot(0,this,SLOT(quit()));
                return;
            }
        }

        createChildProcess();
        emit thisParentProcess();
        thread()->sleep(1);
    }
    else if(isChildProcess())
    {
        log->info(tr("This process is child."));
        _buffer.writeToBuffer("__PID",applicationPid(),true);
        scheduler_flag = true;
        scheduler();
        emit thisChildProcess();
    }
}

ServiceCore::~ServiceCore()
{
    log->info(tr("Process is completed."));

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
    log->setDebugMode(enable);
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
        if(smemdata->value("__PARENT_GUID").toString() == parent_uuid->toString())
            return true;

    return false;
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

    if(!QProcess::startDetached(qApp->applicationFilePath(), param, QString(), &child_pid))
    {
        log->error(tr("Can't create a child process."));
        QTimer::singleShot(0,this,SLOT(quit()));
    }
    else
    {
        _buffer.writeToBuffer("__PID",child_pid,true);
        log->info(tr("Child process created"),tr("Child process PID=%1").arg(QString::number(child_pid)));
    }
}

