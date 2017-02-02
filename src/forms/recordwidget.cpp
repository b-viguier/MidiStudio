#include "recordwidget.h"
#include "ui_recordwidget.h"

#include "qmidi.h"
#include <QDebug>

namespace {
    struct Internal {
    };
}

RecordWidget::RecordWidget(QMidi& midi) :
    ui(new Ui::RecordWidget),
    _midi(midi)
{
    ui->setupUi(this);

    connect(&_bpm_timer,&QTimer::timeout, this, &RecordWidget::onBpmTimeout);
    on_bpm_valueChanged();
}

RecordWidget::~RecordWidget()
{
    delete ui;
}

void RecordWidget::on_play_clicked(bool enabled)
{
    if(enabled) {
        _cursor = _track.begin();
        _timer_id = startTimer(10/*ms*/);
        _time.start();
    } else {
        killTimer(_timer_id);
    }
}

void RecordWidget::on_record_clicked(bool enabled)
{
    if(enabled) {
        _track.clear();
        connect(&_midi,&QMidi::messageReceived, this, &RecordWidget::onMidiEvent);
        _midi.resetTime();
    } else {
        disconnect(&_midi,&QMidi::messageReceived, this, &RecordWidget::onMidiEvent);
    }
}

void RecordWidget::on_metronome_clicked(bool enabled)
{
    if(enabled) {
        _current_beat = 0;
        onBpmTimeout();
        _bpm_timer.start();
    } else {
        _bpm_timer.stop();
    }
}

void RecordWidget::onBpmTimeout()
{
    enum MidiValue {
        NOTE_ON = 0b10010000,
        NOTE_OFF = 0b10000000,

        BEAT_NOTE = 0x25,
        CHANNEL = 9,
        VELOCITY = 60,
    };

    MidiMessage note = {
        MidiValue::NOTE_ON + MidiValue::CHANNEL,
        MidiValue::BEAT_NOTE,
        MidiValue::VELOCITY
    };
    if(_current_beat == 0) {
        note[2] += 40;
    }
    _midi.sendMessage(note);
    _current_beat = (_current_beat + 1) % ui->beats->value();
    note[0] = MidiValue::NOTE_OFF + MidiValue::CHANNEL;
    _midi.sendMessage(note);
}

void RecordWidget::on_bpm_valueChanged(int)
{
    _bpm_timer.setInterval(60000 / ui->bpm->value());
}

void RecordWidget::timerEvent(QTimerEvent*)
{
    unsigned int current_millis = _time.elapsed();
    for(;_cursor != _track.end() && _cursor->millis <= current_millis; ++_cursor) {
        _midi.sendMessage(_cursor->data);
    }
}

void RecordWidget::onMidiEvent(unsigned int millis, const MidiMessage& message)
{
    MidiTimeMessage msg = {
        millis,
        message,
    };
    _track.append(msg);
}
