#ifndef SEQUENCER_H
#define SEQUENCER_H

#include <QObject>
#include <QScopedPointer>

#include "midimessage.h"

class QMidi;
class TimeLine;

class Sequencer : public QObject
{
    Q_OBJECT
public:
    explicit Sequencer(QMidi& midi, TimeLine& timeline, QObject *parent = 0);
    ~Sequencer();

    TimeLine& timeline() const;

public slots:
    void enableRecord(bool enable);
    void enableMetronome(bool enable);
    void setBeatsPerMeasure(unsigned int beats_per_measure);

    void play(unsigned int from_time, unsigned int to_time) const;
    void playMetronome(unsigned int beat);

private slots:
    void onMidiEvent(unsigned int, const MidiMessage&);

private:
    struct Internal;
    QScopedPointer<Internal> _pimpl;
};

#endif // SEQUENCER_H
