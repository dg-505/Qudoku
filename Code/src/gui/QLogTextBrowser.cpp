#include "gui/QLogTextBrowser.h"

#include <QtWidgets/QScrollBar>

namespace sudoku
{
    QLogTextBrowser::QLogTextBrowser(QWidget* parent)
        : QTextBrowser(parent)
    {
    }

    void QLogTextBrowser::wheelEvent(QWheelEvent* event)
    {
        if (event->modifiers().testFlag(Qt::ShiftModifier))
        {
            QScrollBar* hScrollBar = horizontalScrollBar();
            if (hScrollBar)
            {
                int const delta = event->angleDelta().y();
                hScrollBar->setValue(hScrollBar->value() - delta);
                event->accept();
                return;
            }
        }
        QTextBrowser::wheelEvent(event);
    }
} // namespace sudoku
