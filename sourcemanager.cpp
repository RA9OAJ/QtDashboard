#include "sourcemanager.h"

SourceManager::SourceManager(QObject *parent) :
    QObject(parent)
{
    QDir config_path(QDir::homePath()+"/.config/QtDashboard");
    _config_path = config_path.absolutePath();

    _status = LOADING;

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

    if(!loadXmlConfig(QDir::homePath()+"/.config/QtDashboard/config.xml"))
        setErrors(CONFIG_ERROR);

    _cur_id = 0;
    _mute = false;
    _volume = 1.0;
    _visibilityInterval = 30;
    _loop = true;

    _error = NO_ERROR;
    _status = READY;
    emit statusChanged(_status);
}

QString SourceManager::source() const
{
    return getSource(_cur_id).value("source");
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
    MediaTypes type;
    QString str = getSource(_cur_id).value("type");
    if(str == "web")
        type = WEB;
    else if(str == "audio")
        type = AUDIO;
    else if(str == "video")
        type = VIDEO;
    else if(str == "text")
        type = TEXT;
    else if(str == "image")
        type = IMAGE;
    else if(str == "widget")
        type = WIDGET;
    else if(str == "user_widget")
        type = USER_WIDGET;
    else type = NONE;

    return type;
}

int SourceManager::size() const
{
    int count = 0;
    foreach (int id, sections.keys()) {
        count += sections.value(id).sources.size();
    }

    return count;
}

bool SourceManager::loop() const
{
    return _loop;
}

void SourceManager::setLoop(bool arg)
{
    if(_loop != arg)
    {
        _loop = arg;
        emit loopChanged(_loop);
    }
}

int SourceManager::current() const
{
    return _cur_id;
}

void SourceManager::goNext()
{
    for(int i = _cur_id + 1;  i <= size() || i != _cur_id; i++)
    {
        if(i == size() && _loop)
            i = 0;

        if(!blacklist.contains(i))
        {
            setCurrent(i);
            break;
        }
    }
}

void SourceManager::goPrev()
{
    for(int i = _cur_id - 1; i >= -1 || i != _cur_id; i--)
    {
        if(i < 0 && _loop)
            i = size() - 1;

        if(!blacklist.contains(i))
        {
            setCurrent(i);
            break;
        }
    }
}

void SourceManager::sourceLoaded(int id)
{
    if(blacklist.contains(id))
        blacklist.removeOne(id);
}

void SourceManager::sourceError(int id)
{
    if(!blacklist.contains(id))
        blacklist.append(id);
}

