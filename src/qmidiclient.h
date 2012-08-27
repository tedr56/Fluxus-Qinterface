#ifndef MIDICLIENT_H
#define MIDICLIENT_H

#include <QtCore/QObject>
#include "RtMidi.h"
#include "qmiditypes.h"
#include "mididefs.h"

class QMidiClient : public MidiBase
{
    Q_OBJECT
public:
    QMidiClient(QWidget *parent , QString MidiClientName = "RtMidi", QString MidiClientPortName = "RtMidiPort");
    ~QMidiClient();

    bool initMidi();

public slots:
    void sendData(MidiInfo*, int);

private:
    void sendMessageWrapper(std::vector<unsigned char> *message) const;

    QWidget *m_parent;
    QString m_client_name;
    QString m_client_port_name;

    RtMidiOut* m_midiout;

    bool m_outputActive;
};

#endif
