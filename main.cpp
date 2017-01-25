#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

#include "statuswidget.h"

#include <RtMidi.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    try {
        RtMidiIn midi_in;
        RtMidiOut midi_out;

        w.addDockWidget(
                    Qt::LeftDockWidgetArea,
                    new StatusWidget(midi_in, midi_out)
                    );
        w.show();
        return a.exec();
    }
    catch (RtMidiError &error) {
        qDebug() << error.getMessage().data();
        return EXIT_FAILURE;
    }

    return EXIT_FAILURE;
}
