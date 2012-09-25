#ifndef DROPTABWIDGET_H
#define DROPTABWIDGET_H

#include <QTabWidget>
#include <QTabBar>
#include <QTimer>
#include "mediawidget.h"

class DropTabBar : public QTabBar
{
    Q_OBJECT
public:
    DropTabBar(QWidget* parent = 0);

signals:
    void TabDropActivate(int);

public slots:
    void DropTabTimeOut();
    void showContextMenu(const QPoint &);
    void renameTab();

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

    int addTab(const QString &);
    int addTab(QWidget *widget, const QString &);
    int addTab(QWidget *widget, const QIcon &icon, const QString &label);

    MediaWidget *getScrollWidget(int);
public slots:
    int addDeckTab();

private:
    QList<MediaWidget*> m_mimeList;
    void setScrollbar(int);
    void renameTab(int);
};

#endif
