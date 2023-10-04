#include "gui/CandidatesGUI.h"
#include "globals.h"
#include "gui/SolvedGUI.h"

namespace sudoku
{

    CandidatesGUI::CandidatesGUI(Sudoku* sudoku, const std::array<uint8_t, static_cast<uint8_t>(global::order* global::order)>& initVals, QWidget* /*parent*/)
        : sudoku(sudoku)
    {
        constexpr QSize guiDim(512, 512);
        this->setFixedSize(guiDim);
        this->setObjectName("CandidatesGUI");
        this->setWindowTitle(QStringLiteral("Candidates"));
        this->setWindowIcon(QIcon(QStringLiteral(":/res/Qudoku.ico")));

        SolvedGUI::drawGrid(*sudoku->getGrid(), *sudoku->getGrid(), initVals, this);
    }
} // namespace sudoku
