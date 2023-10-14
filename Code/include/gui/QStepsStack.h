#pragma once

#include <QtGui/QWheelEvent>
#include <QtWidgets/QStackedWidget>

namespace sudoku
{
    class QStepsStack : public QStackedWidget
    {
            Q_OBJECT
        public:
            explicit QStepsStack(QWidget* parent);
            QStepsStack(const QStepsStack&) = delete;
            QStepsStack(QStepsStack&&) = delete;
            auto operator=(const QStepsStack&) -> QStepsStack& = delete;
            auto operator=(QStepsStack&&) -> QStepsStack& = delete;
            ~QStepsStack() override = default;

        signals:
            void stepChanged(int step);

        protected:
            void wheelEvent(QWheelEvent* event) override;
    };
} // namespace sudoku
