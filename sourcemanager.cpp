#include "sourcemanager.h"

SourceManager::SourceManager(QObject *parent) :
    QObject(parent)
{
}

QString SourceManager::source() const
{
    return "file:///home/user/1.mp4";//_source;
}

SourceManager::MediaTypes SourceManager::sourceType() const
{
    return VIDEO;
}

SourceManager::MediaTypes SourceManager::nextSourceType(int seek) const
{
    return NONE;
}

SourceManager::MediaTypes SourceManager::prevSourceType(int seek) const
{
    return NONE;
}

int SourceManager::size() const
{
    return 1;
}

void SourceManager::goNext()
{

}

void SourceManager::goPrev()
{

}

void SourceManager::nextLoaded()
{

}

void SourceManager::nextError()
{

}

void SourceManager::readXmlSourceList(const QString &path)
{
    QFile fl;
    fl.setFileName(path);

    if(!fl.open(QFile::ReadOnly))
    {
        qDebug()<<"readXmlSourceList: "<<fl.errorString();
        return;
    }

    QXmlStreamReader reader;
    reader.addData(fl.readAll());
    fl.close();

    while(!reader.atEnd())
    {
        if(reader.isStartElement())
            qDebug()<<reader.name();
        reader.readNext();
    }

    if(reader.hasError())
        qDebug()<<reader.errorString();
    else emit sourceListRead();
}


