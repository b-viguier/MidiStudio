#include "recordwidget.h"
#include "ui_recordwidget.h"

#include "sequencer.h"
#include "timeline.h"

namespace {
    struct Internal {
    };
}

RecordWidget::RecordWidget(Sequencer& sequencer) :
    ui(new Ui::RecordWidget),
    _sequencer(sequencer)
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
        _sequencer.timeline().start();
    } else {
        _sequencer.timeline().stop();
    }
}

void RecordWidget::on_record_clicked(bool enabled)
{
    _sequencer.enableRecord(enabled);
}

void RecordWidget::on_metronome_clicked(bool enabled)
{
    _sequencer.enableMetronome(enabled);
}
