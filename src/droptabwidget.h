#ifndef DROPTABWIDGET_H
#define DROPTABWIDGET_H

#include <QTabWidget>
#include <QTabBar>
#include <QTimer>

class DropTabBar : public QTabBar
{
    Q_OBJECT
public:
    DropTabBar(QWidget* parent = 0);

signals:
    void TabDropActivate(int);

public slots:
    void DropTabTimeOut();

protected:
    void dragEnterEvent(QDragEnterEvent*);
    void dragMoveEvent(QDragMoveEvent *);
    void dragLeaveEvent(QDragLeaveEvent *);
    void dropEvent(QDropEvent *);

private:
    QTimer *tabTimer;
    int m_overTabIndex;
    QString m_mimeType;
};

class DropTabWidget : public QTabWidget
{
    Q_OBJECT

public:
    DropTabWidget(QWidget* parent = 0);

};

#endif
