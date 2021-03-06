QT += core network
QT -= gui

TARGET = qtdnetservice
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

DESTDIR = ./usr/bin

MOC_DIR        = build/moc_dir
OBJECTS_DIR    = build/obj_dir
UI_HEADERS_DIR = build/ui/h
UI_SOURCES_DIR = build/ui/cpp
RCC_DIR        = build/res

SOURCES += main.cpp \
            ../QtDashboard/mycoreapplication.cpp \
            ../QtDashboard/processsharedbuffer.cpp \
    tcpservice.cpp \
    servicecore.cpp \
    servicelog.cpp

HEADERS += ../QtDashboard/mycoreapplication.h \
                ../QtDashboard/processsharedbuffer.h \
    tcpservice.h \
    servicecore.h \
    servicelog.h