void SourceManager::readXmlSourceList(const QString &name)
{
    QString filepath;
    if(sources_dirs.size())
    {
        foreach (QString cur_dir, sources_dirs)
        {
            QDir dir(cur_dir);
            if(!dir.exists())
                continue;

            dir.setFilter(QDir::Files);

            QStringList filter;
            filter<<QString("%1.xml").arg(name);
            dir.setNameFilters(filter);

            QList<QFileInfo> lst = dir.entryInfoList();
            if(lst.size())
                filepath = lst.value(0).absoluteFilePath();
        }
    }

    QFile *fl = new QFile(filepath);

    if(!fl->open(QFile::ReadOnly | QFile::Text))
    {
        setErrors(SOURCE_NOT_FOUND);
        qDebug()<<"readXmlSourceList:"<<fl->errorString();
        return;
    }

    QXmlStreamReader reader(fl);
    QString cur_element;
    Areas area = ROOT;
    Section section;

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
            {
                area = LIST;
                sections.insert(section.priority,section);
            }
            else if(cur_element == "list" && area == LIST)
                area = ROOT;
        }
        else if(token == QXmlStreamReader::StartElement)
        {
            if(cur_element == "list")
                area = LIST;
            else if(cur_element == "section")
            {
                section.clear();
                area = SECTION;
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
                continue;
            }

            switch(area)
            {
            case LIST:
                continue;
            case SECTION:
                if(cur_element == "source")
                {
                    if(!reader.attributes().isEmpty() || reader.text().isEmpty())
                    {
                        QXmlStreamAttributes attributes = reader.attributes();
                        QXmlStreamAttributes::iterator it = attributes.begin();
                        Source source;

                        source.insert("source",reader.text().toString());
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
                        source.insert("source",reader.readElementText());
                        section.sources.insert(source.value("id").toInt(),source);
                    }
                    else qDebug()<<tr("Could not find a mandatory attribute 'id' within the structure 'source'");
                }
                else if(cur_element == "title")
                    section.title = reader.readElementText();
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

    if(fl->isOpen())
        fl->close();
    fl->deleteLater();

    emit sizeChanged(size());
}

void SourceManager::setCurrent(int arg)
{
    if(_cur_id != arg && arg < size() && arg >= 0)
    {
        _cur_id = arg;
        emit sourceChanget(_cur_id);
    }
}

qreal SourceManager::volume() const
{
    if(_cur_id < size())
        return getSource(_cur_id).value("volume",QString::number(_volume)).toFloat();

    return _volume;
}

int SourceManager::timer() const
{
    if(_cur_id < size())
        return getSource(_cur_id).value("timer",QString::number(_visibilityInterval)).toInt();

    return _visibilityInterval;
}

bool SourceManager::show() const
{
    if(_cur_id < size())
    {
        QString val = getSource(_cur_id).value("video","true").toLower();
        if(val == "true" || val == "1")
            return true;
    }

    return true;
}

QString SourceManager::title() const
{
    if(_cur_id < size())
        return getSource(_cur_id).value("title");

    return QString();
}

int SourceManager::startPosition() const
{
    if(_cur_id < size())
        return getSource(_cur_id).value("start","0").toInt();

    return 0;
}

int SourceManager::stopPosition() const
{
    if(_cur_id < size())
        return getSource(_cur_id).value("stop","0").toInt();

    return 0;
}

bool SourceManager::mute() const
{
    if(_cur_id < size())
    {
        QString val = getSource(_cur_id).value("mute", _mute ? "true" : "false").toLower();
        if(val == "true" || val == "1")
            return true;
    }

    return false;
}

QDateTime SourceManager::startPublicDate() const
{
    if(_cur_id < size())
    {
        QDateTime dt;
        dt.fromString(getSource(_cur_id).value("start_public",QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss")));
        return dt;
    }

    return QDateTime::currentDateTime();
}

QDateTime SourceManager::endPublicDate() const
{
    if(_cur_id < size())
    {
        QDateTime dt;
        dt.fromString(getSource(_cur_id).value("start_public",QDateTime::currentDateTime().addYears(100).toString("dd-MM-yyyy HH:mm:ss")));
        return dt;
    }

    return QDateTime::currentDateTime().addYears(100);
}

bool SourceManager::showSectionTitle() const
{
    if(_cur_id < size())
    {
        int id_section, count;
        id_section = count = 0;

        QList<int> keys = sections.keys();
        qSort(keys.begin(),keys.end());

        foreach (int cur_id, keys) {
            count += sections.value(cur_id).sources.size();
            if(_cur_id < count) {
                id_section = cur_id;
                break;
            }
        }

        QString val = sections.value(id_section).attribs.value("visible","true").toLower();
        if(val == "true" || val == "1")
            return true;
    }

    return false;
}

QString SourceManager::sectionTitle() const
{
    if(_cur_id < size())
        return getSection(_cur_id).title;

    return QString();
}

SourceManager::Statuses SourceManager::status() const
{
    return _status;
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

bool SourceManager::loadXmlConfig(const QString &path)
{
    bool result = false;
    QFile *fl = new QFile(path);
    if(fl->open(QFile::ReadOnly | QFile::Text))
    {
        QXmlStreamReader reader(fl);
        int cur_section = 0;
        QString cur_element;

        while(!reader.atEnd())
        {
            reader.readNext();

            if(reader.isStartDocument())
                continue;

            if(reader.isEndElement())
            {
                if(cur_element == "global")
                    --cur_section;
            }
            else if(reader.isStartElement())
            {
                cur_element = reader.name().toString().toLower();

                switch(cur_section)
                {
                case 0:
                    if(cur_element == "global")
                        cur_section = 1;
                    break;
                case 1:
                    if(cur_element == "sources")
                        sources_dirs.append(reader.readElementText().split(";"));
                    break;
                default:
                    break;
                }
            }
        }

        if(!sources_dirs.isEmpty())
        {
            if(sources_dirs.indexOf("$CONFIG") != -1)
                sources_dirs.replaceInStrings("$CONFIG",QDir::homePath() + "/.config/QtDashboard");
            if(sources_dirs.indexOf("$HOME") != -1)
                sources_dirs.replaceInStrings("$HOME",QDir::homePath());
        }

        if(reader.hasError())
        {
            result = false;
            setErrors(CONFIG_ERROR);
            qDebug()<<tr("Read configuration file error: %").arg(reader.errorString());
        }
        else result = true;
    }
    else qDebug()<<tr("Can't open configuration fil: %1").arg(fl->fileName());

    if(fl->isOpen())
        fl->close();
    fl->deleteLater();

    return result;
}

void SourceManager::setErrors(SourceManager::Errors _e)
{
    if(_error != _e && _e != NO_ERROR)
    {
        _error = _e;
        _status = ERROR;
        emit errorChanget(_error);
        emit statusChanged(_status);
    }
}

const Source SourceManager::getSource(int id) const
{
    int id_section, count;
    id_section = count = 0;
    int id_source = id;

    QList<int> keys = sections.keys();
    qSort(keys.begin(),keys.end());

    foreach (int cur_id, keys) {
        count += sections.value(cur_id).sources.size();
        if(id < count) {
            id_section = cur_id;
            break;
        }
        else id_source = id - count;
    }

    QList<int> source_keys = sections.value(id_section).sources.keys();
    qSort(source_keys.begin(),source_keys.end());

    return sections.value(id_section).sources.value(source_keys.value(id_source));
}

const Section SourceManager::getSection(int id) const
{
    int id_section, count;
    id_section = count = 0;

    QList<int> keys = sections.keys();
    qSort(keys.begin(),keys.end());

    foreach (int cur_id, keys) {
        count += sections.value(cur_id).sources.size();
        if(id < count) {
            id_section = cur_id;
            break;
        }
    }

    Section section = sections.value(id_section);
    return section;
}


