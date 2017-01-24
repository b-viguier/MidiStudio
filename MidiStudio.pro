#-------------------------------------------------
#
# Project created by QtCreator 2017-01-24T13:18:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MidiStudio
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    statuswidget.cpp

HEADERS  += mainwindow.h \
    statuswidget.h

FORMS    += mainwindow.ui \
    statuswidget.ui
