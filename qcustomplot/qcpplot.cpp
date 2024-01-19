#include "qcpplot.h"

QCPPlot::QCPPlot(QWidget *parent)
    : QCustomPlot(parent)
    , mSizeHandleManager(new QCPSizeHandleManager(this))
{

}

void QCPPlot::mousePressEvent(QMouseEvent *event)
{
    mMousePressPos = event->pos();
    mMouseHasMoved = false;
    mLastMousePos = mMousePressPos;

    if (Qt::LeftButton == event->button()) {
        processPointSelection(event);
        if (selectedItems().count()) {
            emit mousePress(event);
            replot(rpQueuedReplot);
            return;
        }
    }
    else if(Qt::RightButton == event->button())
    {
        emit SigRightButtonClicked(event);
        return;
    }

    QCustomPlot::mousePressEvent(event);
}

void QCPPlot::mouseMoveEvent(QMouseEvent *event)
{
    QCustomPlot::mouseMoveEvent(event);

    if (mMouseHasMoved && (event->buttons() & Qt::LeftButton)) {
        auto items = selectedItems();
        for (auto item : items) {
            if (auto sizeHandle = qobject_cast<QCPSizeHandle*>(item))
                mSizeHandleManager->onItemResize(sizeHandle, event->pos() - mLastMousePos);
            else
            {
                mSizeHandleManager->onItemMove(item, event->pos() - mLastMousePos);
            }
        }

        mLastMousePos = event->pos();
        if (items.count())
            replot(rpQueuedReplot);
    }
}

void QCPPlot::mouseReleaseEvent(QMouseEvent *event)
{
    QCustomPlot::mouseReleaseEvent(event);
    if (event->button() == Qt::LeftButton) {
        if (auto item = itemAt(event->pos(), true)) {
            // item->setSelected(false);
            item->layer()->setItemToTop(item);
            replot(rpQueuedReplot);
        }
    }
}
