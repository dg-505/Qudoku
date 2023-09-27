#pragma once

#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>

#include "sudoku/Sudoku.h"

// #include "sudoku/Sudoku.h"

namespace sudoku
{
    class CandidatesGUI : public QMainWindow
    {
        public:
            explicit CandidatesGUI(Sudoku* sudoku, QWidget* parent);
            ~CandidatesGUI() override;

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
