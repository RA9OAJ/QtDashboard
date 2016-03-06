#ifndef SECTION_H
#define SECTION_H

#include <QVariant>
#include <QMap>

typedef QMap<QString, QString> Source;

struct Section {
    int priority;
    QString title;
    QMap<QString, QString> attribs;

    QMap<int,Source> sources;

    Section() {clear();}

    void clear()
    {
        priority = 5;
        title.clear();
        attribs.clear();
        sources.clear();
    }
};

#endif // SECTION_H
