#ifndef RECORDWIDGET_H
#define RECORDWIDGET_H

#include <QDockWidget>

class Sequencer;

namespace Ui {
class RecordWidget;
}

class RecordWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit RecordWidget(Sequencer& sequencer);
    ~RecordWidget();

private slots:
    void on_play_clicked(bool);
    void on_record_clicked(bool);
    void on_metronome_clicked(bool);

private:
    Ui::RecordWidget *ui;

    Sequencer& _sequencer;
};

#endif // RECORDWIDGET_H
