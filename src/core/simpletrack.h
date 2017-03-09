#ifndef SIMPLETRACK_H
#define SIMPLETRACK_H

#include <QObject>
#include <QScopedPointer>

#include "abstractplayabletrack.h"
#include "midisequence.h"

class SimpleTrack : public QObject, public AbstractPlayableTrack
{
    Q_OBJECT
public:
    explicit SimpleTrack(QObject *parent = 0);
    SimpleTrack(const SimpleTrack& other) = delete;
    SimpleTrack(SimpleTrack&& other);
    ~SimpleTrack();
    SimpleTrack& operator=(SimpleTrack& other) = delete;
    SimpleTrack& operator=(SimpleTrack&& other);

    void addMidiSequence(const MidiSequence& sequence);
    void clear();

public slots:
    void play(unsigned int from_time, unsigned int to_time) const;

signals:
    void messagePlayed(const MidiMessage& message) const;

private:
    struct Internal;
    QScopedPointer<Internal> _pimpl;
};

#endif // SIMPLETRACK_H
