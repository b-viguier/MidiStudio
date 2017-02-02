#ifndef RECORDWIDGET_H
#define RECORDWIDGET_H

#include <QDockWidget>
#include <QVector>
#include <QTime>
#include <QTimer>

#include "midimessage.h"
#include "midisequence.h"

class QMidi;

namespace Ui {
class RecordWidget;
}

class RecordWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit RecordWidget(QMidi& midi);
    ~RecordWidget();

private slots:
    void on_play_clicked(bool);
    void on_record_clicked(bool);
    void on_metronome_clicked(bool);
    void onBpmTimeout();
    void on_bpm_valueChanged(int = 0);
    void onMidiEvent( unsigned int, const MidiMessage&);

protected:
      void timerEvent(QTimerEvent *event);

private:
    Ui::RecordWidget *ui;

    QMidi& _midi;
    QTime _time;
    MidiSequence _track;
    MidiSequence::Iterator _cursor;
    int _timer_id;
    QTimer _bpm_timer;
    quint8 _current_beat;
};

#endif // RECORDWIDGET_H
