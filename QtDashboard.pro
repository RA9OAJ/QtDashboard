TEMPLATE = app

QT += qml quick widgets sql av

SOURCES += main.cpp \
    sourcemanager.cpp \
    processsharedbuffer.cpp \
    myapplication.cpp \
    mycoreapplication.cpp

RESOURCES += qml.qrc

TARGET = qdashboard

DESTDIR = ./usr/bin

MOC_DIR        = build/moc_dir
OBJECTS_DIR    = build/obj_dir
UI_HEADERS_DIR = build/ui/h
UI_SOURCES_DIR = build/ui/cpp
RCC_DIR        = build/res

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    sourcemanager.h \
    Section.h \
    processsharedbuffer.h \
    myapplication.h \
    mycoreapplication.h
