#pragma once

#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>

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

            QStackedWidget* stepsStack;

            QPushButton* firstButton;
            QPushButton* prevButton;
            QPushButton* nextButton;
            QPushButton* lastButton;
    };
} // namespace sudoku
