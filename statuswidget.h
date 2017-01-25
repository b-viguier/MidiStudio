#ifndef STATUSWIDGET_H
#define STATUSWIDGET_H

#include <QDockWidget>

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

public slots:
    void refresh();

private:
    Ui::StatusWidget *ui;

    RtMidiIn& _midi_in;
    RtMidiOut& _midi_out;
};

#endif // STATUSWIDGET_H
