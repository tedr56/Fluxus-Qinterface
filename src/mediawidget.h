#ifndef MEDIAWIDGET_H
#define MEDIAWIDGET_H

#include <QWidget>
#include <QtGui>
#include <Qt>

class QDragEnterEvent;
class QDropEvent;

class MediaWidget : public QWidget
{
    Q_OBJECT

public:
    MediaWidget(QWidget* parent = 0);
    void setMimeType(QString& Type) { m_mimeType=Type; } ;
    QString MimeType() { return m_mimeType; };

protected:
    void dragEnterEvent(QDragEnterEvent* event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:
    QHBoxLayout *MediaLayout;
    QString m_mimeType;
    Qt::DropAction DragFlag;
    Qt::DropActions DropFlags;
};

#endif
