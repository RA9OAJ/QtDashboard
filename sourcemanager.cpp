#include "sourcemanager.h"

SourceManager::SourceManager(QObject *parent) :
    QObject(parent)
{
}

QString SourceManager::source() const
{
    return "http://roskazna.ru/";//_source;
}

SourceManager::MediaTypes SourceManager::sourceType() const
{
    return WEB;
}

SourceManager::MediaTypes SourceManager::nextSourceType() const
{
    return NONE;
}

SourceManager::MediaTypes SourceManager::prevSourceType() const
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


