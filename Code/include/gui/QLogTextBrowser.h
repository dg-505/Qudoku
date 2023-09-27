#pragma once

#include <QtGui/QWheelEvent>
#include <QtWidgets/QTextBrowser>

namespace sudoku
{

    class QLogTextBrowser : public QTextBrowser
    {
        public:
            explicit QLogTextBrowser(QWidget* parent = nullptr);

        protected:
            void wheelEvent(QWheelEvent* event) override;
    };

} // namespace sudoku
