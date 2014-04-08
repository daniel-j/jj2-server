#-------------------------------------------------
#
# Project created by QtCreator 2014-04-07T15:00:34
#
#-------------------------------------------------

QT       += core network gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = jj2-server-gui
TEMPLATE = app


SOURCES += gui/main.cpp\
    core/application.cpp \
    core/server.cpp \
    gui/mainwindow.cpp \
    core/logger.cpp \
    core/listserver.cpp

HEADERS  += \
    core/application.h \
    core/server.h \
    gui/mainwindow.h \
    core/logger.h \
    core/listserver.h

FORMS    += \
    gui/mainwindow.ui

CONFIG(debug, debug|release) {
	DESTDIR = ../build-$${TARGET}-debug
} else {
	DESTDIR = ../build-$${TARGET}-release
}

OBJECTS_DIR = $${DESTDIR}/.obj
MOC_DIR = $${DESTDIR}/.moc
RCC_DIR = $${DESTDIR}/.rcc
UI_DIR = $${DESTDIR}/.ui
