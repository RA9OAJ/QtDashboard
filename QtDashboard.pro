TEMPLATE = app

QT += qml quick widgets sql av

SOURCES += main.cpp \
    sourcemanager.cpp

RESOURCES += qml.qrc

TARGET = qdashboard

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    sourcemanager.h \
    Section.h
