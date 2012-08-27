#ifndef QMIDITYPES_H
#define QMIDITYPES_H

#include <QMetaType>
#include <QDebug>

class QMidiServer;
class QMidiClient;

class MidiInfo : public QObject
{
    Q_OBJECT
public:
    MidiInfo();
    MidiInfo(int, QList<int>);
    MidiInfo(int, int, int);
    MidiInfo(const MidiInfo *other);
    MidiInfo(const MidiInfo &other);
    const MidiInfo operator=(const MidiInfo *other);
//    const MidiInfo operator=(const MidiInfo &other);
    ~MidiInfo();

    const int &getType() const;
    const int &getChannel() const;
    const int &getControl() const;

private:
    int m_type;
    int m_channel;
    int m_control;
};

inline uint qHash(const MidiInfo &key)
{
    return qHash(key.getType() ^ key.getChannel() ^ key.getControl());
}

inline bool operator==(const MidiInfo &I1, const MidiInfo &I2)
{
    return ((I1.getType() == I2.getType()) && (I1.getChannel() == I2.getChannel()) && (I1.getControl() == I2.getControl()));
}

//Q_DECLARE_METATYPE(MidiInfo);

inline QDebug operator<<(QDebug dbg, const MidiInfo *midiinfo)
{
    dbg.nospace() << "(" << midiinfo->getType() << ", " << midiinfo->getChannel() << ", " << midiinfo->getControl() << ")";
    return dbg.space();
}

class MidiObject : public QObject
{
    Q_OBJECT
    friend class QMidiServer;

public:
    //MidiObject();
    MidiObject(MidiInfo*, QMidiServer *);
    MidiObject(MidiInfo*, QMidiClient *);
    ~MidiObject();

    MidiInfo* getInfo() const;
    QMidiServer *getMidiServer() { return _server; };
    QMidiClient *getMidiClient() { return _client; };

public slots:
    void sendData(int);

signals:
    void data(int) const;
    void data(MidiInfo*, int);

protected:
    //Called by QMidiServer
    void SendSignal(int) const;

private:
    MidiInfo* m_midiinfo;
    QMidiServer* _server;
    QMidiClient* _client;
};

//uint QHash(const MidiInfo*);

class MidiBase : public QObject
{
    Q_OBJECT

public:
    MidiBase();
    bool Initialized();

protected:
    bool m_initialized;

private:

};

#endif
