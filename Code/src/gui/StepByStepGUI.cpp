//#include <iomanip>
//#include <iostream>
//#include <sstream>

#include "globals.h"
#include "gui/StepByStepGUI.h"

//using std::cout;
//using std::endl;

namespace sudoku
{

    StepByStepGUI::StepByStepGUI(Sudoku* sudoku, uint8_t* initVals, QWidget*)
        : sudoku(sudoku)
    {
        setFixedSize(512, 612);
        setObjectName("StepByStepGUI");
        setWindowTitle(QStringLiteral("Step-by-step solution"));
        QMainWindow::setWindowIcon(QIcon(QStringLiteral(":/res/Qudoku.ico")));

        // auto sudoku = MainGUI::init();

        // sudoku->print();
        // sudoku->printFields();

        QFont const fieldsFont("Liberation Mono", 32, QFont::Bold);
        QFont const candsFont("Liberation Mono", 14, QFont::Bold);

        stepsStack = new QStackedWidget(this);
        stepsStack->setObjectName("stepsStack");
        stepsStack->setGeometry(0, 0, 512, 562);
        // sudoku->getLogTextArea()->append(QString::number(sudoku->getSteps().size()));

        for (unsigned short numStep = 0; numStep < sudoku->getSteps()->size(); numStep++)
        {
            QWidget* stepWidget = new QWidget(stepsStack);
            stepsStack->addWidget(stepWidget);

            QLabel* messageLabel = new QLabel(stepWidget);

            // messageLabel->setObjectName("messageLabel");
            messageLabel->setGeometry(0, 0, 512, 50);
            messageLabel->setStyleSheet("color: black; background-color: rgba(239, 239, 239, 1.0)");
            messageLabel->setFont(QFont("Open Sans", 12, QFont::Bold));
            messageLabel->setAlignment(Qt::AlignCenter);
            messageLabel->setText("Run " + QString::number(sudoku->getFoundInRunNo()->at(numStep)) + "/" +
                                  QString::number(sudoku->getFoundInRunNo()->back()) + ", Step " +
                                  QString::number(numStep) + " of " + QString::number(sudoku->getSteps()->size() - 1) + ":\n" +
                                  sudoku->getFoundByType()->at(numStep));

            QWidget* stepFields = new QWidget(stepWidget);
            stepFields->setGeometry(0, 50, 512, 512);

            auto step = sudoku->getSteps()->at(numStep);

            uint8_t fID = 1;
            short posY = 0;
            for (uint8_t rID = 1; rID <= order; rID++)
            {
                short posX = 0;
                for (uint8_t cID = 1; cID <= order; cID++)
                {
                    QLabel* field = new QLabel(stepFields);
                    //                    std::stringstream objectName;
                    //                    objectName << "field" << std::setw(2) << std::setfill('0') << i;
                    field->setObjectName(QString::number(fID));
                    field->setGeometry(posX, posY, 56, 56);
                    field->setFont(fieldsFont);
                    field->setStyleSheet(
                        "color: black; background-color: rgba(239, 239, 239, 1.0)");
                    field->setAlignment(Qt::AlignCenter);
                    field->setFrameShape(QFrame::Panel);
                    uint8_t const val = *step[rID - 1][cID - 1].getVal();
                    // If field is solved => fill value
                    if (val != 0)
                    {
                        field->setText(QString::number(val));
                        // If field was NOT given initially, but is solved now => set color green
                        if (initVals[fID - 1] == 0)
                        {
                            field->setStyleSheet(
                                "color: rgb(20,160,50); background-color: rgba(239, 239, 239, "
                                "1.0); border: 1px solid black");
                        }
                    }
                    else
                    {
                        short candY = posY + 1;
                        uint8_t candI = 1;
                        for (short candR = 1; candR <= 3; candR++)
                        {
                            short candX = posX + 1;
                            for (short candR = 1; candR <= 3; candR++)
                            {
                                QLabel* cand = new QLabel(stepFields);
                                cand->setGeometry(candX, candY, 17, 17);
                                cand->setAlignment(Qt::AlignCenter);
                                cand->setFont(candsFont);
                                cand->setStyleSheet("color: rgb(20,50,255)");

                                // cand->setFrameShape(QFrame::Panel);
                                auto* cands = step[rID - 1][cID - 1].getCandidates();
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
            hLine0 = new QFrame(stepFields);
            hLine0->setObjectName("hLine0");
            hLine0->setGeometry(0, 0, 512, 2);
            hLine0->setLineWidth(2);
            hLine0->setFrameShape(QFrame::HLine);
            hLine0->setStyleSheet("color: black");
            hLine1 = new QFrame(stepFields);
            hLine1->setObjectName("hLine1");
            hLine1->setGeometry(0, 169, 512, 2);
            hLine1->setLineWidth(2);
            hLine1->setFrameShape(QFrame::HLine);
            hLine1->setStyleSheet("color: black");
            hLine2 = new QFrame(stepFields);
            hLine2->setObjectName("hLine2");
            hLine2->setGeometry(0, 341, 512, 2);
            hLine2->setLineWidth(2);
            hLine2->setFrameShape(QFrame::HLine);
            hLine2->setStyleSheet("color: black");
            hLine3 = new QFrame(stepFields);
            hLine3->setObjectName("hLine3");
            hLine3->setGeometry(0, 510, 512, 2);
            hLine3->setLineWidth(2);
            hLine3->setFrameShape(QFrame::HLine);
            hLine3->setStyleSheet("color: black");
            // vLine0 = new QFrame(stepFields);
            // vLine0->setObjectName("vLine0");
            // vLine0->setGeometry(0, 0, 2, 512);
            // vLine0->setLineWidth(2);
            // vLine0->setFrameShape(QFrame::VLine);
            vLine1 = new QFrame(stepFields);
            vLine1->setObjectName("vLine1");
            vLine1->setGeometry(169, 0, 2, 512);
            vLine1->setLineWidth(2);
            vLine1->setFrameShape(QFrame::VLine);
            vLine1->setStyleSheet("color: black");
            vLine2 = new QFrame(stepFields);
            vLine2->setObjectName("vLine2");
            vLine2->setGeometry(341, 0, 2, 512);
            vLine2->setLineWidth(2);
            vLine2->setFrameShape(QFrame::VLine);
            vLine2->setStyleSheet("color: black");
            // vLine3 = new QFrame(stepFields);
            // vLine3->setObjectName("vLine3");
            // vLine3->setGeometry(510, 0, 2, 512);
            // vLine3->setLineWidth(2);
            // vLine3->setFrameShape(QFrame::VLine);
        }

        // Buttons
        QFont const buttonFont("Open Sans", 28, QFont::Bold);
        QSize const buttonIconSize(24, 24);

        firstButton = new QPushButton(QIcon(":/res/first.png"), "", this);
        firstButton->setIconSize(buttonIconSize);
        firstButton->setObjectName("firstButton");
        firstButton->setGeometry(0, 562, 128, 50);
        firstButton->setFont(buttonFont);
        firstButton->setStyleSheet("color: black; background-color: rgb(239, 239, 239)");
        // firstButton->setText("<<");
        // firstButton->setText("\u23EE");
        QObject::connect(firstButton, &QPushButton::clicked, this, [this]() {
            stepsStack->setCurrentIndex(0);
        });

        prevButton = new QPushButton(QIcon(":/res/prev.png"), "", this);
        prevButton->setIconSize(buttonIconSize);
        prevButton->setObjectName("prevButton");
        prevButton->setGeometry(128, 562, 128, 50);
        prevButton->setFont(buttonFont);
        prevButton->setStyleSheet("color: black; background-color: rgb(239, 239, 239)");
        // prevButton->setText("<");
        // prevButton->setText("\u23F4");
        QObject::connect(prevButton, &QPushButton::clicked, this, [this]() {
            stepsStack->setCurrentIndex(stepsStack->currentIndex() - 1);
        });

        nextButton = new QPushButton(QIcon(":/res/next.png"), "", this);
        nextButton->setIconSize(buttonIconSize);
        nextButton->setObjectName("nextButton");
        nextButton->setGeometry(256, 562, 128, 50);
        nextButton->setFont(buttonFont);
        nextButton->setStyleSheet("color: black; background-color: rgb(239, 239, 239)");
        // nextButton->setText(">");
        // nextButton->setText("\u23F5");
        QObject::connect(nextButton, &QPushButton::clicked, this, [this]() {
            stepsStack->setCurrentIndex(stepsStack->currentIndex() + 1);
        });

        lastButton = new QPushButton(QIcon(":/res/last.png"), "", this);
        lastButton->setIconSize(buttonIconSize);
        lastButton->setObjectName("lastButton");
        lastButton->setGeometry(384, 562, 128, 50);
        lastButton->setFont(buttonFont);
        lastButton->setStyleSheet("color: black; background-color: rgb(239, 239, 239)");
        // lastButton->setText(">>");
        // lastButton->setText("\u23ED");
        QObject::connect(lastButton, &QPushButton::clicked, this, [this]() {
            stepsStack->setCurrentIndex(stepsStack->count() - 1);
        });
    }

    StepByStepGUI::~StepByStepGUI()
    {
    }

} // namespace sudoku
