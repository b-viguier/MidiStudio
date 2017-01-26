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
        _midi_in.setCallback(RecordWidget::onMidiEvent, this);
        _time.start();
    } else {
        _midi_in.cancelCallback();
    }
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
