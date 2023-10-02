#include "gui/CandidatesGUI.h"
#include "globals.h"
#include "gui/SolvedGUI.h"

namespace sudoku
{

    CandidatesGUI::CandidatesGUI(Sudoku* sudoku, QWidget* /*parent*/)
        : sudoku(sudoku)
    {
        constexpr QSize guiDim(512, 512);
        this->setFixedSize(guiDim);
        this->setObjectName("CandidatesGUI");
        this->setWindowTitle(QStringLiteral("Candidates"));
        this->setWindowIcon(QIcon(QStringLiteral(":/res/Qudoku.ico")));

        const QFont fieldsFont(QStringLiteral("Liberation Mono"), 32, QFont::Bold);
        const QFont candsFont(QStringLiteral("Liberation Mono"), 14, QFont::Bold);

        uint8_t fID = 1;
        uint16_t posY = 0;
        for (uint8_t rID = 1; rID <= global::order; rID++)
        {
            uint16_t posX = 0;
            for (uint8_t cID = 1; cID <= global::order; cID++)
            {
                auto* field = new QLabel(this);
                field->setObjectName("field" + QString::number(fID));
                field->setGeometry(posX, posY, global::fieldDim, global::fieldDim);
                field->setFont(fieldsFont);
                field->setStyleSheet(QStringLiteral("color: black; background-color: rgb(239, 239, 239)"));
                field->setAlignment(Qt::AlignCenter);
                field->setFrameShape(QFrame::Panel);
                const uint8_t val = *sudoku->getFieldByCoord(rID, cID)->getVal();
                if (val != 0)
                {
                    field->setText(QString::number(val));
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
                            auto* cand = new QLabel(this);
                            const QRect candGeom(candX, candY, 17, 17);
                            cand->setGeometry(candGeom);
                            cand->setAlignment(Qt::AlignCenter);
                            cand->setFont(candsFont);
                            cand->setStyleSheet(QStringLiteral("color: rgb(20,50,255);"));

                            auto* cands = sudoku->getFieldByCoord(rID, cID)->getCandidates();
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

        SolvedGUI::createFrame();
    }
} // namespace sudoku
