#pragma once

#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>

#include "sudoku/Sudoku.h"

namespace sudoku
{
    class SolvedGUI : public QMainWindow
    {
            Q_OBJECT
        public:
            SolvedGUI(const SolvedGUI&) = delete;
            SolvedGUI(SolvedGUI&&) = delete;
            auto operator=(const SolvedGUI&) -> SolvedGUI& = delete;
            auto operator=(SolvedGUI&&) -> SolvedGUI& = delete;
            SolvedGUI(Sudoku* sudoku, const std::array<uint8_t, static_cast<uint8_t>(global::order* global::order)>& initVals, QWidget* /*parent*/);
            ~SolvedGUI() override = default;

            // Helper functions
            static void fillSolvedGrid(const QFont& candsFont, const QFont& fieldsFont, std::array<std::array<Field, global::order>, global::order>& step, const std::array<uint8_t, static_cast<uint8_t>(global::order* global::order)>& initVals, QWidget* parent);
            static void createFrame();

        private:
            Sudoku* sudoku;
    };
} // namespace sudoku
