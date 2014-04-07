#-------------------------------------------------
#
# Project created by QtCreator 2014-04-06T00:51:27
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = jj2-server
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += \
    core/application.cpp \
    core/server.cpp \
    console/consolecommands.cpp \
    console/logger.cpp \
    console/main.cpp

HEADERS += \
    core/application.h \
    core/server.h \
    console/consolecommands.h \
    console/logger.h
