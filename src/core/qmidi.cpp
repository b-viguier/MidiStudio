#include "qmidi.h"

#include <QDebug>
#include <QTime>
#include "RtMidi.h"

#include "midimessage.h"

#define QMIDI_VIRTUAL_PORT_NAME "MidiStudio"

// Explicit instantiation
template QStringList QMidi::availableInterfaces<QMidi::IN>() const;
template bool  QMidi::selectInterface<QMidi::IN>(const QString&);
template QStringList QMidi::availableInterfaces<QMidi::OUT>() const;
template bool  QMidi::selectInterface<QMidi::OUT>(const QString&);

struct QMidi::Internal {

    RtMidiOut midi_out;
    RtMidiIn midi_in;
    QTime time;

    static void onMidiError(RtMidiError::Type, const std::string &errorText, void* userData)
    {
        QMidi* midi = reinterpret_cast<QMidi*>(userData);
        emit midi->errorOccurred(QString::fromStdString(errorText));
    }

    static void onMidiEvent(double , std::vector<unsigned char> *message, void *userData)
    {
        QMidi* midi = reinterpret_cast<QMidi*>(userData);
        emit midi->messageReceived(midi->_pimpl->time.elapsed(), *message);
    }

    template<QMidi::Type type>
    RtMidi& midiInterface()
    {
        static_assert(type == QMidi::OUT || type == QMidi::IN, "Invalid QMidi::Type");
        if(type == QMidi::OUT) {
           return midi_out;
        }
        return midi_in;
    }
};

QMidi::QMidi(QObject *parent) : QObject(parent)
{
    try {
        _pimpl.reset(new Internal);
        _pimpl->midi_in.setErrorCallback(&Internal::onMidiError, this);
        _pimpl->midi_out.setErrorCallback(&Internal::onMidiError, this);
        _pimpl->midi_in.setCallback(&Internal::onMidiEvent, this);
        resetTime();
    }
    catch (RtMidiError &error) {
        qDebug() << error.getMessage().data();
        throw error;
    }
}

QMidi::~QMidi()
{

}

void QMidi::resetTime(unsigned int millis)
{
    _pimpl->time = QTime::fromMSecsSinceStartOfDay(millis);
    _pimpl->time.start();
}

void QMidi::sendMessage(const MidiMessage &message)
{
    _pimpl->midi_out.sendMessage(const_cast<MidiMessage*>(&message));
}

template<QMidi::Type type>
QStringList QMidi::availableInterfaces() const
{
    RtMidi& midi = _pimpl->midiInterface<type>();
    QStringList results;
    for(unsigned int i=0, nbPorts = midi.getPortCount(); i<nbPorts; ++i) {
        results << QString::fromStdString(midi.getPortName(i));
    }
    results << QMIDI_VIRTUAL_PORT_NAME;
    return results;
}

template<QMidi::Type type>
bool QMidi::selectInterface(const QString &name)
{
    RtMidi& midi = _pimpl->midiInterface<type>();
    midi.closePort();

    // Virtual port special case
    if( name == QMIDI_VIRTUAL_PORT_NAME ) {
        midi.openVirtualPort(QMIDI_VIRTUAL_PORT_NAME);
        return true;
    }

    const std::string std_name = name.toStdString();
    for(unsigned int i=0, nbPorts = midi.getPortCount(); i<nbPorts; ++i) {
        if(midi.getPortName(i) == std_name) {
            midi.openPort(i);
            return true;
        }
    }

    return false;
}
