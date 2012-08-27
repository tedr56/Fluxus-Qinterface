#include <QMainWindow>
#include <QtGui/QMessageBox>
#include <QDebug>
#include "qmidiclient.h"

QMidiClient::QMidiClient(QWidget *parent, QString MidiClientName, QString MidiClientPortName):
    MidiBase(),
    m_outputActive(false)
{
    m_parent = parent;
    m_client_name = MidiClientName;
    m_client_port_name = MidiClientPortName;

    initMidi();
}

bool QMidiClient::initMidi()
{
    try {
        m_midiout = new RtMidiOut(RtMidi::UNSPECIFIED, m_client_name.toStdString());
#if !defined(__LINUX_ALSA__) && !defined(__MACOSX_CORE__) && !defined(__LINUX_JACK__)
        int nOutPorts = m_midiout->getPortCount();
        if (nOutPorts == 0) {
            delete m_midiout;
            m_midiout = 0;
            QMessageBox::critical( qobject_cast<QWidget*>(this->parent()), tr("Error"),
                tr("No MIDI output ports available. Aborting") );
            return false;
        }
#endif
#if defined(__LINUX_ALSA__) || defined(__MACOSX_CORE__) || defined(__LINUX_JACK__)
        if (m_midiout != 0)
            m_midiout->openVirtualPort(m_client_port_name.toStdString());
#endif
        if (m_midiout != 0) {
            m_outputActive = true;
            m_initialized = true;
        }
    } catch (RtError& err) {
        QMessageBox::critical( m_parent, tr("Error. Aborting"),
                               QString::fromStdString(err.getMessage()));
        return false;
    }
    return true;
}

void QMidiClient::sendData(MidiInfo *infos, int value)
{
    std::vector<unsigned char> message;
    int status = infos->getType();

    switch(status)
    {
    case(STATUS_CTLCHG):
    {
        unsigned char chan = static_cast<unsigned char>(infos->getChannel());
        unsigned char ctl  = static_cast<unsigned char>(infos->getControl());
        unsigned char val  = static_cast<unsigned char>(value);
        // Controller: 0xB0 + channel, ctl, val
        message.push_back(STATUS_CTLCHG + (chan & MASK_CHANNEL));
        message.push_back(ctl & MASK_SAFETY);
        message.push_back(val & MASK_SAFETY);
        sendMessageWrapper( &message );
        break;
    }
    case(STATUS_NOTEON):
    case(STATUS_NOTEOFF):
    {
        unsigned char chan = static_cast<unsigned char>(infos->getChannel());
        unsigned char ctl  = static_cast<unsigned char>(infos->getControl());
        unsigned char val  = static_cast<unsigned char>(value);
        // Controller: 0xB0 + channel, ctl, val
        if (value == 0)
            status = STATUS_NOTEOFF;
        message.push_back(status + (chan & MASK_CHANNEL));
        message.push_back(ctl & MASK_SAFETY);
        message.push_back(val & MASK_SAFETY);
        sendMessageWrapper( &message );
        break;
    }
    }
}

void QMidiClient::sendMessageWrapper(std::vector<unsigned char> *message) const
{
    try {
        m_midiout->sendMessage( message );
    } catch (RtError& err) {
        qDebug() << QString::fromStdString(err.getMessage());
//        QMainWindow *parent = qobject_cast<QMainWindow *>(this->parent());
//        if (parent)
//            parent->
//            parent->ui.statusBar->showMessage(QString::fromStdString(err.getMessage()));
    }

}

QMidiClient::~QMidiClient()
{
    try {
        if (m_midiout != 0) {
            m_midiout->closePort();
            delete m_midiout;
        }
    } catch (RtError& err) {
        qWarning() << QString::fromStdString(err.getMessage());
    }
}
