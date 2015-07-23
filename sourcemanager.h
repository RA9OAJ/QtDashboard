#ifndef SORCEMANAGER_H
#define SORCEMANAGER_H

#include <QObject>
#include <QString>

class SourceManager : public QObject
{
    Q_OBJECT
    Q_ENUMS(MediaTypes)

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

    Q_PROPERTY(QString source READ source NOTIFY sourceChanget)
    Q_PROPERTY(int sourceType READ sourceType NOTIFY sourceTypeChanged)
    Q_PROPERTY(int nextSourceType READ nextSourceType NOTIFY nextSourceTypeChanged)
    Q_PROPERTY(int size READ size NOTIFY sizeChanged)

    QString source() const;

    MediaTypes sourceType() const;
    MediaTypes nextSourceType() const;
    MediaTypes prevSourceType() const;
    int size() const;

signals:
    void sourceChanget();
    void nextSourceChanget();
    void volumeChanget();
    void soppedSource();
    void sourceTypeChanged(MediaTypes type);
    void nextSourceTypeChanged(MediaTypes type);
    void sizeChanged();

public slots:
    void goNext();
    void goPrev();
    void nextLoaded();
    void nextError();

private:
    int _cur_id;
    bool _audio;
    QString _source;
    QString _type;
    int _visibilityInterval;
    qreal _volume;

};

#endif // SORCEMANAGER_H
