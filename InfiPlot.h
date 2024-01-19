#ifndef _INFIPLOT_H
#define _INFIPLOT_H

#include <QWidget>
#include <QToolBar>
#include <QActionGroup>
#include <QPair>

#include <vector>
#include <set>

#include "qcustomplot.h"
namespace Ui {
class InfiPlot;
}

class InfiTracer;
class InfiCutRect;
class InfiEventLine;
class InfiCursorLine;
class InfiBreathRect;
class InfiPlot : public QWidget
{
    Q_OBJECT
public:
enum ActionTypeFlag
{
    atActionCut       = 0x0001,
    atActionZoomIn    = 0x0002,
    atActionZoomOut   = 0x0004,
    atActionReset     = 0x0008,
};
Q_DECLARE_FLAGS(ActionType, ActionTypeFlag)

public:
    explicit InfiPlot(QWidget *parent = nullptr);
    ~InfiPlot();

    QCustomPlot* GetCustomPlot();
    void ClearData();
    void SetData(const QVector<double> &data,
                qint64 startTime,
                quint32 frameInterval = 50);
    void UpdateData(const QVector<double> &values);

    void ShowTimeRange(bool vis);
    void SetIncreaseFactor(double factor);
    qint64 GetTimeMSeconds(double key);
private:
    void IncreaseRange(double facotr);
    void CheckMouseIsIn();
    void SetGraphData(const QVector<double> &data);
    void UpdateGraphData(const QVector<double> &values);
    void SetDataValid(bool valid);
    void SetActionStatus(bool enabled);
    
private:
    void Init();
    void InitToolbar();
    void InitActions();
    void InitPlot();
    void InitTimeRange();
    void InitLayout();

private:
    double PixelXToCoordX(double pixelx);

signals:
    void SigAddEvent(double key);
    void SigCutRangeChanged(double left, double right);
    void SigReset();
    void SigKeyPressed(double key);
    void SigKeyMoved(Qt::MouseButtons button, double key, bool moved = false);
    void SigKeyReleased(double key, bool moved);
    void SigRightClicked(double key);

public:
    Q_SLOT void SlotMouseEnter(QEvent *event);
    Q_SLOT void SlotMouseLeave(QEvent* event);

    Q_SLOT void SlotMousePressEvent(QMouseEvent* mouseEvent);
    Q_SLOT void SlotMouseMoved(QMouseEvent* mouseEvent, bool pressed);
    Q_SLOT void SlotMouseReleaseEvent(QMouseEvent* mouseEvent);
    /**
     * @param moved mouse is moved
    */
    Q_SLOT void SlotMouseClicked(QMouseEvent* mouseEvent, bool moved);    
    Q_SLOT void SlotActionGroupTriggerd(QAction* action);
    Q_SLOT void SlotActionResetTriggered(bool checked);
    Q_SLOT void SlotActionZoomInTriggered(bool checked);
    Q_SLOT void SlotActionZoomOutTriggered(bool checked);
    Q_SLOT void SlotActionMoveChartTriggered(bool checked);
    Q_SLOT void SlotXAxisRangeChanged(QCPRange range);
private:

public: 
    
protected:
    Ui::InfiPlot *ui;
    QToolBar *ui_operToolbar = nullptr;
    QCustomPlot *ui_plotWdg = nullptr;

    QAction* ui_actionReset = nullptr;
    QAction* ui_actionZoomIn = nullptr;
    QAction* ui_actionZoomOut = nullptr;
    QAction* ui_actionMoveChart = nullptr;
    QActionGroup* ui_exclusiveAction = nullptr;
    //for calc press position to x
    QCPItemTracer *ui_helpTracer = nullptr;

    QCPItemText *ui_timeLeft = nullptr;
    QCPItemText *ui_timeRight = nullptr;

    InfiTracer* ui_moveShowTracer = nullptr;

    qint64 _timeStart = 0;
    quint32 _internal = 50;

    double _marginFactor = 0.2;
    bool _timeRangeVisible = true;
    bool _dataValid = false;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(InfiPlot::ActionType)

#endif // _INFIPLOT_H
