#ifndef DROPDOCKWIDGET_H
#define DROPDOCKWIDGET_H

#include <QDockWidget>

class DropDockWidget : public QDockWidget
{
    Q_OBJECT
public:
    DropDockWidget(QWidget *parent = 0);
public slots:
    void DropTabTimeOut();
protected:
    void dragEnterEvent(QDragEnterEvent*);
    void dragMoveEvent(QDragMoveEvent *);
    void dragLeaveEvent(QDragLeaveEvent *);
    void dropEvent(QDropEvent *);
private:
    QTimer *tabTimer;
    QString m_mimeType;
};


#endif
