TEMPLATE = app

QT += qml quick widgets sql av
QMAKE_EXTRA_TARGETS += mytemplate

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

#Default template
mytemplate.target = DefaultTemplate
mytemplate.commands = mkdir -p $$DESTDIR/../share/QtDashboard/templates/DefaultTemplate; cp $$PWD/qml/main.qml $$DESTDIR/../share/QtDashboard/templates/DefaultTemplate/main.qml

POST_TARGETDEPS += DefaultTemplate
