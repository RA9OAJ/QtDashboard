#ifndef SORCEMANAGER_H
#define SORCEMANAGER_H

#include <QObject>
#include <QString>

class SourceManager : public QObject
{
    Q_OBJECT
public:
    explicit SourceManager(QObject *parent = 0);

    Q_PROPERTY(QString source READ source NOTIFY sourceChanget)

    QString source() const;

signals:
    void sourceChanget();
    void nextSourceChanget();
    void volumeChanget();
    void soppedSource();

public slots:
    /*void goNext();
    void goPrev();
    void nextLoaded();
    void nextError();*/

private:
    int _cur_id;
    bool _audio;
    QString _source;
    QString _type;
    int _visibilityInterval;
    qreal _volume;

};

#endif // SORCEMANAGER_H
