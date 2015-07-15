#include "sourcemanager.h"

SourceManager::SourceManager(QObject *parent) :
    QObject(parent)
{
}

QString SourceManager::source() const
{
    return _source;
}


