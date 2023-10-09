#pragma once

#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QStackedWidget>

#include "gui/QStepsStack.h"
#include "sudoku/Sudoku.h"

namespace sudoku
{
    class StepByStepGUI : public QMainWindow
    {
            Q_OBJECT
        public:
            StepByStepGUI(const StepByStepGUI&) = delete;
            StepByStepGUI(StepByStepGUI&&) = delete;
            auto operator=(const StepByStepGUI&) -> StepByStepGUI& = delete;
            auto operator=(StepByStepGUI&&) -> StepByStepGUI& = delete;
            StepByStepGUI(Sudoku* sudoku, const std::array<uint8_t, static_cast<uint8_t>(global::order* global::order)>& initVals, QWidget* /*parent*/);
            ~StepByStepGUI() override = default;

        private:
            Sudoku* sudoku;
            QStepsStack* stepsStack;
            QScrollBar* stepsScrollBar;
            QPushButton* firstButton;
            QPushButton* prevButton;
            QPushButton* nextButton;
            QPushButton* lastButton;

        protected:
            void keyPressEvent(QKeyEvent* event) override;
    };
} // namespace sudoku
