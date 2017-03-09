#ifndef METRONOMETRACK_H
#define METRONOMETRACK_H

#include <QObject>

#include "abstractplayabletrack.h"

class MetronomeTrack : public QObject, public AbstractPlayableTrack
{
    Q_OBJECT
public:
    explicit MetronomeTrack(QObject *parent = 0);

public slots:
    void play(unsigned int from_time, unsigned int to_time) const;
    void setBpm(unsigned int bpm);
    void setNumberOfBeats(unsigned int number_of_beats);

signals:
    void messagePlayed(const MidiMessage& message) const;

private:
    unsigned int _bpm;
    unsigned int _nb_beats;
};

#endif // METRONOMETRACK_H
