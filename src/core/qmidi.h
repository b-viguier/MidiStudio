#ifndef QMIDI_H
#define QMIDI_H

#include <QObject>
#include <QScopedPointer>

#include "midimessage.h"

class QString;
class RtMidiIn;
class RtMidiOut;

class QMidi : public QObject
{
    Q_OBJECT
public:

    enum Type {
        IN,
        OUT,
    };

    explicit QMidi(QObject *parent = 0);
    ~QMidi();

    void resetTime(unsigned int millis = 0);

    template<Type type>
    QStringList availableInterfaces() const;
    template<Type type>
    bool selectInterface(const QString& name);



signals:
    void errorOccurred(const QString& details);
    void messageReceived(unsigned int millis, const MidiMessage& message);

public slots:
    void sendMessage(const MidiMessage& message);

private:
    Q_DISABLE_COPY(QMidi)
    struct Internal;
    QScopedPointer<Internal> _pimpl;
};

#endif // QMIDI_H
