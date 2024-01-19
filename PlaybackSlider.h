#ifndef PLAYBACKSLIDER_H
#define PLAYBACKSLIDER_H

#include <QWidget>
#include <QBrush>
#include <QPen>
namespace Ui {
class PlaybackSlider;
}

class PlaybackSlider : public QWidget
{
    Q_OBJECT

public:
    explicit PlaybackSlider(QWidget *parent = nullptr);
    ~PlaybackSlider();

    void SetLength(double len);
    void SetRange(double l, double r);
    void SetCurPos(double cur);

    int GetCurPos();
public:
    Q_SIGNAL void SigRangeChanged(double left, double right);
protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    Ui::PlaybackSlider *ui;
    QBrush ui_backgroundBrush;
    QBrush ui_rangeBrush;
    QBrush ui_handleBrush;
    double _len = 100;
    double _left = 0;
    double _right = 100;
    double _cur = 0;
    int _handleWidth = 8;
    QRect ui_rangeRect;
    QRect ui_handleRect;
    int ui_pressPos;
    QPoint ui_startRange;
    bool ui_pressed = false;
};

#endif // PLAYBACKSLIDER_H
