#ifndef SORCEMANAGER_H
#define SORCEMANAGER_H

#include <QObject>
#include <QString>
#include <QXmlStreamReader>
#include <QFile>
#include <QDebug>
#include <QDir>
#include <QMap>

#include "Section.h"


class SourceManager : public QObject
{
    Q_OBJECT
    Q_ENUMS(MediaTypes)
    Q_ENUMS(Errors)

public:
    explicit SourceManager(QObject *parent = 0);


    enum MediaTypes {
        NONE,
        TEXT,
        WEB,
        IMAGE,
        AUDIO,
        VIDEO,
        WIDGET,
        USER_WIDGET
    };

    enum Errors {
        NO_ERROR,
        CONFIG_ERROR,
        SOURCE_ERROR,
        SOURCE_NOT_FOUND
    };

    enum Areas {
        ROOT,
        LIST,
        SECTION
    };

    Q_PROPERTY(QString source READ source NOTIFY sourceChanget)
    Q_PROPERTY(int sourceType READ sourceType NOTIFY sourceTypeChanged)
    Q_PROPERTY(int nextSourceType READ nextSourceType NOTIFY nextSourceTypeChanged)
    Q_PROPERTY(int size READ size NOTIFY sizeChanged)
    Q_PROPERTY(Errors error READ error NOTIFY errorChanget)
    Q_PROPERTY(QString errorString READ errorString)

    QString source() const;
    Errors error() const;
    QString errorString() const;

    MediaTypes sourceType() const;
    MediaTypes nextSourceType(int seek = 0) const;
    MediaTypes prevSourceType(int seek = 0) const;
    int size() const;

signals:
    void sourceChanget();
    void nextSourceChanget();
    void volumeChanget();
    void soppedSource();
    void sourceTypeChanged(MediaTypes type);
    void nextSourceTypeChanged(MediaTypes type);
    void sizeChanged();
    void sourceListRead();
    void errorChanget();

public slots:
    void goNext();
    void goPrev();
    void nextLoaded();
    void nextError();
    void readXmlSourceList(const QString &path);

private:
    bool createDefaultConfig(const QString &dir);
    void setErrors(Errors _e);

    int _cur_id;
    bool _audio;
    Errors _error;
    QString _source;
    QString _type;
    QString _config_path;
    int _visibilityInterval;
    qreal _volume;

    QMap<int, Section> sections;

};

#endif // SORCEMANAGER_H
