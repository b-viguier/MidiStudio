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
    statuswidget.cpp \
    recordwidget.cpp

HEADERS  += mainwindow.h \
    statuswidget.h \
    recordwidget.h

FORMS    += mainwindow.ui \
    statuswidget.ui \
    recordwidget.ui

# RtMidi
RTMIDI_PATH = libs/rtmidi-2.1.1
INCLUDEPATH += $$RTMIDI_PATH
SOURCES += $$RTMIDI_PATH/RtMidi.cpp
HEADERS += $$RTMIDI_PATH/RtMidi.h

macx {
    DEFINES += __MACOSX_CORE__
    LIBS += -framework CoreMidi -framework CoreAudio -framework CoreFoundation
}
