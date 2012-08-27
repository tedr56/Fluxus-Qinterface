#ifndef FADERCONTROL_H
#define FADERCONTROL_H

#include <Qt>
#include "multicontrol.h"
#include "ui_fadercontrol.h"

class MultiControl;

class FaderControl : public MultiControl, private Ui::FaderControl
{
    Q_OBJECT

public:
    //Constructor
    FaderControl(QWidget *Parent = 0);
    void setStyle(QStyle *);
    void setMinimum(int Min);
    void setMaximum(int Max);

public slots:


private:

};

#endif // SLIDERCONTROL_H
