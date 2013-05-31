#include "multicontrol.h"
#include <QDebug>

MultiControl::MultiControl(QWidget *parent) :
    QWidget(parent),
    m_inputMode(MidiCC), m_midiinputmode(MidiCC),
    m_min(0), m_max(127),
    m_oscMin(0), m_oscMax(127), m_oscInAddress(""), m_oscInArg(0),
    m_midichannel(0), m_midicontrol(0), m_value(0),
    m_outputMode(OutMidi), m_oscoutAddress(""),
    m_midiinfo(0),
    m_midiserverobject(0), m_midiclientobject(0),
    m_oscserverobject(0),
    m_midiserver_set(false), m_midiclient_set(false),
    m_oscserver_set(false), m_oscclient_set(false),
    m_mimeType("multicontrol/multicontrol"), m_inputMime("multicontrol/parameter"), m_acceptedDrops(Qt::CopyAction)
{
    setMidiInfo();
}

MultiControl::~MultiControl()
{
    delete m_midiserverobject;
    delete m_midiclientobject;
    delete m_oscserverobject;
    foreach(PathObject* Path, m_oscclientsobject)
    {
        delete Path;
    }
    delete m_midiinfo;
}

void MultiControl::setInputMode(ControlInputType typeMode)
{
    m_inputMode = typeMode;
    if (typeMode != Osc)
        m_midiinputmode = typeMode;
}

void MultiControl::setMinimum(int Min)
{
    m_min = Min;
}

void MultiControl::setMaximum(int Max)
{
    m_max = Max;
}

void MultiControl::setOscMin(double OscMin)
{
    m_oscMin = OscMin;
}

void MultiControl::setOscMax(double OscMax)
{
    m_oscMax = OscMax;
}

void MultiControl::setOscInputAddress(const QString &OscAddress)
{
    m_oscInAddress = OscAddress;
}

void MultiControl::setOscInputArgument(int Argument)
{
    m_oscInArg = Argument;
}

void MultiControl::setChannel(int Channel)
{
    m_midichannel = Channel;
    setMidiInfo();
}

void MultiControl::setControl(int Control)
{
    m_midicontrol = Control;
    setMidiInfo();
}

void MultiControl::setMidiInfo()
{
    QMidiServer *server = 0;
    QMidiClient *client = 0;
    if (m_midiserver_set)
    {
        server = m_midiserverobject->getMidiServer();
        delete m_midiserverobject;
    }
    if (m_midiclient_set)
    {
        client  = m_midiclientobject->getMidiClient();
        delete m_midiclientobject;
    }
    if (m_midiinfo)
    {
        delete(m_midiinfo);
    }
    m_midiinfo = new MidiInfo(m_midiinputmode, m_midichannel, m_midicontrol);
    if (server)
    {
        setMidiObject(server);
    }
    if (client)
    {
        setMidiObject(client);
    }
}

void MultiControl::setMimeType(QString newMime)
{
    m_mimeType = newMime;
}

MidiInfo *MultiControl::getMidiInfo()
{
    return m_midiinfo;
}

void MultiControl::setMidiObject(QMidiServer *midiServer)
{
    m_midiserverobject = new MidiObject(getMidiInfo() , midiServer);
    connect(m_midiserverobject, SIGNAL(data(int)), this, SLOT(ControllerUpdate(int)));
    m_midiserver_set = true;
}

void MultiControl::setMidiObject(QMidiClient * midiClient)
{
    m_midiclientobject = new MidiObject(getMidiInfo(),midiClient);
    connect(this, SIGNAL(ControllerMoved(int)), m_midiclientobject, SLOT(sendData(int)));
    m_midiclient_set = true;
}

void MultiControl::setOscObject(QOscServer* oscserver)
{
    if (m_oscInAddress.count())
    {
        m_oscserverobject = new PathObject(m_oscInAddress, QVariant::Int, oscserver);
        connect(m_oscserverobject, SIGNAL(data(int)), this, SLOT(ControllerUpdate(int)));
        m_oscserver_set = true;
    }
}

