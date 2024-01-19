#ifndef INFITRACER_H
#define INFITRACER_H

#include <QObject>
#include "qcustomplot.h"
class InfiTracer : public QObject
{
    Q_OBJECT
public:
    explicit InfiTracer(QCustomPlot *plot,QCPGraph *graph, QObject *parent = nullptr);
    ~InfiTracer();
public:
    void updatePosition(double xValue, double yValue, bool right = true);
    void setVisible(bool vis);
    
signals:

public:
    Q_SLOT void SlotMouseMoveEvent(QMouseEvent* event);

private:
    QCustomPlot *m_plot = nullptr;	     //传入实例化的QcustomPlot
    QCPGraph *m_graph = nullptr;	   	 //这里是存传入的绘图图层
    QCPItemTracer *m_tracer = nullptr;   // 跟踪的点
    QCPItemText *m_label = nullptr;   	 // 显示的数值
    QCPItemLine *m_arrow = nullptr;  	 // 箭头

    bool m_visible = false;
};

#endif // INFITRACER_H
