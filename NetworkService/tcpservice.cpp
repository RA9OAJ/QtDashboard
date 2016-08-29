#include "tcpservice.h"

TcpService::TcpService(QObject *parent) :
    QTcpServer(parent)
{
    _shared_buffer = new ProcessSharedBuffer(this);
    _shared_buffer->connectToBuffer(QString("%1-%2").arg(qApp->applicationName(),QDir::homePath()));
    readSettings();
}

TcpService::~TcpService()
{
    _shared_buffer->disconnectFromBuffer();
    _shared_buffer->deleteLater();
}

void TcpService::parsingParameters()
{

}

void TcpService::readSettings()
{
    _settings.clear();
    QStringList sett_files;

    sett_files.append(QString());
    sett_files.append(QDir::homePath());

#ifdef Q_OS_WIN32
    sett_files[0] = qApp->applicationDirPath()+"/../../etc/qtdnetservice.ini";
    sett_files[1] += "/AppData";
#endif

#ifdef Q_OS_UNIX
    sett_files[0] = qApp->applicationDirPath()+"/../../etc/QtDNetService/qtdnetservice.ini";
    sett_files[1] += "/.config";
#endif
    sett_files[1] += "/QtDNetService/qtdnetservice.ini";

    for(int i = 0; i < sett_files.size(); ++i)
    {
        if(!QFile::exists(sett_files.value(i)))
        {
            qSce->warning(tr("Settings file '%1' not found.").arg(sett_files.value(i)));
            continue;
        }

        qSce->log(tr("Settings file '%1' found.").arg(sett_files.value(i)));

        QSettings sett(sett_files.value(i),QSettings::IniFormat);
        foreach(QString key, sett.allKeys()){
            _settings.insert(key.toLower(),sett.value(key));
            qSce->log(tr("Read Settings"),QString("%1:%2").arg(key.toLower(),sett.value(key).toString()));
        }
    }

    if(_settings.isEmpty())
    {
        qSce->error(tr("Settings files not found!"),tr("It's a critical error, the application will be closed."));
        QTimer::singleShot(0,qSce,SLOT(quit()));
    }
}
