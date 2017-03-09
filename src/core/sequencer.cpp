#include "sequencer.h"

#include <QTime>

#include "qmidi.h"
#include "timeline.h"
#include "midisequence.h"
#include "simpletrack.h"

namespace {
enum MidiValue : unsigned char {
    NOTE_ON = 0b10010000,
    NOTE_OFF = 0b10000000,

    BEAT_NOTE = 0x25,
    CHANNEL = 9,
    LOW_VELOCITY = 60,
    HIGH_VELOCITY = 100,
};
}

struct Sequencer::Internal {

    Internal(QMidi& midi, TimeLine& timeline) :
        midi(midi),
        timeline(timeline)
    {}

    QMidi& midi;
    TimeLine& timeline;
    unsigned int beats_per_measure = 0;

    MidiSequence record_sequence;
    std::vector<SimpleTrack> tracks;
};

Sequencer::Sequencer(QMidi& midi, TimeLine& timeline, QObject *parent) :
    QObject(parent),
    _pimpl(new Internal(midi, timeline))
{

}

Sequencer::~Sequencer()
{

}

TimeLine &Sequencer::timeline() const
{
    return _pimpl->timeline;
}

void Sequencer::enableRecord(bool enabled)
{
    if(enabled) {
        _pimpl->record_sequence.clear();
        connect(&_pimpl->midi,&QMidi::messageReceived, this, &Sequencer::onMidiEvent);
    } else {
        disconnect(&_pimpl->midi,&QMidi::messageReceived, this, &Sequencer::onMidiEvent);
        if(!_pimpl->record_sequence.empty()) {
            SimpleTrack track;
            track.addMidiSequence(_pimpl->record_sequence);
            _pimpl->tracks.push_back(std::move(track));
        }
    }
}

void Sequencer::enableMetronome(bool enable)
{
    if(enable) {
        connect(&_pimpl->timeline, &TimeLine::beatChanged, this, &Sequencer::playMetronome);
    } else {
        disconnect(&_pimpl->timeline, &TimeLine::beatChanged, this, &Sequencer::playMetronome);
    }
}

void Sequencer::setBeatsPerMeasure(unsigned int beats_per_measure)
{
    _pimpl->beats_per_measure = beats_per_measure;
}

void Sequencer::play(unsigned int from_time, unsigned int to_time) const
{
    for(unsigned int i=0,size=_pimpl->tracks.size(); i<size; ++i) {
        _pimpl->tracks[i].play(from_time, to_time);
    }
}

void Sequencer::playMetronome(unsigned int beat)
{
    const MidiMessage beat_message = {
        MidiValue::NOTE_ON + MidiValue::CHANNEL,
        MidiValue::BEAT_NOTE,
        (beat % _pimpl->beats_per_measure) == 0 ? MidiValue::HIGH_VELOCITY : MidiValue::LOW_VELOCITY
    };

    _pimpl->midi.sendMessage(beat_message);
}

void Sequencer::onMidiEvent(unsigned int, const MidiMessage &message)
{
    _pimpl->record_sequence.push_back({_pimpl->timeline.currentTime(), message});
}
