#pragma once

#include <QKeyEvent>
#include <QLineEdit>

namespace sudoku
{
    class QInputField : public QLineEdit
    {
            Q_OBJECT // NOLINT
        public:
            explicit QInputField(QWidget* parent);
            QInputField(const QInputField&) = delete;
            QInputField(QInputField&&) = delete;
            auto operator=(const QInputField&) -> QInputField& = delete;
            auto operator=(QInputField&&) -> QInputField& = delete;
            ~QInputField() override = default;

        protected:
            void keyPressEvent(QKeyEvent* event) override;
            void contextMenuEvent(QContextMenuEvent* /*event*/) override;
    };
} // namespace sudoku
