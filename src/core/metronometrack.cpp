#include "metronometrack.h"

namespace {
    struct Internal {

        static unsigned int timeToBeat(unsigned int time, unsigned int bpm) {
            enum {
                MILLIS_PER_MINUTE = 60000,
            };
            return (time * bpm) / MILLIS_PER_MINUTE ;
        }
    };

    enum MidiValue : unsigned char {
        NOTE_ON = 0b10010000,
        NOTE_OFF = 0b10000000,

        BEAT_NOTE = 0x25,
        CHANNEL = 9,
        LOW_VELOCITY = 60,
        HIGH_VELOCITY = 100,
    };
}

MetronomeTrack::MetronomeTrack(QObject *parent) :
    QObject(parent),
    _bpm(120),
    _nb_beats(4)
{

}

void MetronomeTrack::play(unsigned int from_time, unsigned int to_time) const
{
    const unsigned int
            from_beat = Internal::timeToBeat(from_time, _bpm),
            to_beat = Internal::timeToBeat(to_time, _bpm);

    if(from_beat == to_beat) {
        return;
    }

    const MidiMessage beat = {
        MidiValue::NOTE_ON + MidiValue::CHANNEL,
        MidiValue::BEAT_NOTE,
        (to_beat % _nb_beats) == 0 ? MidiValue::HIGH_VELOCITY : MidiValue::LOW_VELOCITY
    };

    emit messagePlayed(beat);
}

void MetronomeTrack::setBpm(unsigned int bpm)
{
    _bpm = bpm;
}

void MetronomeTrack::setNumberOfBeats(unsigned int number_of_beats)
{
    _nb_beats = number_of_beats;
}
