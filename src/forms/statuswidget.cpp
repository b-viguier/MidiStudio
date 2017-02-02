#include "statuswidget.h"
#include "ui_statuswidget.h"
#include <QScrollBar>

#include "qmidi.h"

namespace {
struct Internal {

    static void fillMidiInterfaces(const QStringList& items, QComboBox& comboBox) {
        comboBox.clear();
        comboBox.addItem(QObject::tr("None"));
        comboBox.addItems(items);
    }
};
}

StatusWidget::StatusWidget(QMidi &midi) :
    ui(new Ui::StatusWidget),
    _midi(midi)
{
    ui->setupUi(this);

    connect(&_midi, &QMidi::errorOccurred, this, &StatusWidget::onMidiError);
    refresh();
}

StatusWidget::~StatusWidget()
{
    delete ui;
}

void StatusWidget::onMidiError(const QString& message)
{
    ui->logs->appendPlainText(message);
    ui->logs->verticalScrollBar()->setValue(ui->logs->verticalScrollBar()->maximum());
}

void StatusWidget::refresh()
{
    Internal::fillMidiInterfaces(_midi.availableInterfaces<QMidi::IN>(), *ui->midi_in);
    Internal::fillMidiInterfaces(_midi.availableInterfaces<QMidi::OUT>(), *ui->midi_out);
}

void StatusWidget::on_midi_in_currentTextChanged(const QString& text)
{
    if(!_midi.selectInterface<QMidi::IN>(text)) {
        ui->midi_in->setCurrentIndex(0);
    }
}

void StatusWidget::on_midi_out_currentTextChanged(const QString& text)
{
    if(!_midi.selectInterface<QMidi::OUT>(text)) {
        ui->midi_out->setCurrentIndex(0);
    }
}
