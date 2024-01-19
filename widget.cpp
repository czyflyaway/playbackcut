#include "widget.h"

#include <QPen>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    customPlot = ui->chart->GetCustomPlot();

//    customPlot->xAxis->setRangeFilter(true);
//    customPlot->axisRect()->setRangeZoom(Qt::Horizontal/*  | Qt::Vertical */);
//    customPlot->axisRect()->setRangeDrag(Qt::Horizontal/*  | Qt::Vertical */);
//    customPlot->axisRect()->setAutoMargins(QCP::MarginSide::msNone);
//    customPlot->axisRect()->setMargins(QMargins(100, 0, 100, 60));

//    {
//        // add two new graphs and set their look:
//        customPlot->addGraph();
//        customPlot->graph(0)->setPen(QPen(Qt::blue)); // line color blue for first graph
//        customPlot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
//        //  customPlot->addGraph();
//        //  customPlot->graph(1)->setPen(QPen(Qt::red)); // line color red for second graph
//        // generate some points of data (y0 for first, y1 for second graph):
//        QVector<double> x(251), y0(251), y1(251);
//        for (int i=0; i<251; ++i)
//        {
//            x[i] = i;
//            y0[i] = qExp(-i/150.0)*qCos(i/10.0) + 3; // exponentially decaying cosine
//            //    y1[i] = qExp(-i/150.0) + 3;              // exponential envelope
//        }
//        // configure right and top axis to show ticks but no labels:
//        // (see QCPAxisRect::setupFullAxesBox for a quicker method to do this)
//        customPlot->xAxis2->setVisible(false);
//        customPlot->xAxis2->setTickLabels(false);
//        customPlot->yAxis2->setVisible(false);
//        customPlot->yAxis2->setTickLabels(false);
//        // make left and bottom axes always transfer their ranges to right and top axes:
//        connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
//        connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));
//        // pass data points to graphs:
//        customPlot->graph(0)->setData(x, y0);
//        //  customPlot->graph(1)->setData(x, y1);
//        // let the ranges scale themselves so graph 0 fits perfectly in the visible area:
//        customPlot->graph(0)->rescaleAxes();
//        QCPRange range = customPlot->xAxis->range();
//        qDebug() << "range:" << (int)range.lower << "," << (int)range.upper;
//        QCPRange yRange = customPlot->yAxis->range();

//        double valueRange = yRange.upper - yRange.lower;
//        double margin = valueRange * 0.5;
//        yRange.upper += margin;
//        yRange.lower -= margin;
//        customPlot->yAxis->setRange(yRange);
//        qDebug() << "size:" << customPlot->graph(0)->data()->size();

//        // same thing for graph 1, but only enlarge ranges (in case graph 1 is smaller than graph 0):
//        //  customPlot->graph(1)->rescaleAxes(true);
//        // Note: we could have also just called customPlot->rescaleAxes(); instead
//        // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
//        //  customPlot->setSelectionRectMode(QCP::SelectionRectMode::srmZoom);
//        customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables | QCP::iSelectItems);
//        customPlot->axisRect()->setRangeZoom(Qt::Horizontal/*  | Qt::Vertical */);
//        customPlot->axisRect()->setRangeDrag(Qt::Horizontal/*  | Qt::Vertical */);
//    }


//    ui->slider->SetLength(250);
//    ui->slider->SetRange(0, 250);

    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(SlotXRangeChanged(QCPRange)));
    connect(ui->slider, &PlaybackSlider::SigRangeChanged, this, [=](double left, double right){
        customPlot->xAxis->setRange(left, right);
        customPlot->replot();
    });
}

Widget::~Widget()
{
    delete ui;
}

void Widget::SlotXRangeChanged(QCPRange range)
{
    qDebug() << "SlotXRangeChanged-----:";
    qDebug() << "range:" << range.lower << "," << range.upper;
    double l = range.lower < 0 ? 0 : range.lower;
    double r = range.upper > 250 ? 250 : range.upper;
//    customPlot->xAxis->setRange(l, r);
    ui->slider->SetRange(l, r);
    qDebug() << "range:" << range.lower << "," << range.upper;
    qDebug() << "-----SlotXRangeChanged:";
}


void Widget::on_pushButton_clicked()
{
//    QCPRange range = customPlot->xAxis->range();
//    qDebug() << "on_pushButton_clicked:range:" << range.lower << "," << range.upper;
//    ui->slider->SetCurPos(ui->slider->GetCurPos() + 1);


//    QCPItemLine *leftLine = new QCPItemLine(customPlot);
//    leftLine->setClipToAxisRect(false);
//    leftLine->setHead(QCPLineEnding::esSquare);
////    leftLine->start->setType(QCPItemPosition::ptViewportRatio);
//    QCPRange yRange = customPlot->yAxis->range();
//    leftLine->setSelectable(true);
//    leftLine->setSelectedPen(QPen(Qt::blue));
//    leftLine->start->setCoords(10, yRange.upper);
//    leftLine->end->setCoords(10, yRange.lower - 0.1);
//    leftLine->setPen(QPen(QBrush(QColor(Qt::red)), 3));
//    QCPItemLine *rightLine = new QCPItemLine(customPlot);
//    rightLine->setHead(QCPLineEnding::esSquare);
//    rightLine->start->setCoords(100, yRange.upper);
//    rightLine->end->setCoords(100, yRange.lower);
//    rightLine->setPen(QPen(Qt::red));
//    customPlot->replot();




}

void Widget::AddCustomPlot()
{


}

