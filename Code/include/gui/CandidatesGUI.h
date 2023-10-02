#pragma once

#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>

#include "sudoku/Sudoku.h"

namespace sudoku
{
    class CandidatesGUI : public QMainWindow
    {
            Q_OBJECT
        public:
            CandidatesGUI(const CandidatesGUI&) = delete;
            CandidatesGUI(CandidatesGUI&&) = delete;
            auto operator=(const CandidatesGUI&) -> CandidatesGUI& = delete;
            auto operator=(CandidatesGUI&&) -> CandidatesGUI& = delete;
            explicit CandidatesGUI(Sudoku* sudoku, QWidget* /*parent*/);
            ~CandidatesGUI() override = default;

        private:
            Sudoku* sudoku;
    };
} // namespace sudoku
