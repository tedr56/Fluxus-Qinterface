#ifndef MULTICONTROL_H
#define MULTICONTROL_H

#include <QWidget>
#include <QDragEnterEvent>
#include "qosctypes.h"
#include "qoscserver.h"
#include "qoscclient.h"
#include "qmiditypes.h"
#include "qmidiserver.h"
#include "qmidiclient.h"
#include "mididefs.h"
#include "multicontrolinterface.h"

class QDragEnterEvent;
class QDropEvent;

class MultiControl : public QWidget, public MultiControlInterface
{
    Q_OBJECT
    Q_INTERFACES(MultiControlInterface)

    Q_ENUMS(ControlInputType)
    Q_FLAGS(ControlOutputType ControlOutputTypes)

    Q_PROPERTY( ControlInputType InputMode READ getInputMode WRITE setInputMode )
    Q_PROPERTY( double OscMin  READ getOscMin WRITE setOscMin  )
    Q_PROPERTY( double OscMax  READ getOscMax WRITE setOscMax  )
    Q_PROPERTY( const QString &OscInputAddress READ getOscInputAddress WRITE setOscInputAddress)
    Q_PROPERTY( int OscInputArgument READ getOscInputArgument WRITE setOscInputArgument)
    Q_PROPERTY( int MidiChannel READ getChannel WRITE setChannel)
    Q_PROPERTY( int MidiControl READ getControl WRITE setControl)

    Q_PROPERTY(int Value READ getValue WRITE setValue)

    Q_PROPERTY( ControlOutputTypes OutputMode READ getOutputMode WRITE setOutputMode)
    Q_PROPERTY( const QString &OscOutputAddress READ getOscOutputAddress WRITE setOscOutputAddress)

public:
    MultiControl(QWidget *parent = 0);
    ~MultiControl();

    enum ControlInputType  { MidiCC = STATUS_CTLCHG, MidiNote = STATUS_NOTEON, Osc};
    enum ControlOutputType { OutMidi = 0x01 , OutOsc = 0x02};

    Q_DECLARE_FLAGS(ControlOutputTypes, ControlOutputType)

    ControlInputType        getInputMode()          const { return m_inputMode;     }
    double                   getOscMin()             const { return m_oscMin;        }
    double                   getOscMax()             const { return m_oscMax;        }
    const QString          &getOscInputAddress()    const { return m_oscInAddress;  }
    int                     getOscInputArgument()   const { return m_oscInArg;      }
    int                     getChannel()            const { return m_midichannel;   }
    int                     getControl()            const { return m_midicontrol;   }

    int                     getValue();

    ControlOutputTypes      getOutputMode()         const { return m_outputMode;    }
    const QString          &getOscOutputAddress()   const { return m_oscoutAddress; }

    //MultiControl *Copy();
public slots:
    void setInputMode(ControlInputType typeMode);
    void setOscMin(double OscMin);
    void setOscMax(double OscMax);
    void setOscInputAddress(const QString &OscAddress);
    void setOscInputArgument(int Argument);
    void setChannel(int Channel);
    void setControl(int Control);

    void setOutputMode(ControlOutputTypes outMode);
    void setOscOutputAddress(const QString &OscOutputAddress);

    void ControllerUpdate(int Value);
    void WidgetMoved(int Value);

    MidiInfo *getMidiInfo();
    void setMidiObject(QMidiServer*);
    void setMidiObject(QMidiClient*);
    void setOscObject(QOscServer*);
    void setOscObject(QList<QOscClient*>);

    QString mimeType();
signals:
    void WidgetUpdate(int Value);
    void ControllerMoved(int);
    void ControllerMovedBridge(int);
protected:
    int getMidiValue();
    double getOscValue();

    int getMinimum() {return m_min;};
    int getMaximum() {return m_max;};

    void setValue(int Value);
    void setMinimum(int Min);
    void setMaximum(int Max);

    void setMidiInfo();

    void setMimeType(QString);
    void dragEnterEvent(QDragEnterEvent* event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
private:
    ControlInputType m_inputMode;
    ControlInputType m_midiinputmode;
    int m_min;
    int m_max;
    double m_oscMin;
    double m_oscMax;
    QString m_oscInAddress;
    int m_oscInArg;
    int m_midichannel;
    int m_midicontrol;
    int m_value;
    ControlOutputTypes m_outputMode;
    QString m_oscoutAddress;

    MidiInfo *m_midiinfo ;
    MidiObject *m_midiserverobject;
    MidiObject *m_midiclientobject;
    PathObject *m_oscserverobject;
    QList<PathObject *> m_oscclientsobject;

    bool m_midiserver_set;
    bool m_midiclient_set;
    bool m_oscserver_set;
    bool m_oscclient_set;

    QString m_mimeType;
    QStringList m_inputMime;
    Qt::DropActions m_acceptedDrops;
    QStringList checkMimeType(QStringList);
    bool checkParentDrop(QDropEvent *event);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(MultiControl::ControlOutputTypes)

#endif // MULTICONTROL_H
