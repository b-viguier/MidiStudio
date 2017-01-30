INCLUDEPATH += $$PWD
SOURCES += $$PWD/RtMidi.cpp
HEADERS += $$PWD/RtMidi.h

macx {
    DEFINES += __MACOSX_CORE__
    LIBS += -framework CoreMidi -framework CoreAudio -framework CoreFoundation
}
