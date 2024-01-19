#include "PlaybackSlider.h"
#include "ui_PlaybackSlider.h"

#include <QPainter>
#include <QDebug>
#include <QMouseEvent>

PlaybackSlider::PlaybackSlider(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlaybackSlider)
{
    ui->setupUi(this);
    ui_backgroundBrush = QBrush(QColor("#7F7F7F"));
    ui_rangeBrush = QBrush(QColor("#FFC90E"));
    ui_handleBrush = QBrush(QColor("#00A2E8"));
}

PlaybackSlider::~PlaybackSlider()
{
    delete ui;
}

void PlaybackSlider::SetLength(double len)
{
    _len = len;
    update();
}


void PlaybackSlider::SetRange(double l, double r)
{
    _left = l;
    _right = r;
    update();
    
}

void PlaybackSlider::SetCurPos(double cur)
{
    _cur = cur;
    update();
}

int PlaybackSlider::GetCurPos()
{
    return _cur;
}

void PlaybackSlider::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    int width = this->width();
    QRect rect = this->rect();
    painter.save();
    painter.setBrush(ui_backgroundBrush);
    painter.drawRect(rect);
    painter.restore();

    painter.save();
    painter.setBrush(ui_rangeBrush);
    int left = (_left / (_len *1.0)) * width;
    int right = (_right / (_len * 1.0)) * width;
    ui_rangeRect = {left, 0, right - left + 1, height()};
    painter.drawRect(ui_rangeRect);
    painter.restore();

    painter.save();

    int curPos = (_cur / (_len * 1.0)) * width;
    int curPosLeft = curPos;
    int curPosRight = curPosLeft + _handleWidth;
    if(curPosRight > width)
    {
        curPosRight = width;
        curPosLeft = curPosRight - _handleWidth;
    }
    ui_handleRect = {curPosLeft, 0, _handleWidth, height()};
    painter.setBrush(ui_handleBrush);
    painter.drawRect(ui_handleRect);
    painter.restore();
}

void PlaybackSlider::mouseMoveEvent(QMouseEvent *event)
{
    if(ui_pressed)
    {
        int currentPos = event->pos().x();
        int diff = ui_pressPos - currentPos;
        int toLeft = ui_startRange.x() - diff;
        int toRight = ui_startRange.y() - diff;
        if(toLeft < 0)
        {
            toLeft = 0;
            toRight = ui_startRange.y() - ui_startRange.x();
        }
        else if(toRight > width())
        {
            toRight = width();
            toLeft = toRight - (ui_startRange.y() - ui_startRange.x());
        }

        double left = (toLeft * 1.0) / width() * _len;
        double right = (toRight * 1.0) / width() * _len;

        emit SigRangeChanged(left, right);
    }
}

void PlaybackSlider::mousePressEvent(QMouseEvent *event)
{
    if(ui_rangeRect.contains(event->pos()))
    {
        ui_pressPos = event->pos().x();
        ui_startRange = {ui_rangeRect.left(), ui_rangeRect.right()};
        ui_pressed = true;
    }
}

void PlaybackSlider::mouseReleaseEvent(QMouseEvent *event)
{
    ui_pressed = false;
}
