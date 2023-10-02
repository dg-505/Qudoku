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

        const QFont fieldsFont(QStringLiteral("Liberation Mono"), 32, QFont::Bold);
        const QFont candsFont(QStringLiteral("Liberation Mono"), 14, QFont::Bold);

        SolvedGUI::fillSolvedGrid(candsFont, fieldsFont, *sudoku->getGrid(), initVals, this);
        SolvedGUI::createFrame();
    }

    // Helper functions
    void SolvedGUI::fillSolvedGrid(const QFont& candsFont, const QFont& fieldsFont, std::array<std::array<sudoku::Field, global::order>, global::order>& step, const std::array<uint8_t, static_cast<uint8_t>(global::order* global::order)>& initVals, QWidget* parent)
    {
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
    }

    void SolvedGUI::createFrame()
    {
        // Frame for the grid
        // hLine0 = new QFrame(this);
        // hLine0->setObjectName("hLine0");
        // hLine0->setGeometry(0, 0, 512, 2);
        // hLine0->setLineWidth(2);
        // hLine0->setFrameShape(QFrame::HLine);
        // hLine1 = new QFrame(this);
        auto* hLine1 = new QFrame();
        hLine1->setObjectName("hLine1");
        constexpr QRect hLine1Geom(0, 169, 512, 2);
        hLine1->setGeometry(hLine1Geom);
        hLine1->setLineWidth(2);
        hLine1->setFrameShape(QFrame::HLine);
        hLine1->setStyleSheet(QStringLiteral("color: black"));
        // hLine2 = new QFrame(this);
        auto* hLine2 = new QFrame();
        hLine2->setObjectName("hLine2");
        constexpr QRect hLine2Geom(0, 341, 512, 2);
        hLine2->setGeometry(hLine2Geom);
        hLine2->setLineWidth(2);
        hLine2->setFrameShape(QFrame::HLine);
        hLine2->setStyleSheet(QStringLiteral("color: black"));
        // hLine3 = new QFrame(this);
        // hLine3->setObjectName("hLine3");
        // hLine3->setGeometry(0, 510, 512, 2);
        // hLine3->setLineWidth(2);
        // hLine3->setFrameShape(QFrame::HLine);
        // vLine0 = new QFrame(this);
        // vLine0->setObjectName("vLine0");
        // vLine0->setGeometry(0, 0, 2, 512);
        // vLine0->setLineWidth(2);
        // vLine0->setFrameShape(QFrame::VLine);
        // vLine1 = new QFrame(this);
        auto* vLine1 = new QFrame();
        vLine1->setObjectName("vLine1");
        constexpr QRect vLine1Geom(169, 0, 2, 512);
        vLine1->setGeometry(vLine1Geom);
        vLine1->setLineWidth(2);
        vLine1->setFrameShape(QFrame::VLine);
        vLine1->setStyleSheet(QStringLiteral("color: black"));
        // vLine2 = new QFrame(this);
        auto* vLine2 = new QFrame();
        vLine2->setObjectName("vLine2");
        constexpr QRect vLine2Geom(341, 0, 2, 512);
        vLine2->setGeometry(vLine2Geom);
        vLine2->setLineWidth(2);
        vLine2->setFrameShape(QFrame::VLine);
        vLine2->setStyleSheet(QStringLiteral("color: black"));
        // vLine3 = new QFrame(this);
        // vLine3->setObjectName("vLine3");
        // vLine3->setGeometry(510, 0, 2, 512);
        // vLine3->setLineWidth(2);
        // vLine3->setFrameShape(QFrame::VLine);
    }
} // namespace sudoku