void MultiControl::setOscObject(QList<QOscClient*> OscList)
{
    QString oscoutAddress;
    if (m_oscoutAddress.count())
        oscoutAddress = m_oscoutAddress;
    else if (m_oscInAddress.count())
        oscoutAddress = m_oscInAddress;

    if (oscoutAddress.count())
    {
        foreach(PathObject* oscP, m_oscclientsobject)
        {
            delete oscP;
        }
        m_oscclientsobject.clear();
        if (OscList.empty())
        {
            m_oscclient_set = false;
        }
        else
        {
            foreach(QOscClient *oscClient, OscList)
            {
                PathObject *clientpath = new PathObject(oscoutAddress, QVariant::Int, oscClient);
                m_oscclientsobject.append(clientpath);
                connect(this, SIGNAL(ControllerMovedBridge(int)), clientpath, SLOT(send(int)));
            }
            m_oscclient_set = true;
        }
    }
}

void MultiControl::setStyle(QStyle *s)
{
    QWidget::setStyle(s);
}

QStyle *MultiControl::style()
{
    return QWidget::style();
}

QString MultiControl::mimeType()
{
    return m_mimeType;
}

void MultiControl::setOutputMode(ControlOutputTypes outMode)
{
    m_outputMode = outMode;

    if (outMode.testFlag(OutMidi))
    {
        if ((!outMode.testFlag(OutOsc)) && m_oscclient_set)
        {
            foreach(PathObject* Path, m_oscclientsobject)
            {
                disconnect(this, SIGNAL(ControllerMovedBridge(int)), Path, SLOT(send(int)));
            }
        }
        if (m_midiclient_set)
        {
            connect(this, SIGNAL(ControllerMoved(int)), m_midiclientobject , SLOT(sendData(int)));
        }
    }
    else if (outMode.testFlag(OutOsc))
    {
        if ((!outMode.testFlag(OutMidi)) && m_midiclient_set)
        {
            disconnect(this, SIGNAL(ControllerMoved(int)), m_midiclientobject, SLOT(sendData(int)));
        }
        if (m_oscclient_set)
        {
            //connect(this, SIGNAL(ControllerMoved(int)), m_oscclientobject, SLOT(send(int)));
            foreach(PathObject* Path, m_oscclientsobject)
            {
                connect(this, SIGNAL(ControllerMovedBridge(int)), Path, SLOT(send(int)));
            }
        }
    }
}

void MultiControl::setOscOutputAddress(const QString &OscOutputAddress)
{
    m_oscoutAddress = OscOutputAddress;
}

void MultiControl::ControllerUpdate(int Value)
{
    setValue(Value);
    emit WidgetUpdate(Value);
    emit ControllerMovedBridge(Value);
}

void MultiControl::WidgetMoved(int Value)
{
    emit ControllerMoved(Value);
    emit ControllerMovedBridge(Value);
}

int MultiControl::getValue()
{
    return m_value;
}

void MultiControl::setValue(int Value)
{
    m_value = Value;
}


QStringList MultiControl::checkMimeType(QStringList mimeTypes)
{
    QStringList validMime;
    foreach(QString mime, mimeTypes){
        if (m_inputMime.contains(mime)){
            validMime.append(mime);
        }
    }
    return validMime;
}

bool MultiControl::checkParentDrop(QDropEvent* event)
{
    if (event->source()->parent() == parent()){
        event->setDropAction(Qt::MoveAction);
        return true;
    } else {
        return false;
    }

}

void MultiControl::dragEnterEvent(QDragEnterEvent* event)
{
    if (checkMimeType(event->mimeData()->formats()).count()){
        if (checkParentDrop(event)){
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }
}

void MultiControl::dragMoveEvent(QDragMoveEvent *event)
{
    if (checkMimeType(event->mimeData()->formats()).count()){
        if (checkParentDrop(event)){
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }
}

void MultiControl::dropEvent(QDropEvent *event)
{
    if (checkMimeType(event->mimeData()->formats()).count()){

        if (checkParentDrop(event)){
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }
}


//virtual MultiControl::~MultiControlInterface()
//{
////    ~MultiControl();
//}
