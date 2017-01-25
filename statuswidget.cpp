#include "statuswidget.h"
#include "ui_statuswidget.h"
#include <QScrollBar>

#include <RtMidi.h>

namespace {
struct Internal {

    static void fillMidiInterfaces(RtMidi& midi, QComboBox& comboBox) {
        comboBox.clear();
        comboBox.addItem(QObject::tr("None"));
        comboBox.addItem(QObject::tr("Virtual"), QVariant("MidiStudio Virtual Interface"));
        const unsigned int nbPorts = midi.getPortCount();
        for ( unsigned int i=0; i<nbPorts; ++i ) {
            comboBox.addItem(QString::fromStdString(midi.getPortName(i)), QVariant(i));
        }
    }

    static void openMidiInterface(RtMidi& midi, const QVariant& data) {
        midi.closePort();
        switch(data.type()) {
        case QMetaType::Int:
            midi.openPort(data.toInt());
            break;
        case QMetaType::QString:
            midi.openVirtualPort(data.toString().toStdString());
            break;
        default:
            break;
        }
    }
};
}

StatusWidget::StatusWidget(RtMidiIn& midi_in, RtMidiOut& midi_out) :
    ui(new Ui::StatusWidget),
    _midi_in(midi_in),
    _midi_out(midi_out)
{
    ui->setupUi(this);

    _midi_in.setErrorCallback(StatusWidget::onMidiError, this);
    _midi_out.setErrorCallback(StatusWidget::onMidiError, this);
    refresh();
}

StatusWidget::~StatusWidget()
{
    delete ui;
}

void StatusWidget::onMidiError(RtMidiError::Type, const std::string &errorText, void* userData)
{
    StatusWidget* self = reinterpret_cast<StatusWidget*>(userData);
    self->ui->logs->appendPlainText(QString::fromStdString(errorText));
    self->ui->logs->verticalScrollBar()->setValue(self->ui->logs->verticalScrollBar()->maximum());
}

void StatusWidget::refresh()
{
    ui->midi_out->clear();
    ui->midi_out->clear();

    Internal::fillMidiInterfaces(_midi_in, *ui->midi_in);
    Internal::fillMidiInterfaces(_midi_out, *ui->midi_out);
}

void StatusWidget::on_midi_in_currentIndexChanged(int)
{
    Internal::openMidiInterface(_midi_in, ui->midi_in->currentData());
}

void StatusWidget::on_midi_out_currentIndexChanged(int)
{
    Internal::openMidiInterface(_midi_out, ui->midi_out->currentData());
}
