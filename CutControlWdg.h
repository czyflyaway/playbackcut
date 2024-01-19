#ifndef CUTCONTROLWDG_H
#define CUTCONTROLWDG_H

#include <QWidget>

namespace Ui {
class CutControlWdg;
}

class CutControlWdg : public QWidget
{
    Q_OBJECT

public:
    explicit CutControlWdg(QWidget *parent = nullptr);
    ~CutControlWdg();

    void SetButtonSize(const QSize& size);
    void SetRange(double lower, double upper);
    void SetLeft(double left);
    void SetRight(double right);
protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    Ui::CutControlWdg *ui;
    double _lower = 0.0;
    double _upper = 100.0;
    double _left = 0.0;
    double _right = 100.0;
    int _buttonWidth = 80;
    int _buttonHeight = 40;
    QRect _leftButtonRect;
    QRect _rightButtonRect;
    int _currentSelect = 0;//0: no,1:left, 2: right;
    int _pressPos = 0;
    int _pressDif = 0;
};

#endif // CUTCONTROLWDG_H
