#ifndef RECORDWIDGET_H
#define RECORDWIDGET_H

#include <QDockWidget>
#include <QVector>
#include <QTime>

#include "RtMidi.h"

namespace Ui {
class RecordWidget;
}

struct MidiMessage {
    int millis;
    std::vector<unsigned char> data;
};
typedef QVector<MidiMessage> Track;

class RecordWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit RecordWidget(RtMidiIn& midi_in, RtMidiOut& midi_out);
    ~RecordWidget();

public slots:
    void on_play_clicked(bool);
    void on_record_clicked(bool);

protected:
      void timerEvent(QTimerEvent *event);
      static void onMidiEvent( double deltatime, std::vector< unsigned char > *message, void *userData );

private:
    Ui::RecordWidget *ui;

    RtMidiIn& _midi_in;
    RtMidiOut& _midi_out;
    QTime _time;
    Track _track;
    Track::Iterator _cursor;
    int _timer_id;
};

#endif // RECORDWIDGET_H
