#ifndef MIDISEQUENCE_H
#define MIDISEQUENCE_H

#include <QVector>
#include "midimessage.h"

struct MidiTimeMessage {
    unsigned int millis;
    MidiMessage data;
};

typedef QVector<MidiTimeMessage> MidiSequence;

#endif // MIDISEQUENCE_H
