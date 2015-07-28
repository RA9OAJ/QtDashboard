#include "sourcemanager.h"

SourceManager::SourceManager(QObject *parent) :
    QObject(parent)
{
    QDir config_path(QDir::homePath()+"/.config/QtDashboard");
    _config_path = config_path.absolutePath();
    if(!config_path.exists())
    {
        if(!config_path.mkpath(config_path.absolutePath()))
        {
            setErrors(CONFIG_ERROR);
            qDebug()<<tr("Unable to create directory '%1'").arg(config_path.absolutePath());
            return;
        }
    }

    QFileInfo cfg(config_path.absolutePath()+"/confix.xml");
    if(!cfg.exists() && createDefaultConfig(config_path.absolutePath()))
    {
        setErrors(CONFIG_ERROR);
        qDebug()<<tr("The configuration file is not found. Application will be closed.");
    }

    setErrors(NO_ERROR);
}

QString SourceManager::source() const
{
    return "file:///home/user/1.mp4";//_source;
}

SourceManager::Errors SourceManager::error() const
{
    return _error;
}

QString SourceManager::errorString() const
{
    switch (_error) {
    case NO_ERROR:
        return tr("No error.");
    case CONFIG_ERROR:
        return tr("The configuration file is not found or is not available.");
    case SOURCE_NOT_FOUND:
        return tr("Directory of sources or source files are not found or are not accessible.");
    case SOURCE_ERROR:
        return tr("The source does not exist or is not available.");
    default:
        break;
    }
    return tr("Uncnown error.");
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
    QFile *fl = new QFile(path);

    if(!fl->open(QFile::ReadOnly | QFile::Text))
    {
        setErrors(SOURCE_NOT_FOUND);
        qDebug()<<"readXmlSourceList: "<<fl->errorString();
        return;
    }

    QXmlStreamReader reader(fl);
    QString cur_element;
    Areas area = ROOT;
    int cur_section = -1;

    while(!reader.atEnd())
    {
        QXmlStreamReader::TokenType token = reader.readNext();
        cur_element = reader.name().toString();
        cur_element = cur_element.toLower();

        if(token == QXmlStreamReader::StartDocument)
            continue;
        else if(token == QXmlStreamReader::EndElement)
        {
            if(cur_element == "section" && area == SECTION)
                area = LIST;
            else if(cur_element == "list" && area == LIST)
                area = ROOT;
        }
        else if(token == QXmlStreamReader::StartElement)
        {
            if(cur_element == "list")
                area = LIST;
            else if(cur_element == "section")
            {
                area = SECTION;
                Section section;

                if(!reader.attributes().isEmpty())
                {
                    QXmlStreamAttributes attributes = reader.attributes();
                    QXmlStreamAttributes::iterator it = attributes.begin();
                    while(it != attributes.end())
                    {
                        section.attribs.insert(it->name().toString().toLower(),it->value().toString());
                        ++it;
                    }
                }

                section.priority = section.attribs.value("priority","5").toInt();
                section.id = section.attribs.value("id","-1").toInt();

                if(section.id == -1)
                {
                    setErrors(SOURCE_ERROR);
                    qDebug()<<tr("Could not find a mandatory attribute 'id' within the structure 'section'");
                    return;
                }

                cur_section = section.id;
                sections.insert(section.id,section);
                continue;
            }

            switch(area)
            {
            case LIST:
                continue;
            case SECTION:
                if(cur_element == "title")
                {
                    sections[cur_section].title = reader.text().toString();
                }
                else if(cur_element == "source")
                {
                    if(!reader.attributes().isEmpty())
                    {
                        QXmlStreamAttributes attributes = reader.attributes();
                        QXmlStreamAttributes::iterator it = attributes.begin();
                        Source source;

                        while(it != attributes.end())
                        {
                            source.insert(it->name().toString().toLower(),it->value().toString());
                            ++it;
                        }

                        if(!source.contains("id"))
                        {
                            qDebug()<<tr("Could not find a mandatory attribute 'id' within the structure 'source'");
                            continue;
                        }

                        sections[cur_section].sources.insert(source.value("id").toInt(),source);
                    }
                    else qDebug()<<tr("Could not find a mandatory attribute 'id' within the structure 'source'");
                }
                else qDebug()<<tr("Invalid element '%1' within the structure 'section'").arg(reader.name().toString());
                break;
            default:
                continue;
            }
        }
    }

    if(reader.hasError())
    {
        setErrors(SOURCE_ERROR);
        qDebug()<<reader.errorString();
    }
    else emit sourceListRead();
}

bool SourceManager::createDefaultConfig(const QString &dir)
{
    QFile fl(dir + "/config.xml");
    if(fl.exists())
    {
        qDebug()<<tr("File '%1' already exists").arg(fl.fileName());
        return false;
    }

    fl.setFileName(":/config.xml");
    if(!fl.copy(QDir::homePath()+"/.config/QtDashboard/config.xml"))
    {
        qDebug()<<tr("Can't create configuration file '%1'").arg(fl.fileName());
        return false;
    }

    return true;
}

void SourceManager::setErrors(SourceManager::Errors _e)
{
    _error = _e;
    if(_error != NO_ERROR)
        emit errorChanget();
}


