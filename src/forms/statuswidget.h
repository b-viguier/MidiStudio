#ifndef STATUSWIDGET_H
#define STATUSWIDGET_H

#include <QDockWidget>

class QMidi;

namespace Ui {
class StatusWidget;
}

class StatusWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit StatusWidget(QMidi& midi);
    ~StatusWidget();

public slots:
    void refresh();
    void on_midi_in_currentTextChanged(const QString&);
    void on_midi_out_currentTextChanged(const QString&);

private slots:
    void onMidiError(const QString&);

private:
    Ui::StatusWidget *ui;

    QMidi& _midi;
};

#endif // STATUSWIDGET_H
