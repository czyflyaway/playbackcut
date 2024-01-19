#include "InfiPlot.h"
#include "ui_InfiPlot.h"

#include "InfiTracer.h"

#include <algorithm>

InfiPlot::InfiPlot(QWidget *parent) 
    : QWidget(parent)
    , ui(new Ui::InfiPlot)
{
    ui->setupUi(this);
    Init();
}
InfiPlot::~InfiPlot()
{
    delete ui;
}
QCustomPlot* InfiPlot::GetCustomPlot()
{
    return ui_plotWdg;
}
void InfiPlot::ClearData()
{
    ui_plotWdg->layer("overlay")->replot();
    ui_plotWdg->layer("tracerlayer")->replot();
    ui_plotWdg->graph()->data()->clear();
    ui_moveShowTracer->setVisible(false);
    ui_plotWdg->replot();
    ui_plotWdg->setEnabled(false);
    SetDataValid(false);
    SetActionStatus(_dataValid);
}
void InfiPlot::SetData(const QVector<double> &data,
            qint64 startTime,
            quint32 frameInterval)
{
    if (data.size() <= 0)
        return;
    ClearData();

    ui_plotWdg->setEnabled(true);
    SetDataValid(true);
    SetActionStatus(_dataValid);
    _timeStart = startTime;
    _internal = frameInterval;
    SetGraphData(data);
    IncreaseRange(_marginFactor);
    CheckMouseIsIn();
    ui_plotWdg->replot();   
}
void InfiPlot::UpdateData(const QVector<double> &values)
{
    UpdateGraphData(values);
    IncreaseRange(_marginFactor);
    ui_plotWdg->replot();
}
void InfiPlot::ShowTimeRange(bool vis)
{
    _timeRangeVisible = vis;
    ui_timeLeft->setVisible(vis);
    ui_timeRight->setVisible(vis);
}
void InfiPlot::SetIncreaseFactor(double factor)
{
    _marginFactor = factor;
}

