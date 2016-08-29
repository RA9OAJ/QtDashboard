#include "servicelog.h"

ServiceLog::ServiceLog(QObject *parent) : QObject(parent)
{
    QFileInfo fi(qApp->applicationFilePath());
    appname = qApp->applicationName().isEmpty() ? fi.fileName() : qApp->applicationName();

#ifdef Q_OS_WIN32
    log_type = ServiceLog::WINDOWS;
    logs.insert("default", qApp->applicationDirPath());
#endif
#ifdef Q_OS_UNIX
    log_type = ServiceLog::UNIX;
    QPair<QString,QString> log_path;
    QFileInfo flinfo(QString("/var/log/")+appname);
    if(flinfo.exists() && flinfo.isWritable())
        log_path.first = "/var/log/"+appname;
    else
    {
        log_path.first = QString("%1/.config/%2").arg(QDir::homePath(),appname);

        if(!QFile::exists(log_path.first))
        {
            QDir dir;
            dir.mkpath(log_path.first);
        }
    }

    log_path.second = appname + ".log";
    logs.insert("default",log_path);
#else
    log_type = ServiceLog::FILE;
    logs.insert("default") = qApp->applicationDirPath();
#endif

    debug_mode = true;
}

bool ServiceLog::createLog(const QString &name, const QString &path)
{
    QStringList _logs = logs.keys();

    if(_logs.contains(name))
    {
        qDebug()<< appname <<": ServiceLog: " << tr("log \"%1\" already exists.").arg(name);
        return false;
    }

    QPair<QString,QString> log_path;

    if(!path.isEmpty() && QFile::exists(path))
        log_path.first = path;
    else log_path.first = qApp->applicationDirPath();

    switch (log_type) {
    case UNIX:
        if(QFile::exists("/var/log"))
            log_path.first = "/var/log";
        log_path.second = name + ".log";
        break;

    case WINDOWS:
        log_path.first = qApp->applicationDirPath();
        log_path.second = name + ".evt";
        break;
    case WINDOWS7:
        log_path.first = qApp->applicationDirPath();
        log_path.second = name + ".evtx";
    default:
        break;
    }

    logs.insert(name,log_path);

    return true;
}

void ServiceLog::setTitlePrefix(const QString &prefix)
{
    _prefix = prefix;
}

void ServiceLog::setDefaultLogFileName(const QString &name, const QString &filename)
{
    if(!logs.contains(name))
    {
        qDebug()<< appname <<": ServiceLog: " << tr("log \"%1\" not exists.").arg(name);
        return;
    }

    logs[name].second = filename;
}

void ServiceLog::log(const QString &title, const QString &text, const QString &logname, ServiceLog::EventType type)
{
    QString message = QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss ");

    switch (type) {
    case ERROR:
        message += "Error: ";
        break;
    case WARNING:
        message += "Warning: ";
        break;
    default:
        message += "Info: ";
        break;
    }

    if(!_prefix.isEmpty())
        message += _prefix + " ";

    message += title;

    if(!text.isEmpty())
        message += QString(": %1").arg(text);

    if(debug_mode)
        qDebug()<<message;

    switch (log_type) {
    case WINDOWS:

        break;
    case WINDOWS7:

        break;
    default:
        QFile fl(logs.value(logname).first + "/" + logs.value(logname).second);
        if(!fl.open(QIODevice::ReadWrite | QIODevice::Text))
        {
            qDebug() << appname <<": ServiceLog: " << "can't open log file " << fl.fileName() << ":" << fl.errorString();
            return;
        }

        message += "\r\n";
        fl.seek(fl.size());
        fl.write(message.toUtf8());
        fl.close();
        break;
    }
}

void ServiceLog::error(const QString &title, const QString &text, const QString &logname)
{
    log(title,text,logname,ERROR);
}

void ServiceLog::warning(const QString &title, const QString &text, const QString &logname)
{
    log(title,text,logname,WARNING);
}

void ServiceLog::info(const QString &title, const QString &text, const QString &logname)
{
    log(title,text,logname,INFO);
}

void ServiceLog::setDebugMode(bool mode)
{
    debug_mode = mode;
}

