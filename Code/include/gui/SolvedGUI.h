#pragma once

#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>

//#include "gui/QInputField.h"
#include "sudoku/Sudoku.h"

// #include "sudoku/Sudoku.hpp"

namespace sudoku
{
    class SolvedGUI : public QMainWindow
    {
        public:
            SolvedGUI(Sudoku* sudoku, uint8_t* initVals, QWidget*);
            ~SolvedGUI();

        private:
            Sudoku* sudoku;

            // QLabel* fields[81];

            // QFrame* hLine0;
            QFrame* hLine1;
            QFrame* hLine2;
            // QFrame* hLine3;
            // QFrame* vLine0;
            QFrame* vLine1;
            QFrame* vLine2;
            // QFrame* vLine3;
    };
} // namespace sudoku
