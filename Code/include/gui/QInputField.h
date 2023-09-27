#pragma once

#include <QtGui/QKeyEvent>
#include <QtWidgets/QLineEdit>

namespace sudoku
{
    class QInputField : public QLineEdit
    {
        public:
            QInputField(QWidget* parent = nullptr);

        protected:
            void keyPressEvent(QKeyEvent* event) override;
            void contextMenuEvent(QContextMenuEvent* event) override;
    };
} // namespace sudoku
