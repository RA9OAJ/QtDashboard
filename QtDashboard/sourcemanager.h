#ifndef SORCEMANAGER_H
#define SORCEMANAGER_H

#include <QObject>
#include <QString>
#include <QXmlStreamReader>
#include <QDateTime>
#include <QFile>
#include <QDebug>
#include <QDir>
#include <QMap>
#include <QList>

#include "Section.h"


class SourceManager : public QObject
{
    Q_OBJECT
    Q_ENUMS(MediaTypes)
    Q_ENUMS(Errors)
    Q_ENUMS(Statuses)

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

    enum Statuses {
        LOADING,
        READY,
        ERROR
    };

    enum Areas {
        ROOT,
        LIST,
        SECTION
    };

    Q_PROPERTY(int current READ current WRITE setCurrent NOTIFY sourceChanget)
    Q_PROPERTY(QString source READ source NOTIFY sourceChanget)
    Q_PROPERTY(int sourceType READ sourceType)
    Q_PROPERTY(int size READ size NOTIFY sizeChanged)
    Q_PROPERTY(Errors error READ error NOTIFY errorChanget)
    Q_PROPERTY(QString errorString READ errorString)
    Q_PROPERTY(bool loop READ loop WRITE setLoop NOTIFY loopChanged)
    Q_PROPERTY(Statuses status READ status NOTIFY statusChanged)

    Q_PROPERTY(qreal volume READ volume NOTIFY volumeChanged)
    Q_PROPERTY(int  timer READ timer)
    Q_PROPERTY(bool show READ show)
    Q_PROPERTY(QString title READ title)
    Q_PROPERTY(int startPosition READ startPosition)
    Q_PROPERTY(int stopPosition READ stopPosition)
    Q_PROPERTY(bool mute READ mute)
    Q_PROPERTY(QDateTime startPublicDate READ startPublicDate)
    Q_PROPERTY(QDateTime endPublicDate READ endPublicDate)
    Q_PROPERTY(bool showSectionTitle READ showSectionTitle)
    Q_PROPERTY(QString sectionTitle READ sectionTitle)
    Q_PROPERTY(QString widget_name READ widget_name)

    QString source() const;
    Errors error() const;
    QString errorString() const;

    MediaTypes sourceType() const;
    int size() const;
    bool loop() const;
    void setLoop(bool arg);
    int current() const;
    void setCurrent(int arg);

    qreal volume() const;
    int timer() const;
    bool show() const;
    QString title() const;
    int startPosition() const;
    int stopPosition() const;
    bool mute() const;
    QDateTime startPublicDate() const;
    QDateTime endPublicDate() const;
    bool showSectionTitle() const;
    QString sectionTitle() const;
    Statuses status() const;
    QString widget_name() const;

signals:
    void sourceChanget(int arg);
    void volumeChanged(qreal arg);
    void stoppedSource();
    void sizeChanged(int arg);
    void sourceListRead();
    void errorChanget(int arg);
    void loopChanged(bool arg);
    void statusChanged(Statuses arg);

public slots:
    void goNext();
    void goPrev();
    void sourceLoaded(int id);
    void sourceError(int id);
    void readXmlSourceList(const QString &name);

private:
    bool createDefaultConfig(const QString &dir);
    bool loadXmlConfig(const QString &path);
    void setErrors(Errors _e);
    const Source getSource(int id) const;
    const Section getSection(int id) const;

    Statuses _status;
    int _cur_id;
    bool _loop;
    Errors _error;
    QString _source;
    QString _type;
    QString _config_path;
    int _visibilityInterval;
    bool _mute;
    qreal _volume;
    QStringList sources_dirs;

    QMultiMap<int, Section> sections;
    QList<int> blacklist;

};

#endif // SORCEMANAGER_H
