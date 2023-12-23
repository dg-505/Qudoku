#include "globals.h"
#include "gui/CandidatesGUI.h"
#include "gui/SolvedGUI.h"

namespace sudoku
{
    CandidatesGUI::CandidatesGUI(Sudoku* sudoku, const std::array<uint8_t, static_cast<uint8_t>(global::order* global::order)>& initVals, QWidget* parent)
        : QMainWindow(parent, Qt::WindowFlags()), _sudoku(sudoku)
    {
        constexpr QSize guiDim(537, 537);
        this->setFixedSize(guiDim);
        this->setObjectName("CandidatesGUI");
        this->setWindowTitle(QStringLiteral("Candidates"));
        this->setWindowIcon(QIcon(QStringLiteral(":/res/Qudoku.ico")));
        this->setStyleSheet(QStringLiteral("background: rgb(239, 239, 239)"));

        sudoku->getSteps()->emplace_back(*sudoku->getGrid(), std::vector<Field*>{}, std::vector<uint8_t>{}, 0, "Initial status");
        SolvedGUI::drawFields(sudoku->getSteps()->front(), sudoku->getSteps()->front(), initVals, this);
        SolvedGUI::drawFrame(this);
    }

    void CandidatesGUI::keyPressEvent(QKeyEvent* event)
    {
        if (event->key() == Qt::Key_Escape)
        {
            this->close();
        }
    }
} // namespace sudoku