qint64 InfiPlot::GetTimeMSeconds(double key)
{
    if(!_dataValid)
    {
        return 0;
    }
    return _timeStart + key * _internal;
}
void InfiPlot::IncreaseRange(double facotr)
{
    QCPRange yRange = ui_plotWdg->yAxis->range();
    double valueRange = yRange.upper - yRange.lower;
    double margin = valueRange * facotr;
    yRange.upper += margin;
    yRange.lower -= margin;
    ui_plotWdg->yAxis->setRange(yRange);
}
void InfiPlot::CheckMouseIsIn()
{
    QPoint currentMousePos = QCursor::pos();
    QPoint plotCurrentTopLeftPos = ui_plotWdg->mapToGlobal(QPoint(0, 0));
    QRect plotRect = QRect(plotCurrentTopLeftPos, ui_plotWdg->size());
    if(plotRect.contains(currentMousePos))
        ui_moveShowTracer->setVisible(true);
    else
        ui_moveShowTracer->setVisible(false);
    ui_moveShowTracer->updatePosition(0, ui_plotWdg->graph()->data()->at(int(0))->value);
}
void InfiPlot::SetGraphData(const QVector<double> &data)
{
    QVector<double> keys;
    for (int i = 0; i < data.size(); ++i)
    {
        keys.push_back(i);
    }
    ui_plotWdg->graph()->setData(keys, data, true);
    ui_plotWdg->graph()->rescaleAxes();
}
void InfiPlot::UpdateGraphData(const QVector<double> &values)
{
    auto graphData = ui_plotWdg->graph()->data();
    int i = 0;
    for(auto iter = graphData->begin();
        iter != graphData->end() && i < values.size();
        ++iter, ++i)
    {
        iter->value = values.at(i);
    }
    ui_plotWdg->graph()->rescaleValueAxis();
}
void InfiPlot::SetDataValid(bool valid)
{
    _dataValid = valid;
}
void InfiPlot::SetActionStatus(bool enabled)
{
    if(ui_actionReset) ui_actionReset->setEnabled(enabled);
    if(ui_actionZoomIn) ui_actionZoomIn->setEnabled(enabled);
    if(ui_actionZoomOut) ui_actionZoomOut->setEnabled(enabled);
    if(ui_actionMoveChart) ui_actionMoveChart->setEnabled(enabled);
}
void InfiPlot::Init()
{
    InitToolbar();
    InitActions();
    InitPlot();
    InitTimeRange();
    ShowTimeRange(true);
    InitLayout();
}
void InfiPlot::InitToolbar()
{
    ui_operToolbar = new QToolBar(this);
    ui_operToolbar->setObjectName("OperToolbar");
    ui_operToolbar->setOrientation(Qt::Horizontal);
    ui_operToolbar->setFixedHeight(40);
    ui_operToolbar->setStyleSheet(
        "QToolBar#OperToolbar{background-color:#D9D9D9; border: 1px solid #A9A9A9;}");
    connect(ui_operToolbar, &QToolBar::actionTriggered, 
            this, &InfiPlot::SlotActionGroupTriggerd);
}
void InfiPlot::InitActions()
{
    ui_exclusiveAction = new QActionGroup(this);
    ui_exclusiveAction->setExclusive(true);

    ui_actionZoomIn = ui_operToolbar->addAction(tr("ZoomIn"));
    ui_actionZoomOut = ui_operToolbar->addAction(tr("ZoomOut"));
    ui_actionReset = ui_operToolbar->addAction(tr("Reset"));
    ui_actionMoveChart = ui_operToolbar->addAction(tr("Move"));
    ui_exclusiveAction->addAction(ui_actionReset);
    ui_exclusiveAction->addAction(ui_actionZoomIn);
    ui_exclusiveAction->addAction(ui_actionZoomOut);
    ui_exclusiveAction->addAction(ui_actionMoveChart);
    connect(ui_actionZoomIn, &QAction::triggered,
            this, &InfiPlot::SlotActionZoomInTriggered);
    connect(ui_actionZoomOut, &QAction::triggered,
            this, &InfiPlot::SlotActionZoomOutTriggered);
    connect(ui_actionMoveChart, &QAction::triggered,
            this, &InfiPlot::SlotActionMoveChartTriggered);

    SetActionStatus(_dataValid);
}
void InfiPlot::InitPlot()
{
    ui_plotWdg = new QCustomPlot(this);
    ui_plotWdg->setEnabled(false);
    //tracerlayer for show coord when move
    ui_plotWdg->addLayer("tracerlayer");
    ui_plotWdg->layer("tracerlayer")->setMode(QCPLayer::lmBuffered);

    ui_plotWdg->setInteractions(QCP::iSelectItems |
                                QCP::iRangeDrag |
                                QCP::iRangeZoom |
                                QCP::iSelectPlottables);

    ui_plotWdg->setBackground(QBrush(QColor("#D9D9D9")));
    ui_plotWdg->axisRect()->setAutoMargins(QCP::MarginSide::msNone);
    ui_plotWdg->axisRect()->setMargins(QMargins(5, 10, 5, 20));
    ui_plotWdg->axisRect()->setRangeZoom(Qt::Horizontal/*  | Qt::Vertical */);
    ui_plotWdg->axisRect()->setRangeDrag(Qt::Horizontal/*  | Qt::Vertical */);

    ui_plotWdg->xAxis->setRangeFilter(true);
    ui_plotWdg->xAxis->grid()->setVisible(false);
    ui_plotWdg->yAxis->grid()->setVisible(false);
    ui_plotWdg->xAxis->setTicks(false);
    ui_plotWdg->yAxis->setTicks(false);

    QCPGraph* graph =  ui_plotWdg->addGraph();
    graph->setPen(QPen(Qt::black));
    graph->selectionDecorator()->setPen(QPen(Qt::black));

    ui_moveShowTracer = new InfiTracer(ui_plotWdg, graph);
    connect(ui_plotWdg, &QCustomPlot::mouseMove,
            ui_moveShowTracer, &InfiTracer::SlotMouseMoveEvent);

    // 只是辅助,用来获取对应的frameIndex
    ui_helpTracer = new QCPItemTracer(ui_plotWdg);
    ui_helpTracer->setGraph(graph);
    ui_helpTracer->setPen(QPen(Qt::NoPen));
    ui_helpTracer->setStyle(QCPItemTracer::tsNone);
    // 调用此函数不进行插值
    // QCPItemTracer::updatePosition()中会自动搜索一个距离形参x最近的采样点的x
    ui_helpTracer->setInterpolating(false);

    connect(ui_plotWdg->xAxis, SIGNAL(rangeChanged(QCPRange)), 
            this, SLOT(SlotXAxisRangeChanged(QCPRange)));
    connect(ui_plotWdg, &QCustomPlot::mousePress, this, &InfiPlot::SlotMousePressEvent);
    connect(ui_plotWdg, &QCustomPlot::mouseRelease, this, &InfiPlot::SlotMouseReleaseEvent);
    connect(ui_plotWdg, &QCustomPlot::mouseMoved, this, &InfiPlot::SlotMouseMoved);
    connect(ui_plotWdg, &QCustomPlot::mouseClicked, this, &InfiPlot::SlotMouseClicked);
    connect(ui_plotWdg, &QCustomPlot::mouseEnter, this, &InfiPlot::SlotMouseEnter);
    connect(ui_plotWdg, &QCustomPlot::mouseLeave, this, &InfiPlot::SlotMouseLeave);

}
void InfiPlot::InitTimeRange()
{
    QMargins axisRectMargins = ui_plotWdg->axisRect()->margins();
    ui_timeLeft = new QCPItemText(ui_plotWdg);
    ui_timeLeft->setClipToAxisRect(false);
    ui_timeLeft->position->setType(QCPItemPosition::ptViewportRatio);
    ui_timeLeft->setPositionAlignment(Qt::AlignLeft | Qt::AlignBottom);
    ui_timeLeft->position->setCoords(0.0, 1.0); // lower right corner of axis rect
    ui_timeLeft->setText("00:00:00.000");
    ui_timeLeft->setTextAlignment(Qt::AlignLeft);
    ui_timeLeft->setFont(QFont("Microsoft YaHei", 9));
    ui_timeLeft->setPadding(QMargins(axisRectMargins.left(), 0, 0, 5));

    ui_timeRight = new QCPItemText(ui_plotWdg);
    ui_timeRight->setClipToAxisRect(false);
    ui_timeRight->position->setType(QCPItemPosition::ptViewportRatio);
    ui_timeRight->setPositionAlignment(Qt::AlignRight | Qt::AlignBottom);
    ui_timeRight->position->setCoords(1.0, 1.0); // lower right corner of axis rect
    ui_timeRight->setText("00:00:00.000");
    ui_timeRight->setTextAlignment(Qt::AlignLeft);
    ui_timeRight->setFont(QFont("Microsoft YaHei", 9));
    ui_timeRight->setPadding(QMargins(0, 0, axisRectMargins.right(), 5));
}

