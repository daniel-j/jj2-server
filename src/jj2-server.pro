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


SOURCES += main.cpp \
    application.cpp \
    server.cpp \
    consolecommands.cpp \
    logger.cpp

HEADERS += \
    application.h \
    server.h \
    consolecommands.h \
    logger.h
