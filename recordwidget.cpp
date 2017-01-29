#include "recordwidget.h"
#include "ui_recordwidget.h"

namespace {
    struct Internal {
    };
}

RecordWidget::RecordWidget(RtMidiIn& midi_in, RtMidiOut& midi_out) :
    ui(new Ui::RecordWidget),
    _midi_in(midi_in),
    _midi_out(midi_out)
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
        _midi_in.setCallback(&RecordWidget::onMidiEvent, this);
        _time.start();
    } else {
        _midi_in.cancelCallback();
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

    std::vector<unsigned char> note = {
        MidiValue::NOTE_ON + MidiValue::CHANNEL,
        MidiValue::BEAT_NOTE,
        MidiValue::VELOCITY
    };
    if(_current_beat == 0) {
        note[2] += 40;
    }
    _midi_out.sendMessage(&note);
    _current_beat = (_current_beat + 1) % ui->beats->value();
    note[0] = MidiValue::NOTE_OFF + MidiValue::CHANNEL;
    _midi_out.sendMessage(&note);
}

void RecordWidget::on_bpm_valueChanged(int)
{
    _bpm_timer.setInterval(60000 / ui->bpm->value());
}

void RecordWidget::timerEvent(QTimerEvent*)
{
    int current_millis = _time.elapsed();
    for(;_cursor != _track.end() && _cursor->millis <= current_millis; ++_cursor) {
        _midi_out.sendMessage(&_cursor->data);
    }
}

void RecordWidget::onMidiEvent(double , std::vector<unsigned char> *message, void *userData)
{
    RecordWidget* self = reinterpret_cast<RecordWidget*>(userData);
    MidiMessage msg = {
        self->_time.elapsed(),
        *message,
    };
    self->_track.append(msg);
}
