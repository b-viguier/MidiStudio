#-------------------------------------------------
#
# Project created by QtCreator 2017-01-24T13:18:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MidiStudio
TEMPLATE = app


SOURCES += src/main.cpp

include( src/forms/forms.pri )

include( libs/rtmidi-2.1.1/rtmidi.pri )
