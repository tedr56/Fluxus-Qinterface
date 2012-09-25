#ifndef MEDIAWIDGET_H
#define MEDIAWIDGET_H

#include <QWidget>
#include <QtGui>
#include <Qt>

class QDragEnterEvent;
class QDropEvent;

class StackHBoxLayout : public QBoxLayout
{
    Q_OBJECT
public:
    StackHBoxLayout(QWidget* parent = 0);
    void addWidget(QWidget *, int stretch, Qt::Alignment alignment);
};

class MediaWidget : public QWidget
{
    Q_OBJECT

public:
    MediaWidget(QWidget* parent);
    QStringList MimeType();
    void setParametersDock(QWidget*);
protected:
    void setMimeType(QStringList);
    void dragEnterEvent(QDragEnterEvent* event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    void dropAction(QDropEvent *event);
    //void mousePressEvent(QMouseEvent *event);

    bool checkMimeFormat(QStringList);
    QStringList getValidMimeFormats(QStringList);

    QStringList m_mimeTypes;
    QStringList m_externalMimeTypes;

    StackHBoxLayout* m_layout;
    void addWidget(QWidget* w);

    QWidget* m_parametersDock;
private:
    Qt::DropAction DragFlag;
    Qt::DropActions DropFlags;
};


class DeckWidget : public MediaWidget
{
    Q_OBJECT
public:
    DeckWidget(QWidget* parent);
    QStringList MimeType();
protected:
    void dropEvent(QDropEvent *event);
    void dropAction(QDropEvent *event);
private:
    QStringList getFilesPath(QList<QUrl>);
    int addFluxusMedia(QDropEvent *event);
};

class SideWidget : public MediaWidget
{
    Q_OBJECT
public:
    SideWidget(QWidget *parent = 0);
protected:
};

#endif
