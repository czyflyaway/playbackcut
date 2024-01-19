#include "CutControlWdg.h"
#include "ui_CutControlWdg.h"

#include <QMouseEvent>
#include <QPainter>

CutControlWdg::CutControlWdg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CutControlWdg)
{
    ui->setupUi(this);
    this->setMinimumHeight(2 * _buttonHeight);
    this->setMaximumHeight(2 * _buttonHeight);
}

CutControlWdg::~CutControlWdg()
{
    delete ui;
}

void CutControlWdg::SetMaxMinRange(double minLeft, double maxRight)
{
    _minLeft = minLeft;
    _maxRight = maxRight;
}

void CutControlWdg::SetButtonSize(const QSize &size)
{
    _buttonWidth = size.width();
    _buttonHeight = size.height();
    this->setMinimumHeight(2 * _buttonHeight);
    update();
}


void CutControlWdg::SetRange(double lower, double upper)
{
    _lower = lower;
    _upper = upper;
    update();
}

void CutControlWdg::SetLeft(double left)
{
    _left = left;
    emit SigLeftChanged(_left);
    update();
}

void CutControlWdg::SetRight(double right)
{
    _right = right;
    emit SigRightChanged(_right);
    update();
}

void CutControlWdg::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.save();
    painter.fillRect(rect(),Qt::red);
    painter.restore();

    int usedWidth = this->width() - _buttonWidth;
    int start = _buttonWidth / 2;
    int end = this->width() - _buttonWidth / 2;

    int leftPos = (_left - _lower) * usedWidth / (_upper - _lower);
    int rightPos = (_right - _lower) * usedWidth / (_upper - _lower);

    _leftButtonRect = QRect(start + leftPos - _buttonWidth / 2, 0,
                            _buttonWidth, _buttonHeight - 2);
    _rightButtonRect = QRect(rightPos, _buttonHeight,
                            _buttonWidth, _buttonHeight - 2);
    {
        painter.save();
        painter.setPen(QPen(Qt::NoPen));
        painter.setBrush(QBrush(QColor("#00ffff")));
        painter.drawRect(_leftButtonRect);
        painter.restore();
        painter.save();
        QFont font = QFont("Arial");
        font.setPixelSize(12);
        painter.setFont(font);
        painter.drawText(_leftButtonRect, Qt::AlignCenter, _left < _right ? tr("left") : tr("right"));
        painter.restore();
    }
    {
        painter.save();
        painter.setPen(QPen(Qt::NoPen));
        painter.setBrush(QBrush(QColor("#00ffff")));
        painter.drawRect(_rightButtonRect);
        painter.restore();
        painter.save();
        QFont font = QFont("Arial");
        font.setPixelSize(12);
        painter.setFont(font);
        painter.setPen(Qt::black);
        painter.drawText(_rightButtonRect, Qt::AlignCenter, _right < _left ? tr("left") : tr("right"));
        painter.restore();
    }
}

void CutControlWdg::mouseMoveEvent(QMouseEvent *event)
{
    if(1 == _currentSelect)
    {
        int diff = event->pos().x() - _pressPos;

        int currentPos = _pressPos + diff - _pressDif;
        int usedWidth = this->width() - _buttonWidth;
        _left = currentPos * (_upper - _lower) / usedWidth + _lower;
        emit SigLeftChanged(_left);
        if(_left < _minLeft)
        {
            _left = _minLeft;
        }
        update();
    }
    else if(2 == _currentSelect)
    {
        int diff = event->pos().x() - _pressPos;
        int currentPos = _pressPos + diff - _pressDif;
        int usedWidth = this->width() - _buttonWidth;
        _right = currentPos * (_upper - _lower) / usedWidth + _lower;
        if(_right > _maxRight)
        {
            _right = _maxRight;
        }
        emit SigRightChanged(_right);
        update();
    }
}

void CutControlWdg::mousePressEvent(QMouseEvent *event)
{
    if(_leftButtonRect.contains(event->pos()))
    {
        _currentSelect = 1;
        _pressPos = _leftButtonRect.left();
        _pressDif = event->pos().x() - _pressPos;
    }
    else if(_rightButtonRect.contains(event->pos()))
    {
        _currentSelect = 2;
        _pressPos = _rightButtonRect.left();
        _pressDif = event->pos().x() - _pressPos;
    }
    else
        _currentSelect = 0;


}

void CutControlWdg::mouseReleaseEvent(QMouseEvent *event)
{

}
