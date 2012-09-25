#ifndef MEDIACONTROL_H
#define MEDIACONTROL_H
#include <Qt>
#include <QFileInfo>
#include <QLabel>
#include "buttoncontrol.h"

class MediaControl : public ButtonControl
{
    Q_OBJECT

public:
    MediaControl(QWidget* parent);
    QString mediaName();
signals:
    void startDragging();
    void clicking();
protected slots:
    void dragging();
protected:
    void setMediaName(QString);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

private:
    QString m_mediaName;
    QLabel* m_nameLabel;
    bool m_dragging;
    QPoint startMousePoint;
};

class FluxusVisual : public MediaControl
{
    Q_OBJECT
public:
    FluxusVisual(QFileInfo fileInfo, QWidget* parent, int defValues = 0);
    FluxusVisual(QString pathInfo, QWidget* parent, int defValues = 0);
protected slots:
    void dragging();
protected:
    void initMedia();
    int checkControls(QFileInfo);
    void showDefaultsValues();
    void ParametersDeck();
private:
    QFileInfo m_fileInfo;
    QStringList *m_controlList;
    int m_defaultsValues;
};

#endif
