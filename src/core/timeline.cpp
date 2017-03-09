#include "timeline.h"

#include <QTime>
#include <QTimerEvent>

#include <limits>

struct TimeLine::Internal {
    unsigned int start_time = 0;
    unsigned int end_time = UINT_MAX;
    unsigned int current_time = 0;
    unsigned int bpm = 0;
    int timer_id = 0;
    unsigned int time_resolution = 0;
    QTime timer;

    enum : unsigned int {
        MILLIS_PER_MINUTE = 60000,
    };

    unsigned int beatToTime(unsigned int beat)
    {
        return (beat * MILLIS_PER_MINUTE) / bpm;
    }

    unsigned int timeToBeat(unsigned int time)
    {
        return (time * bpm) / MILLIS_PER_MINUTE;
    }
};

TimeLine::TimeLine(unsigned time_resolution, QObject *parent) :
    QObject(parent),
    _pimpl(new Internal)
{
    _pimpl->time_resolution = time_resolution;
}

TimeLine::~TimeLine()
{

}

unsigned int TimeLine::currentTime() const
{
    return _pimpl->start_time + _pimpl->timer.elapsed();
}

void TimeLine::setBpm(unsigned int bpm)
{
    _pimpl->bpm = bpm;
}

void TimeLine::setCurrentBeat(unsigned int current_beat)
{
    setCurrentTime(_pimpl->beatToTime(current_beat));
}

void TimeLine::setCurrentTime(unsigned int time)
{
    _pimpl->start_time = _pimpl->current_time = time;
    _pimpl->timer.restart();
}

void TimeLine::setBeatsInLoop(unsigned int nb_beats)
{
    _pimpl->end_time = _pimpl->beatToTime(nb_beats);
}

void TimeLine::start()
{
    if (_pimpl->timer_id) {
        qWarning("QTimeLine::start: already running");
        return;
    }
    _pimpl->timer_id = startTimer(_pimpl->time_resolution);
    _pimpl->start_time = 0;
    _pimpl->timer.start();
}

void TimeLine::stop()
{
    if (_pimpl->timer_id) {
        killTimer(_pimpl->timer_id);
    }
    _pimpl->timer_id = 0;
}

void TimeLine::timerEvent(QTimerEvent *event)
{
    if(event->timerId() != _pimpl->timer_id) {
        event->ignore();
        return;
    }
    event->accept();

    const unsigned int last_time = _pimpl->current_time;
    _pimpl->current_time = (_pimpl->start_time + _pimpl->timer.elapsed()) % _pimpl->end_time;

    if(_pimpl->current_time >= last_time) {
        emit timeChanged(last_time, _pimpl->current_time);
    } else {
        emit timeChanged(last_time, _pimpl->end_time);
        emit timeChanged(0, _pimpl->current_time);
    }

    const unsigned int
        last_beat = _pimpl->timeToBeat(last_time),
        current_beat = _pimpl->timeToBeat(_pimpl->current_time);
    if( last_beat != current_beat ) {
        emit beatChanged(current_beat);
    }
}
