#include <QScrollBar>

#include "gui/QLogTextBrowser.h"

namespace sudoku
{
    QLogTextBrowser::QLogTextBrowser(QWidget* parent)
        : QTextBrowser(parent) {}

    void QLogTextBrowser::wheelEvent(QWheelEvent* event)
    {
        if (event->modifiers().testFlag(Qt::ShiftModifier))
        {
            QScrollBar* hScrollBar = horizontalScrollBar();
            if (hScrollBar != nullptr)
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
