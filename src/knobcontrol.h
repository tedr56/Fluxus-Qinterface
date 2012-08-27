#ifndef KNOBCONTROL_H
#define KNOBCONTROL_H

#include <Qt>
#include "multicontrol.h"
#include "knob.h"
#include "ui_knobcontrol.h"

class MultiControl;

class KnobControl : public MultiControl, private Ui::KnobControl
{
    Q_OBJECT

public:
    //Constructor
    KnobControl(QWidget *Parent = 0);
    void setStyle(QStyle *);
    void setMinimum(int Min);
    void setMaximum(int Max);

public slots:


private:

};

#endif // KNOBCONTROL_H
