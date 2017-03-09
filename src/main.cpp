#include "forms/mainwindow.h"
#include <QApplication>

#include "core/qmidi.h"
#include "core/sequencer.h"
#include "core/timeline.h"

#include "forms/statuswidget.h"
#include "forms/recordwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QMidi midi;
    TimeLine timeline(10);
    Sequencer sequencer(midi, timeline);

    w.addDockWidget(
                Qt::LeftDockWidgetArea,
                new StatusWidget(midi)
                );
    w.addDockWidget(
                Qt::LeftDockWidgetArea,
                new RecordWidget(sequencer)
                );
    w.show();
    return a.exec();
}
