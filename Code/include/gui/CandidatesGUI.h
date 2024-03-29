#pragma once

#include <QMainWindow>

#include "sudoku/Sudoku.h"

namespace sudoku
{
    class CandidatesGUI : public QMainWindow
    {
            Q_OBJECT // NOLINT
        public:
            CandidatesGUI(const CandidatesGUI&) = delete;
            CandidatesGUI(CandidatesGUI&&) = delete;
            auto operator=(const CandidatesGUI&) -> CandidatesGUI& = delete;
            auto operator=(CandidatesGUI&&) -> CandidatesGUI& = delete;
            CandidatesGUI(Sudoku* sudoku, const std::string& name, const std::array<uint8_t, static_cast<uint8_t>(global::order* global::order)>& initVals, QWidget* parent);
            ~CandidatesGUI() override = default;

        protected:
            void keyPressEvent(QKeyEvent* event) override;
    };
} // namespace sudoku
