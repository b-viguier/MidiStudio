#include "midimessage.h"
#include <QMetaType>

Q_DECLARE_METATYPE(MidiMessage);

namespace {

int midi_message_meta_type = qRegisterMetaType<MidiMessage>();

}
