#ifndef MIDISERVER_H
#define MIDISERVER_H

#include <QtCore/QObject>
#include "qmiditypes.h"
#include <QMultiHash>
#include "RtMidi.h"


class QMidiServer : public MidiBase
{
    Q_OBJECT
    friend class MidiObject;
public:
    QMidiServer(QWidget*, QString, QString);
    ~QMidiServer();

    bool initMidi();
    void SendControl(const MidiInfo, int);

private:
    QWidget *m_parent;
    QString m_server_name;
    QString m_server_port_name;

    RtMidiIn* m_midiin;

    bool m_inputActive;

    void RegisterControl( MidiObject* );
    void UnRegisterControl( MidiObject* );

    QMultiHash<const MidiInfo, const MidiObject*> m_controlList;
};

#endif
