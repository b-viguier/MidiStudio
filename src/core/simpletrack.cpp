#include "simpletrack.h"

#include <QMultiMap>

struct SimpleTrack::Internal {
    typedef QMultiMap<unsigned int, MidiMessage> Track;
    Track track;
};

SimpleTrack::SimpleTrack(QObject *parent) :
    QObject(parent),
    _pimpl(new Internal)
{

}

SimpleTrack::SimpleTrack(SimpleTrack &&other) :
    QObject(other.parent()),
    _pimpl(other._pimpl.take())
{

}

SimpleTrack::~SimpleTrack()
{

}

SimpleTrack &SimpleTrack::operator=(SimpleTrack &&other)
{
    setParent(other.parent());
    _pimpl.reset(other._pimpl.take());
    return *this;
}

void SimpleTrack::addMidiSequence(const MidiSequence &sequence)
{
    for(MidiSequence::const_iterator it = sequence.constBegin(), it_end = sequence.constEnd();
        it != it_end;
        ++it) {
        _pimpl->track.insert(it->millis, it->data);
    }
}

void SimpleTrack::clear()
{
    _pimpl->track.clear();
}

void SimpleTrack::play(unsigned int from_time, unsigned int to_time) const
{
    Internal::Track::const_iterator
            it = _pimpl->track.upperBound(from_time),
            it_end = _pimpl->track.upperBound(to_time);
    for(;it != it_end; ++it) {
        emit messagePlayed(it.value());
    }
}
