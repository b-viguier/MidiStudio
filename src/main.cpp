#include "forms/mainwindow.h"
#include <QApplication>

#include "core/qmidi.h"
#include "forms/statuswidget.h"
#include "forms/recordwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QMidi midi;

    w.addDockWidget(
                Qt::LeftDockWidgetArea,
                new StatusWidget(midi)
                );
    w.addDockWidget(
                Qt::LeftDockWidgetArea,
                new RecordWidget(midi)
                );
    w.show();
    return a.exec();
}
