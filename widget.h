#ifndef WIDGET_H
#define WIDGET_H

#include "ui_widget.h"
#include "qcustomplot/qcustomplot.h"

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

public:
    Q_SLOT void SlotXRangeChanged(QCPRange range);
private slots:
    void on_pushButton_clicked();
    void AddCustomPlot();

private:
    Ui::Widget *ui;
    QCustomPlot *customPlot = nullptr;
};
#endif // WIDGET_H
