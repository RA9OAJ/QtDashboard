#ifndef SECTION_H
#define SECTION_H

#include <QVariant>
#include <QMap>

typedef QMap<QString, QString> Source;

struct Section {
    int id;
    int priority;
    QString title;
    QMap<QString, QString> attribs;

    QMap<int,Source> sources;
};

#endif // SECTION_H
