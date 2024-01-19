#include "InfiTracer.h"
#include <QFontMetrics>
InfiTracer::InfiTracer(QCustomPlot *plot, QCPGraph *graph, QObject *parent)
    : QObject(parent)
    , m_plot(plot)
    , m_graph(graph)
{
    m_tracer = new QCPItemTracer(m_plot);
    m_tracer->setGraph(m_graph);
    m_tracer->setStyle(QCPItemTracer::tsNone);//可以选择设置追踪光标的样式，这个是小十字，还有大十字，圆点等样式
    m_tracer->setInterpolating(false);
    m_tracer->setVisible(false);


    m_tracer->position->setTypeX(QCPItemPosition::ptPlotCoords);
    m_tracer->position->setTypeY(QCPItemPosition::ptPlotCoords);


    m_arrow = new QCPItemLine(m_plot);
    m_arrow->setLayer("tracerlayer");
    m_arrow->setPen(m_graph->pen());//设置箭头的颜色
    m_arrow->setClipToAxisRect(true);
    m_arrow->setHead(QCPLineEnding::esNone);
    m_arrow->end->setParentAnchor(m_tracer->position);
    // m_arrow->end->setCoords(5, 0);
    m_arrow->start->setParentAnchor(m_arrow->end);
    m_arrow->start->setCoords(10, 0);
    m_arrow->setSelectable(false);

    m_label = new QCPItemText(m_plot);
    m_label->setLayer("tracerlayer");
    m_label->setClipToAxisRect(true);
    m_label->setPadding(QMargins(5, 5, 5, 5));
    m_label->position->setParentAnchor(m_tracer->position);
    m_label->setFont(QFont("宋体", 10));
    m_label->setBrush(QBrush(QColor(244, 244, 244, 150)));
    m_label->setPen(m_graph->pen());
    m_label->setPositionAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    m_label->setSelectable(false);

    setVisible(false);
}

InfiTracer::~InfiTracer()
{
    if (m_tracer)
        m_plot->removeItem(m_tracer);
    if (m_label)
        m_plot->removeItem(m_label);
    if (m_arrow)
        m_plot->removeItem(m_arrow);
}

void InfiTracer::updatePosition(double xValue, double yValue, bool right)
{
    m_tracer->position->setCoords(xValue, yValue);
    m_label->position->setCoords(10, 0);
    m_label->setText(QString("%1,%2").arg(QString::number(xValue, 'f', 2)).arg(QString::number(yValue, 'f', 2)));
    // m_plot->replot();
    m_label->layer()->replot();
}

void InfiTracer::setVisible(bool vis)
{
    m_visible = vis;
    m_tracer->setVisible(m_visible);
    m_label->setVisible(m_visible);
    m_arrow->setVisible(m_visible);
    m_label->layer()->replot();
}

void InfiTracer::SlotMouseMoveEvent(QMouseEvent *event)
{
    if(!m_visible)
        return;
    if(!m_plot)
        return;
    auto size = m_graph->data()->size();
    if(size <= 0)
        return;
    double x = m_plot->xAxis->pixelToCoord(event->pos().x());

    m_tracer->setGraphKey(x);
    m_tracer->updatePosition();
    double key = m_tracer->position->key();
    QCPGraphDataContainer::const_iterator first = m_graph->data()->constBegin();
    QCPGraphDataContainer::const_iterator last = m_graph->data()->constEnd()-1;
    if(key < first->key || key > last->key){
        return;
    }
    double value = m_graph->data()->at(int(key))->value;
    m_tracer->position->setCoords(key, value);

    QFontMetrics fontMetrics(m_label->font());
    QString text = QString("%1,%2").arg(QString::number(key, 'f', 2)).arg(QString::number(value, 'f', 2));
    QRect textRect = fontMetrics.boundingRect(0, 0, 0, 0, Qt::TextDontClip| m_label->textAlignment(), text);

    QRect textBoxRect = textRect.adjusted(-m_label->padding().left(),
                                            -m_label->padding().top(),
                                            m_label->padding().right(),
                                            m_label->padding().bottom());
    QPoint currentPos = event->pos();
    QRect containRect = m_plot->axisRect()->rect();

    if(currentPos.x() + textRect.width() > containRect.right())
    {
        m_label->SetPostionByTail(true);
        m_label->position->setCoords(-10, 0);
        m_arrow->start->setCoords(-10, 0);
    }
    else
    {
        m_label->SetPostionByTail(false);
        m_label->position->setCoords(10, 0);
        m_arrow->start->setCoords(10, 0);
    }
    
    m_label->setText(text);
    // m_plot->replot();
    m_label->layer()->replot();
}
