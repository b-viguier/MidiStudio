#ifndef STATUSWIDGET_H
#define STATUSWIDGET_H

#include <QDockWidget>
#include "RtMidi.h"

namespace Ui {
class StatusWidget;
}

class RtMidiIn;
class RtMidiOut;

class StatusWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit StatusWidget(RtMidiIn& midi_in, RtMidiOut& midi_out);
    ~StatusWidget();

    static void onMidiError(RtMidiError::Type type, const std::string &errorText, void* userData);

public slots:
    void refresh();
    void on_midi_in_currentIndexChanged(int);
    void on_midi_out_currentIndexChanged(int);

private:
    Ui::StatusWidget *ui;

    RtMidiIn& _midi_in;
    RtMidiOut& _midi_out;
};

#endif // STATUSWIDGET_H
