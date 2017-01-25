#include "statuswidget.h"
#include "ui_statuswidget.h"

#include <RtMidi.h>

StatusWidget::StatusWidget(RtMidiIn& midi_in, RtMidiOut& midi_out) :
    ui(new Ui::StatusWidget),
    _midi_in(midi_in),
    _midi_out(midi_out)
{
    ui->setupUi(this);

    refresh();
}

StatusWidget::~StatusWidget()
{
    delete ui;
}

void StatusWidget::refresh()
{
    ui->midi_out->clear();
    ui->midi_out->clear();

    struct Local {
        static void synchronise(RtMidi& midi, QComboBox& comboBox) {
            comboBox.clear();
            const unsigned int nbPorts = midi.getPortCount();
            comboBox.setEnabled(nbPorts > 0);
            for ( unsigned int i=0; i<nbPorts; ++i ) {
                comboBox.addItem(QString::fromStdString(midi.getPortName(i)));
            }
        }
    };

    Local::synchronise(_midi_in, *ui->midi_in);
    Local::synchronise(_midi_out, *ui->midi_out);
}
