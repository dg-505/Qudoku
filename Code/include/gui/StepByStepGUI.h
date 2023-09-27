#pragma once

#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>

//#include "gui/QInputField.h"
#include "sudoku/Sudoku.h"

// #include "sudoku/Sudoku.hpp"

namespace sudoku
{
    class StepByStepGUI : public QMainWindow
    {
        public:
            StepByStepGUI(Sudoku* sudoku, uint8_t* initVals, QWidget*);
            ~StepByStepGUI();

        private:
            Sudoku* sudoku;

            // QLabel* fields[81];

            QStackedWidget* stepsStack;

            QPushButton* firstButton;
            QPushButton* prevButton;
            QPushButton* nextButton;
            QPushButton* lastButton;

            QFrame* hLine0;
            QFrame* hLine1;
            QFrame* hLine2;
            QFrame* hLine3;
            // QFrame* vLine0;
            QFrame* vLine1;
            QFrame* vLine2;
            // QFrame* vLine3;
    };
} // namespace sudoku
