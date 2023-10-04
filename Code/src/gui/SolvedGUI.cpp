#include "gui/SolvedGUI.h"
#include "globals.h"

namespace sudoku
{

    SolvedGUI::SolvedGUI(Sudoku* sudoku, const std::array<uint8_t, static_cast<uint8_t>(global::order* global::order)>& initVals, QWidget* /*parent*/)
        : sudoku(sudoku)
    {
        constexpr QSize guiDim(512, 512);
        this->setFixedSize(guiDim);
        this->setObjectName("SolvedGUI");
        this->setWindowTitle(QStringLiteral("Solved Sudoku"));
        this->setWindowIcon(QIcon(QStringLiteral(":/res/Qudoku.ico")));

        SolvedGUI::drawGrid(*sudoku->getGrid(), *sudoku->getGrid(), initVals, this);
    }

    // Helper functions
    void SolvedGUI::drawGrid(std::array<std::array<sudoku::Field, global::order>, global::order>& step, std::array<std::array<sudoku::Field, global::order>, global::order>& nextStep, const std::array<uint8_t, static_cast<uint8_t>(global::order* global::order)>& initVals, QWidget* parent)
    {
        const QFont fieldsFont(QStringLiteral("Liberation Mono"), 32, QFont::Bold);
        const QFont candsFont(QStringLiteral("Liberation Mono"), 14, QFont::Bold);

        uint8_t fID = 1;
        uint16_t posY = 0;
        for (uint8_t rID = 1; rID <= global::order; rID++)
        {
            uint16_t posX = 0;
            for (uint8_t cID = 1; cID <= global::order; cID++)
            {
                auto* field = new QLabel(parent);
                field->setObjectName("field" + QString::number(fID));
                const QRect fieldGeom(posX, posY, global::fieldDim, global::fieldDim);
                field->setGeometry(fieldGeom);
                field->setFont(fieldsFont);
                field->setStyleSheet(QStringLiteral("color: black; background-color: rgba(239, 239, 239, 1.0)"));
                field->setAlignment(Qt::AlignCenter);
                field->setFrameShape(QFrame::Panel);
                const uint8_t val = *step.at(rID - 1).at(cID - 1).getVal();
                // If field is solved => fill value
                if (val != 0)
                {
                    field->setText(QString::number(val));
                    // If field was NOT given initially, but is solved now => set color green
                    if (initVals.at(fID - 1) == 0)
                    {
                        field->setStyleSheet(QStringLiteral("color: rgb(20,160,50); background-color: rgba(239, 239, 239, 1.0); border: 1px solid black"));
                    }
                }
                else
                {
                    uint16_t candY = posY + 1;
                    uint8_t candI = 1;
                    for (uint8_t candR = 1; candR <= 3; candR++)
                    {
                        uint16_t candX = posX + 1;
                        for (uint8_t candR = 1; candR <= 3; candR++)
                        {
                            auto* cand = new QLabel(parent);
                            const QRect candGeom(candX, candY, 17, 17);
                            cand->setGeometry(candGeom);
                            cand->setAlignment(Qt::AlignCenter);
                            cand->setFont(candsFont);
                            cand->setStyleSheet(QStringLiteral("color: rgb(20,50,255)"));

                            auto* cands = step.at(rID - 1).at(cID - 1).getCandidates();
                            if (std::find(cands->begin(), cands->end(), candI) != cands->end())
                            {
                                cand->setText(QString::number(candI));
                                // If candI disappears in the next step => make background of cand red
                                if (std::find(nextStep.at(rID - 1).at(cID - 1).getCandidates()->begin(), nextStep.at(rID - 1).at(cID - 1).getCandidates()->end(), candI) == nextStep.at(rID - 1).at(cID - 1).getCandidates()->end() &&
                                    *nextStep.at(rID - 1).at(cID - 1).getVal() == 0)
                                {
                                    cand->setStyleSheet(QStringLiteral("color: rgb(20,50,255); background-color: rgba(255, 50, 10, 1.0); border: 1px solid black"));
                                }
                            }
                            else
                            {
                                cand->setText(QStringLiteral(""));
                            }
                            candI++;
                            candX += global::candDim;
                        }
                        candY += global::candDim;
                    }
                }
                fID++;
                posX += global::fieldDim;
                if (cID % 3 == 0)
                {
                    posX += 4;
                }
            }
            posY += global::fieldDim;
            if (rID % 3 == 0)
            {
                posY += 4;
            }
        }

        // Frame for the grid
        auto* hLine0 = new QFrame(parent);
        hLine0->setObjectName("hLine0");
        constexpr QRect hLine0Geom(0, 0, 512, 2);
        hLine0->setGeometry(hLine0Geom);
        hLine0->setLineWidth(2);
        hLine0->setFrameShape(QFrame::HLine);
        hLine0->setStyleSheet(QStringLiteral("color: black"));
        auto* hLine1 = new QFrame(parent);
        hLine1->setObjectName("hLine1");
        constexpr QRect hLine1Geom(0, 169, 512, 2);
        hLine1->setGeometry(hLine1Geom);
        hLine1->setLineWidth(2);
        hLine1->setFrameShape(QFrame::HLine);
        hLine1->setStyleSheet(QStringLiteral("color: black"));
        auto* hLine2 = new QFrame(parent);
        hLine2->setObjectName("hLine2");
        constexpr QRect hLine2Geom(0, 341, 512, 2);
        hLine2->setGeometry(hLine2Geom);
        hLine2->setLineWidth(2);
        hLine2->setFrameShape(QFrame::HLine);
        hLine2->setStyleSheet(QStringLiteral("color: black"));
        auto* hLine3 = new QFrame(parent);
        hLine3->setObjectName("hLine3");
        constexpr QRect hLine3Geom(0, 510, 512, 2);
        hLine3->setGeometry(hLine3Geom);
        hLine3->setLineWidth(2);
        hLine3->setFrameShape(QFrame::HLine);
        hLine3->setStyleSheet(QStringLiteral("color: black"));
        auto* vLine0 = new QFrame(parent);
        vLine0->setObjectName("vLine0");
        constexpr QRect vLine0Geom(0, 0, 2, 512);
        vLine0->setGeometry(vLine0Geom);
        vLine0->setLineWidth(2);
        vLine0->setFrameShape(QFrame::VLine);
        vLine0->setStyleSheet(QStringLiteral("color: black"));
        auto* vLine1 = new QFrame(parent);
        vLine1->setObjectName("vLine1");
        constexpr QRect vLine1Geom(169, 0, 2, 512);
        vLine1->setGeometry(vLine1Geom);
        vLine1->setLineWidth(2);
        vLine1->setFrameShape(QFrame::VLine);
        vLine1->setStyleSheet(QStringLiteral("color: black"));
        auto* vLine2 = new QFrame(parent);
        vLine2->setObjectName("vLine2");
        constexpr QRect vLine2Geom(341, 0, 2, 512);
        vLine2->setGeometry(vLine2Geom);
        vLine2->setLineWidth(2);
        vLine2->setFrameShape(QFrame::VLine);
        vLine2->setStyleSheet(QStringLiteral("color: black"));
        auto* vLine3 = new QFrame(parent);
        vLine3->setObjectName("vLine3");
        constexpr QRect vLine3Geom(510, 0, 2, 512);
        vLine3->setGeometry(vLine3Geom);
        vLine3->setLineWidth(2);
        vLine3->setFrameShape(QFrame::VLine);
        vLine3->setStyleSheet(QStringLiteral("color: black"));
    }
} // namespace sudoku
