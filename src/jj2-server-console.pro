#-------------------------------------------------
#
# Project created by QtCreator 2014-04-06T00:51:27
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = jj2-server-console
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += \
    core/application.cpp \
    core/server.cpp \
    console/main.cpp \
    core/logger.cpp \
    console/consolewrapper.cpp

HEADERS += \
    core/application.h \
    core/server.h \
    core/logger.h \
    console/consolewrapper.h
