#ifndef TIMELINE_H
#define TIMELINE_H

#include <QObject>
#include <QScopedPointer>

class QTimerEvent;

class TimeLine : public QObject
{
    Q_OBJECT
public:
    explicit TimeLine(unsigned time_resolution, QObject *parent = 0);
    ~TimeLine();

    unsigned int currentTime() const;

signals:
    void timeChanged(unsigned int last_time, unsigned int new_time) const;
    void beatChanged(unsigned current_beat) const;

public slots:
    void setBpm(unsigned int bpm);
    void setCurrentBeat(unsigned int beat);
    void setCurrentTime(unsigned int time);
    void setBeatsInLoop(unsigned int nb_beats);
    void start();
    void stop();

protected:
    void timerEvent(QTimerEvent*);

private:
    struct Internal;
    QScopedPointer<Internal> _pimpl;
};

#endif // TIMELINE_H
