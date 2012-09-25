#ifndef BUTTONCONTROL_H
#define BUTTONCONTROL_H

#include <Qt>
#include "multicontrol.h"
#include "ui_buttoncontrol.h"


class ButtonControl : public MultiControl, protected Ui::ButtonWidget
{
    Q_OBJECT
    Q_PROPERTY( bool Checkable  READ getCheckable WRITE setCheckable  )
public:
    //Constructor
    ButtonControl(QWidget *Parent = 0);
    void setStyle(QStyle *);
    bool getCheckable();
public slots:
    void Toggle(int);
    void ButtonToggled(bool);
    void ButtonToggled();
    void setCheckable(bool check);
    void setInputMode(ControlInputType typeMode);
signals:
    void ButtonMoved(int);

//protected:
//    Ui::ButtonWidget *ui;
private:
    uint m_velocity;

};

#endif // BUTTONCONTROL_H
