#include "myapplication.h"

MyApplication::MyApplication(int &argc, char **argv, int flags) :
        QApplication(argc, argv, flags)
{
    _one_proc = false;
    _one_proc_timeout = 2500;
    _first_proc = true;

    _shared_memory = new ProcessSharedBuffer(this);
    _shared_memory->connectToBuffer(QString("%1-%2").arg(QCoreApplication::applicationName(),QDir::homePath()));
    connect(_shared_memory,SIGNAL(dataAvailable()),this,SLOT(parsingParameters()));

    _pidstr = QString("PID_%1").arg(QDir::homePath());
    _pingstr = QString("LastPing_%1").arg(QDir::homePath());
    _argstr = QString("ARG_%1").arg(QDir::homePath());
}

MyApplication::~MyApplication()
{
    if(_shared_memory->isConnected())
        _shared_memory->disconnectFromBuffer();
    _shared_memory->deleteLater();
}

void MyApplication::setOnlyOneProcess(bool flag, int timeout_sec)
{
    _one_proc = flag;
    _one_proc_timeout = timeout_sec * 1000 / 4;

    if(flag)
    {
        _first_proc = isFirstProcess();
        if(_first_proc)
            QTimer::singleShot(_one_proc_timeout,this,SLOT(scheduler()));
        else {_first_proc = false;}
    }
    else
        _first_proc = true;
}

bool MyApplication::isFirstProcess()
{
    if(_one_proc && !_first_proc)
        return _first_proc;

    if(_shared_memory && _shared_memory->isConnected())
    {
        QMap<QString,QVariant> params = _shared_memory->getData();
        int cur_pid = qApp->applicationPid();

        if(params.size() && params.contains(_pidstr) && params.contains(_pingstr))
        {
            if(params.value(_pidstr) != cur_pid && QDateTime::fromString((params.value(_pingstr,QString("01.01.1990 01:01:01")).toString()),"dd.MM.yyyy hh:mm:ss").secsTo(QDateTime::currentDateTime()) < _one_proc_timeout * 4 / 1000)
            {
                _first_proc = false;
                return false;
            }
            else
            {
                _shared_memory->clearData();
                _shared_memory->writeToBuffer(_pidstr,cur_pid,true);
                _shared_memory->writeToBuffer(_pingstr,QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss"),true);
                _first_proc = true;
                return true;
            }
        }
        else
        {
            _shared_memory->writeToBuffer(_pidstr,cur_pid,true);
            _shared_memory->writeToBuffer(_pingstr,QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss"),true);
            _first_proc = true;
            return true;
        }
    }
    _first_proc = false;
    return false;
}

void MyApplication::parsingParameters()
{
    if(!_first_proc && arguments().size() > 1)
    {
        QString out;
        foreach (QString curstr, arguments()) {
            if(curstr == arguments().value(0))
                continue;
            out += curstr + "\r\n";
        }
        _shared_memory->writeToBuffer(_argstr,out);
    }
    else
    {
        QStringList params;
        if(qobject_cast<ProcessSharedBuffer*>(sender()) == _shared_memory)
            params = _shared_memory->getData().value(_argstr).toString().split("\r\n",QString::SkipEmptyParts);
        else params = arguments();

        if(isValidParams(params))
        {
            foreach (QString cur, params) {
                if(cur == "--show-cursor")
                    qApp->setOverrideCursor(Qt::ArrowCursor);
                else if(cur == "--hide-cursor")
                    qApp->setOverrideCursor(Qt::BlankCursor);
                else if(cur == "--quit")
                    QTimer::singleShot(0,qApp,SLOT(quit()));
                else if(cur == "--restart")
                {
                    QProcess proc;
                    _shared_memory->clearData();
                    proc.startDetached(arguments().value(0));
                    qApp->quit();
                }
                else if(cur == "--reload")
                {
                    qDebug()<<"Reload!";
                }
            }
        }
    }
}

void MyApplication::scheduler()
{
    if(_one_proc && _first_proc)
    {
        _shared_memory->writeToBuffer(_pingstr,QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss"),true);
        QTimer::singleShot(_one_proc_timeout, this, SLOT(scheduler()));
    }
}

bool MyApplication::isValidParams(const QStringList &args) const
{
    //QString last_arg;
    QString cur;

    for(int i = 1; i < args.size(); i++) {
        cur = args.value(i);
        if(cur == "--show-cursor"
                || cur == "--hide-cursor"
                || cur == "--reload"
                || cur == "--restart"
                || cur == "--quit")
            return true;
        else return false;
    }

    return false;
}

