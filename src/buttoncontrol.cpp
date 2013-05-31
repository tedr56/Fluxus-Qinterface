#include <QLabel>
#include "buttoncontrol.h"
#include <QDebug>

ButtonControl::ButtonControl ( QWidget *Parent )
    : MultiControl(Parent),
//      ui(new Ui::ButtonWidget),
      m_velocity(127)
{
    setupUi(this);
    connect(this, SIGNAL(WidgetUpdate(int)), this, SLOT(Toggle(int)));
    connect(MultiButton,SIGNAL(pressed()), this, SLOT(ButtonToggled()));
    connect(MultiButton,SIGNAL(released()), this, SLOT(ButtonToggled()));
    connect(this, SIGNAL(ButtonMoved(int)), this, SLOT(WidgetMoved(int)));
}

void ButtonControl::ButtonToggled()
{
    if (MultiButton->isDown())
        emit ButtonMoved(m_velocity);
    else
        emit ButtonMoved(0);
}

void ButtonControl::ButtonToggled(bool Toggle)
{
    emit ButtonMoved(m_velocity * Toggle);
}

void ButtonControl::Toggle(int Value)
{
    if (Value > 0)
    {
        MultiButton->setDown(true);
    }
    else
    {
        MultiButton->setDown(false);
    }
}

bool ButtonControl::getCheckable()
{
    return MultiButton->isCheckable();
}

void ButtonControl::setCheckable(bool check)
{
    MultiButton->setCheckable(check);
    if (check)
    {
        disconnect(MultiButton,SIGNAL(pressed()), this, SLOT(ButtonToggled()));
        disconnect(MultiButton,SIGNAL(released()), this, SLOT(ButtonToggled()));
        connect(MultiButton,SIGNAL(toggled(bool)), this, SLOT(ButtonToggled(bool)));
    }
    else
    {
        disconnect(MultiButton,SIGNAL(toggled(bool)), this, SLOT(ButtonToggled(bool)));
        connect(MultiButton,SIGNAL(pressed()), this, SLOT(ButtonToggled()));
        connect(MultiButton,SIGNAL(released()), this, SLOT(ButtonToggled()));
    }
}

void ButtonControl::setStyle(QStyle *style)
{
    MultiButton->setStyle(style);
}

void ButtonControl::setInputMode(ControlInputType typeMode)
{
    if (typeMode == MidiNote)
    {
        setCheckable(false);
    }
    else if (typeMode == MidiCC)
    {
        setCheckable(true);
    }
}
