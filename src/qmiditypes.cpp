#include "qmiditypes.h"
#include "qmidiserver.h"
#include "qmidiclient.h"
#include "mididefs.h"

#include "QDebug"

MidiInfo::MidiInfo() : QObject()
{
}

MidiInfo::MidiInfo(int type, QList<int> address) : QObject(),
    m_type(type),
    m_channel(address.at(0)),
    m_control(address.at(1))
{
}

MidiInfo::MidiInfo(int Type, int Channel, int Control) : QObject(),
    m_type(Type),
    m_channel(Channel),
    m_control(Control)
{
}

MidiInfo::MidiInfo(const MidiInfo *other) : QObject(),
    m_type(other->m_type) ,
    m_channel(other->m_channel),
    m_control(other->m_control)
{
}

MidiInfo::MidiInfo(const MidiInfo &other) : QObject(),
    m_type(other.m_type),
    m_channel(other.m_channel),
    m_control(other.m_control)
{
}


const MidiInfo MidiInfo::operator=(const MidiInfo *other)
{
    m_type = other->m_type;
    m_channel = other->m_channel;
    m_control = other->m_control;
    return this;
}

//const MidiInfo MidiInfo::operator=(const MidiInfo &other)
//{
//    m_type = other.m_type;
//    m_channel = other.m_channel;
//    m_control = other.m_control;
//    return this;
//}

const int &MidiInfo::getType() const
{
    return m_type;
}

const int &MidiInfo::getChannel() const
{
    return m_channel;
}

const int &MidiInfo::getControl() const
{
    return m_control;
}

//MidiObject::MidiObject() :
//    QObject(),
//    m_midiinfo(),
//    _server(0),
//    _client(0)
//{
//}

MidiObject::MidiObject(MidiInfo* midiInfo, QMidiServer* parent)
    : QObject(parent),
      m_midiinfo(midiInfo),
      _server(parent),
      _client(0)
{
    _server->RegisterControl(this);
}

MidiObject::MidiObject(MidiInfo* midiInfo, QMidiClient* parent)
    : QObject(parent),
      m_midiinfo(midiInfo),
      _server(0),
      _client(parent)
{
    connect(this, SIGNAL(data(MidiInfo*, int)), _client, SLOT(sendData(MidiInfo*,int)));
}

MidiObject::~MidiObject()
{
    if (_server)
        _server->UnRegisterControl(this);
}

void MidiObject::sendData(int value)
{
    if (_server)
        emit data(value);
    else if (_client)
        emit data(m_midiinfo, value);
}

void MidiObject::SendSignal(int value) const
{
    emit data(value);
}

MidiInfo* MidiObject::getInfo() const
{
    return m_midiinfo;
}

MidiBase::MidiBase()
    : QObject(),
      m_initialized(false)
{
}

bool MidiBase::Initialized()
{
    return m_initialized;
}

MidiInfo::~MidiInfo()
{
}