void InfiPlot::InitLayout()
{
    ui->mainLayout->addWidget(ui_operToolbar);
    ui->mainLayout->addWidget(ui_plotWdg);
}

double InfiPlot::PixelXToCoordX(double pixelx)
{
    double x = ui_plotWdg->xAxis->pixelToCoord(pixelx); // 鼠标点的像素坐标转plot坐标
    ui_helpTracer->setGraphKey(x);
    ui_helpTracer->updatePosition();
    return ui_helpTracer->position->key();
}

Q_INVOKABLE void InfiPlot::SlotActionGroupTriggerd(QAction *action)
{
    if (action == ui_actionReset){}
    else if (action == ui_actionZoomIn){}
    else if (action == ui_actionZoomOut){}
    else if (action == ui_actionReset){}
}

void InfiPlot::SlotMousePressEvent(QMouseEvent *event)
{
    if(event->modifiers().testFlag(Qt::ControlModifier))
        return;
    if(!_dataValid)
        return;
    double key = PixelXToCoordX(event->pos().x());

    auto first = ui_plotWdg->graph()->data()->constBegin();
    auto last = ui_plotWdg->graph()->data()->constEnd() - 1;
    if (key >= first->key && key < last->key)
    {
        if(Qt::LeftButton == event->button())
        {
            emit SigKeyPressed(key);
        }
    }
}

void InfiPlot::SlotMouseMoved(QMouseEvent *event, bool pressed)
{
    if(event->modifiers().testFlag(Qt::ControlModifier))
        return;
    if(!_dataValid)
        return;
    double key = PixelXToCoordX(event->pos().x());
    QCPGraphDataContainer::const_iterator first = ui_plotWdg->graph()->data()->constBegin();
    QCPGraphDataContainer::const_iterator last = ui_plotWdg->graph()->data()->constEnd() - 1;
    if (key >= first->key && key < last->key)
    {   
        emit SigKeyMoved(event->buttons(), key, pressed);
    }    
}

void InfiPlot::SlotMouseReleaseEvent(QMouseEvent *event)
{
}

void InfiPlot::SlotMouseClicked(QMouseEvent *event, bool moved)
{
    if(!_dataValid)
        return;

    double key = PixelXToCoordX(event->pos().x());

    auto first = ui_plotWdg->graph()->data()->constBegin();
    auto last = ui_plotWdg->graph()->data()->constEnd() - 1;
    if (key >= first->key && key < last->key)
    {
        if(Qt::RightButton == event->button())
        {
            //TODO:将pos带出去
            QPoint pos= event->pos();
            emit SigRightClicked(key);
            return;
        }
        emit SigKeyReleased(key, moved);
    }
}

void InfiPlot::SlotXAxisRangeChanged(QCPRange range)
{
    if(!_dataValid)
        return;
    if (_timeRangeVisible)
    {
        auto msecondsToTimeText = [](qint64 time){
            return QDateTime::fromMSecsSinceEpoch(time).toString("hh:mm:ss.zzz");
        };
        qint64 time = _timeStart + range.lower * _internal;
        ui_timeLeft->setText(msecondsToTimeText(time));
        time = _timeStart + range.upper * _internal;
        ui_timeRight->setText(msecondsToTimeText(time));
    }
}

void InfiPlot::SlotActionResetTriggered(bool checked)
{
    ui_plotWdg->rescaleAxes();
    IncreaseRange(_marginFactor);
}

void InfiPlot::SlotActionZoomInTriggered(bool checked)
{
}

void InfiPlot::SlotActionZoomOutTriggered(bool checked)
{
}

void InfiPlot::SlotActionMoveChartTriggered(bool checked)
{
}

void InfiPlot::SlotMouseEnter(QEvent *event)
{
    if(ui_moveShowTracer && _dataValid)
    {
        ui_moveShowTracer->setVisible(true);
    }
}

void InfiPlot::SlotMouseLeave(QEvent *event)
{
    if(ui_moveShowTracer)
    {
        ui_moveShowTracer->setVisible(false);
    }
}
