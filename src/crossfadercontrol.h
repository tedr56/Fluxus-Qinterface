#ifndef CROSSFADERCONTROL_H
#define CROSSFADERCONTROL_H

#include <Qt>
#include "multicontrol.h"
#include "ui_crossfadercontrol.h"

class MultiControl;

class CrossFaderControl : public MultiControl, private Ui::CrossFaderControl
{
    Q_OBJECT

public:
    //Constructor
    CrossFaderControl(QWidget *Parent = 0);
    void setStyle(QStyle *);
    void setMinimum(int Min);
    void setMaximum(int Max);

public slots:


private:

};

#endif // FADERCONTROL_H
