#ifndef ABSTRACTPLAYABLETRACK_H
#define ABSTRACTPLAYABLETRACK_H

#include <QObject>

#include "midimessage.h"

class AbstractPlayableTrack {

public slots:
    virtual void play(unsigned int from_time, unsigned int to_time) const = 0;

signals:
    virtual void messagePlayed(const MidiMessage& message) const = 0;

protected:
    ~AbstractPlayableTrack() {}
};

#endif // ABSTRACTPLAYABLETRACK_H
