//#include <iomanip>
//#include <iostream>
//#include <sstream>

#include "globals.h"
#include "gui/SolvedGUI.h"

//using std::cout;
//using std::endl;

namespace sudoku
{

    SolvedGUI::SolvedGUI(Sudoku* sudoku, uint8_t* initVals, QWidget*)
        : sudoku(sudoku)
    {
        setFixedSize(512, 512);
        setObjectName("SolvedGUI");
        setWindowTitle("Solved Sudoku");
        QMainWindow::setWindowIcon(QIcon(QStringLiteral(":/res/Qudoku.ico")));

        // auto sudoku = MainGUI::init();

        // sudoku->print();
        // sudoku->printFields();

        QFont const fieldsFont("Liberation Mono", 32, QFont::Bold);
        QFont const candsFont("Liberation Mono", 14, QFont::Bold);

        uint8_t fID = 1;
        short posY = 0;
        for (uint8_t rID = 1; rID <= order; rID++)
        {
            short posX = 0;
            for (uint8_t cID = 1; cID <= order; cID++)
            {
                QLabel* field = new QLabel(this);
                //                std::stringstream objectName;
                //                objectName << "field" << std::setw(2) << std::setfill('0') << i;
                field->setObjectName("field" + QString::number(fID));
                field->setGeometry(posX, posY, 56, 56);
                field->setFont(fieldsFont);
                field->setStyleSheet("color: black; background-color: rgb(239, 239, 239)");
                field->setAlignment(Qt::AlignCenter);
                field->setFrameShape(QFrame::Panel);
                uint8_t const val = *sudoku->getFieldByCoord(rID, cID)->getVal();
                // If field is solved => fill value
                if (val != 0)
                {
                    field->setText(QString::number(val));
                    // If field was NOT given initially, but is solved now => set color green
                    if (initVals[fID - 1] == 0)
                    {
                        field->setStyleSheet("color: rgb(20,160,50); border: 1px solid black; "
                                             "background-color: rgb(239, 239, 239)");
                    }
                }
                else
                {
                    short candY = posY + 1;
                    uint8_t candI = 1;
                    for (uint8_t candR = 1; candR <= 3; candR++)
                    {
                        short candX = posX + 1;
                        for (uint8_t candR = 1; candR <= 3; candR++)
                        {
                            QLabel* cand = new QLabel(this);
                            cand->setGeometry(candX, candY, 17, 17);
                            cand->setAlignment(Qt::AlignCenter);
                            cand->setFont(candsFont);
                            cand->setStyleSheet("QLabel { color : rgb(20,50,255); }");

                            // cand->setFrameShape(QFrame::Panel);
                            auto* cands = sudoku->getFieldByCoord(rID, cID)->getCandidates();
                            if (std::find(cands->begin(), cands->end(), candI) != cands->end())
                            {
                                cand->setText(QString::number(candI));
                                // cand->setText("");
                            }
                            else
                            {
                                cand->setText("");
                            }
                            candI++;
                            candX += 19;
                        }
                        candY += 19;
                    }
                }
                fID++;
                posX += 56;
                if (cID % 3 == 0)
                {
                    posX += 4;
                }
            }
            posY += 56;
            if (rID % 3 == 0)
            {
                posY += 4;
            }
        }

        // Frame for the grid
        // hLine0 = new QFrame(this);
        // hLine0->setObjectName("hLine0");
        // hLine0->setGeometry(0, 0, 512, 2);
        // hLine0->setLineWidth(2);
        // hLine0->setFrameShape(QFrame::HLine);
        hLine1 = new QFrame(this);
        hLine1->setObjectName("hLine1");
        hLine1->setGeometry(0, 169, 512, 2);
        hLine1->setLineWidth(2);
        hLine1->setFrameShape(QFrame::HLine);
        hLine1->setStyleSheet("color: black");
        hLine2 = new QFrame(this);
        hLine2->setObjectName("hLine2");
        hLine2->setGeometry(0, 341, 512, 2);
        hLine2->setLineWidth(2);
        hLine2->setFrameShape(QFrame::HLine);
        hLine2->setStyleSheet("color: black");
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
        vLine1 = new QFrame(this);
        vLine1->setObjectName("vLine1");
        vLine1->setGeometry(169, 0, 2, 512);
        vLine1->setLineWidth(2);
        vLine1->setFrameShape(QFrame::VLine);
        vLine1->setStyleSheet("color: black");
        vLine2 = new QFrame(this);
        vLine2->setObjectName("vLine2");
        vLine2->setGeometry(341, 0, 2, 512);
        vLine2->setLineWidth(2);
        vLine2->setFrameShape(QFrame::VLine);
        vLine2->setStyleSheet("color: black");
        // vLine3 = new QFrame(this);
        // vLine3->setObjectName("vLine3");
        // vLine3->setGeometry(510, 0, 2, 512);
        // vLine3->setLineWidth(2);
        // vLine3->setFrameShape(QFrame::VLine);
    }

    SolvedGUI::~SolvedGUI()
    {
    }

} // namespace sudoku
