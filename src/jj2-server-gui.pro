#-------------------------------------------------
#
# Project created by QtCreator 2014-04-07T15:00:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gui
TEMPLATE = app


SOURCES += gui/main.cpp\
        gui/widget.cpp \
    core/application.cpp \
    core/server.cpp

HEADERS  += gui/widget.h \
    core/application.h \
    core/server.h

FORMS    += gui/widget.ui
