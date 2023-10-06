#include "gui/QStepsStack.h"
#include <QtGui/QWheelEvent>

namespace sudoku
{
    QStepsStack::QStepsStack(QWidget* parent)
        : QStackedWidget(parent) {}

    void QStepsStack::wheelEvent(QWheelEvent* event)
    {
        if (event->angleDelta().y() > 0)
        {
            this->setCurrentIndex(this->currentIndex() - 1);
        }
        else
        {
            this->setCurrentIndex(this->currentIndex() + 1);
        }
    }
} // namespace sudoku