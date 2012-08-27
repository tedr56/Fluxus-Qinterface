#include <QtGui/QMessageBox>
#include <QDebug>
#include "qmidiserver.h"
#include "mididefs.h"

QMidiServer::QMidiServer(QWidget *parent, QString MidiServerName = "RtMidi", QString MidiServerPortName = "RtMidiPort"):
    MidiBase(),
    m_inputActive(false)
{
    m_parent = parent;
    m_server_name = MidiServerName;
    m_server_port_name = MidiServerPortName;

    initMidi();
}

void midiCallback( double /*deltatime*/,
                   std::vector< unsigned char > *message,
                   void *userData )
{
    QMidiServer* instance = static_cast<QMidiServer *>(userData);
    MidiInfo *midiInfo = 0;
    unsigned char status = message->at(0) & MASK_STATUS;
    unsigned char channel = message->at(0) & MASK_CHANNEL;
    unsigned char value = 0;
    switch( status )
    {
        case STATUS_CTLCHG:
        case STATUS_NOTEON:
        case STATUS_NOTEOFF:
        {
            unsigned char midi_control = message->at(1);
            value = message->at(2);
            if ((status == STATUS_NOTEOFF))
            {
                status = STATUS_NOTEON;
                value = 0;
            }
            midiInfo = new MidiInfo((int)status, (int)channel, (int)midi_control);
            instance->SendControl(*midiInfo, value);
            break;
        }
    }
    delete midiInfo;
}

bool QMidiServer::initMidi()
{
    QList<int> STATUS;
    STATUS << STATUS_PROGRAM << STATUS_CTLCHG << STATUS_NOTEON << STATUS_CHANAFT << STATUS_BENDER << STATUS_POLYAFT;

    try {
        m_midiin = new RtMidiIn(RtMidi::UNSPECIFIED, m_server_name.toStdString());
#if !defined(__LINUX_ALSA__) && !defined(__MACOSX_CORE__) && !defined(__LINUX_JACK__)
        int nInPorts = m_midiin->getPortCount();
        if (nInPorts == 0) {
            delete m_midiin;
            m_midiin = 0;
        }
#endif
#if defined(__LINUX_ALSA__) || defined(__MACOSX_CORE__) || defined(__LINUX_JACK__)
        if (m_midiin != 0)
            m_midiin->openVirtualPort(m_server_port_name.toStdString());
#endif
        if (m_midiin != 0) {
            // ignore SYX, clock and active sense
            m_midiin->ignoreTypes(true,true,true);
            m_midiin->setCallback( &midiCallback, this );
            m_inputActive = true;
            m_initialized = true;
        }
    } catch (RtError& err) {
        QMessageBox::critical( m_parent , tr("Error. Aborting"), QString::fromStdString(err.getMessage()));
        return false;
    }
    return true;
}

void QMidiServer::SendControl(const MidiInfo infos, int value)
{
    QList<const MidiObject *> controlList = m_controlList.values(infos);

    foreach(const MidiObject *MidiO, controlList)
    {
        MidiO->SendSignal(value);
    }
}

void QMidiServer::RegisterControl(MidiObject *m)
{
    m_controlList.insert(m->getInfo(), m);
}

void QMidiServer::UnRegisterControl(MidiObject *m)
{
    m_controlList.remove(m->getInfo(),m);
}

QMidiServer::~QMidiServer()
{
    try {
        if (m_midiin != 0) {
            m_midiin->cancelCallback();
            m_inputActive = false;
            m_midiin->closePort();
            delete m_midiin;
        }
    } catch (RtError& err) {
        qWarning() << QString::fromStdString(err.getMessage());
    }
}
