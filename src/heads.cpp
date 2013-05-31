#include "heads.h"

////////////////////// Screen Class //////////////////////
// Define heads screen for mapping module
// m_display is not store dual/triple heads output

Screen::Screen(int ResX, int ResY, int displays, QObject *parent) :
    QObject(parent),
    m_resX(ResX),
    m_resY(ResY),
    m_display(displays)
{
}

void Screen::setScreen(int ResX, int ResY, int displays)
{
    m_resX = ResX;
    m_resY = ResY;
    m_display = displays;
}

int Screen::getResX()
{
    return m_resX;
}

int Screen::getResY()
{
    return m_resY;
}

int Screen::getDisplays()
{
    return m_display;
}

QList<int> Screen::getParams()
{
    QList<int> ParamsList;
    ParamsList << m_resX << m_resY << m_display;
    return ParamsList;
}

////////////////////// Media Class //////////////////////
// Interfacing class for clips, fx's, mapping fx's between Heads and Gui

Media::Media(QString Type, QObject *parent) :
    QObject(parent),
    m_type(Type)
{
}

Media::Media(QString Type, QList<QString> AuthorizedTypes, QObject *parent) :
    QObject(parent),
    m_type(Type)
{
    for (int i = 0 ; i < AuthorizedTypes.size(); i++) {
        m_authorizedTypes[AuthorizedTypes.at(i)] = true;
    }
}

QString Media::getType()
{
    return m_type;
}

bool Media::AccectType(QString Type)
{
    return m_authorizedTypes.contains(Type);
}

void Media::addAcceptType(QString Type)
{
    m_authorizedTypes[Type] = true;
}

void Media::removeAcceptType(QString Type)
{
    m_authorizedTypes.remove(Type);
}

////////////////////// Head Class //////////////////////
// Abstract class for all foreign and local apps target



Head::Head(QObject *parent) :
    QObject(parent)
{
}

QList<Screen *> Head::getScreen()
{
    return m_screens;
}

void Head::setScreen(int index, int ResX, int ResY, int Displays)
{
    if (index < m_screens.size()) {
        if (Displays == 0) {
            m_screens.removeAt(index);
        } else {
            m_screens.at(index)->setScreen(ResX, ResY, Displays);
        }
    } else {
        if (Displays != 0) {
            m_screens.append(new Screen(ResX, ResY, Displays, this));
        }
    }
}

