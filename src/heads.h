#ifndef HEADS_H
#define HEADS_H

#include <QtCore>

class Screen : public QObject
{
    Q_OBJECT
public:
    Screen(int ResX, int ResY, int displays, QObject* parent);
    void setScreen(int ResX, int ResY, int displays);
    int getResX();
    int getResY();
    int getDisplays();
    QList<int> getParams();
protected:
    int m_resX;
    int m_resY;
    int m_display;
};

class Media : QObject
{
    Q_OBJECT
public:
    Media(QString Type, QObject * parent);
    Media(QString Type, QList<QString> AuthorizedTypes, QObject * parent);
    QString getType();
    bool AccectType(QString Type);
    void addAcceptType(QString Type);
    void removeAcceptType(QString Type);

protected:
    QString m_type;
    QHash<QString, bool> m_authorizedTypes;
};

class Head : public QObject
{
    Q_OBJECT

public:
    Head(QObject *parent);
    QList<Screen*> getScreen();
    void setScreen(int index, int ResX, int ResY, int Displays);

    virtual QList<Media*> getMedia() = 0;
    virtual bool updateMedia(Media* updateMedia) = 0;

protected:
    QList<Screen*> m_screens;
    QHash<Media*, bool> m_availableMedia;
};

#endif // HEADS_H
