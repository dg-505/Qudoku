//#include <iomanip>
//#include <iostream>
//#include <sstream>

#include "gui/StepByStepGUI.h"
#include "globals.h"
#include "gui/SolvedGUI.h"

namespace sudoku
{
    StepByStepGUI::StepByStepGUI(Sudoku* sudoku, const std::array<uint8_t, static_cast<uint8_t>(global::order* global::order)>& initVals, QWidget* /*parent*/)
        : sudoku(sudoku),
          stepsStack(new QStackedWidget(this)),
          firstButton(new QPushButton(QIcon(QStringLiteral(":/res/first.png")), QStringLiteral(""), this)),
          prevButton(new QPushButton(QIcon(QStringLiteral(":/res/prev.png")), QStringLiteral(""), this)),
          nextButton(new QPushButton(QIcon(QStringLiteral(":/res/next.png")), QStringLiteral(""), this)),
          lastButton(new QPushButton(QIcon(QStringLiteral(":/res/last.png")), QStringLiteral(""), this))
    {
        constexpr QSize guiDim(512, 612);
        this->setFixedSize(guiDim);
        this->setObjectName("StepByStepGUI");
        this->setWindowTitle(QStringLiteral("Step-by-step solution"));
        this->setWindowIcon(QIcon(QStringLiteral(":/res/Qudoku.ico")));

        const QFont fieldsFont(QStringLiteral("Liberation Mono"), 32, QFont::Bold);
        const QFont candsFont(QStringLiteral("Liberation Mono"), 14, QFont::Bold);

        this->stepsStack->setObjectName("stepsStack");
        constexpr QRect stepsStackGeom(0, 0, 512, 562);
        this->stepsStack->setGeometry(stepsStackGeom);

        constexpr QRect messageLabelGeom(0, 0, 512, 50);
        const QFont messageFont(QStringLiteral("Open Sans"), 12, QFont::Bold);
        constexpr QRect stepFieldsGeom(0, 50, 512, 512);
        for (int numStep = 0; numStep < sudoku->getSteps()->size(); numStep++)
        {
            auto* stepWidget = new QWidget(stepsStack);
            this->stepsStack->addWidget(stepWidget);

            auto* messageLabel = new QLabel(stepWidget);
            messageLabel->setGeometry(messageLabelGeom);
            messageLabel->setStyleSheet(QStringLiteral("color: black; background-color: rgba(239, 239, 239, 1.0)"));
            messageLabel->setFont(messageFont);
            messageLabel->setAlignment(Qt::AlignCenter);
            messageLabel->setText("Run " + QString::number(sudoku->getFoundInRunNo()->at(numStep)) + "/" +
                                  QString::number(sudoku->getFoundInRunNo()->back()) + ", Step " +
                                  QString::number(numStep) + " of " + QString::number(sudoku->getSteps()->size() - 1) + ":\n" +
                                  sudoku->getFoundByType()->at(numStep));

            auto* stepFields = new QWidget(stepWidget);
            stepFields->setGeometry(stepFieldsGeom);

            SolvedGUI::fillSolvedGrid(candsFont, fieldsFont, sudoku->getSteps()->at(numStep), initVals, stepFields);

            // Frame for the grid
            this->hLine0 = new QFrame(stepFields);
            this->hLine0->setObjectName("hLine0");
            constexpr QRect hLine0Geom(0, 0, 512, 2);
            this->hLine0->setGeometry(hLine0Geom);
            this->hLine0->setLineWidth(2);
            this->hLine0->setFrameShape(QFrame::HLine);
            this->hLine0->setStyleSheet(QStringLiteral("color: black"));
            this->hLine1 = new QFrame(stepFields);
            this->hLine1->setObjectName("hLine1");
            constexpr QRect hLine1Geom(0, 169, 512, 2);
            this->hLine1->setGeometry(hLine1Geom);
            this->hLine1->setLineWidth(2);
            this->hLine1->setFrameShape(QFrame::HLine);
            this->hLine1->setStyleSheet(QStringLiteral("color: black"));
            this->hLine2 = new QFrame(stepFields);
            this->hLine2->setObjectName("hLine2");
            constexpr QRect hLine2Geom(0, 341, 512, 2);
            this->hLine2->setGeometry(hLine2Geom);
            this->hLine2->setLineWidth(2);
            this->hLine2->setFrameShape(QFrame::HLine);
            this->hLine2->setStyleSheet(QStringLiteral("color: black"));
            this->hLine3 = new QFrame(stepFields);
            this->hLine3->setObjectName("hLine3");
            constexpr QRect hLine3Geom(0, 510, 512, 2);
            this->hLine3->setGeometry(hLine3Geom);
            this->hLine3->setLineWidth(2);
            this->hLine3->setFrameShape(QFrame::HLine);
            this->hLine3->setStyleSheet(QStringLiteral("color: black"));
            // vLine0 = new QFrame(stepFields);
            // vLine0->setObjectName("vLine0");
            // vLine0->setGeometry(0, 0, 2, 512);
            // vLine0->setLineWidth(2);
            // vLine0->setFrameShape(QFrame::VLine);
            this->vLine1 = new QFrame(stepFields);
            this->vLine1->setObjectName("vLine1");
            constexpr QRect vLine1Geom(169, 0, 2, 512);
            this->vLine1->setGeometry(vLine1Geom);
            this->vLine1->setLineWidth(2);
            this->vLine1->setFrameShape(QFrame::VLine);
            this->vLine1->setStyleSheet(QStringLiteral("color: black"));
            this->vLine2 = new QFrame(stepFields);
            this->vLine2->setObjectName("vLine2");
            constexpr QRect vLine2Geom(341, 0, 2, 512);
            this->vLine2->setGeometry(vLine2Geom);
            this->vLine2->setLineWidth(2);
            this->vLine2->setFrameShape(QFrame::VLine);
            this->vLine2->setStyleSheet(QStringLiteral("color: black"));
            // vLine3 = new QFrame(stepFields);
            // vLine3->setObjectName("vLine3");
            // vLine3->setGeometry(510, 0, 2, 512);
            // vLine3->setLineWidth(2);
            // vLine3->setFrameShape(QFrame::VLine);
        }

        // Buttons
        const QFont buttonFont(QStringLiteral("Open Sans"), 28, QFont::Bold);
        constexpr QSize buttonIconSize(24, 24);

        this->firstButton->setIconSize(buttonIconSize);
        this->firstButton->setObjectName("firstButton");
        constexpr QRect firstButtonGeom(0, 562, 128, 50);
        this->firstButton->setGeometry(firstButtonGeom);
        this->firstButton->setFont(buttonFont);
        this->firstButton->setStyleSheet(QStringLiteral("color: black; background-color: rgb(239, 239, 239)"));
        StepByStepGUI::connect(firstButton, &QPushButton::clicked, this, [this]()
                               { stepsStack->setCurrentIndex(0); });

        this->prevButton->setIconSize(buttonIconSize);
        this->prevButton->setObjectName("prevButton");
        constexpr QRect prevButtonGeom(128, 562, 128, 50);
        this->prevButton->setGeometry(prevButtonGeom);
        this->prevButton->setFont(buttonFont);
        this->prevButton->setStyleSheet(QStringLiteral("color: black; background-color: rgb(239, 239, 239)"));
        StepByStepGUI::connect(prevButton, &QPushButton::clicked, this, [this]()
                               { stepsStack->setCurrentIndex(stepsStack->currentIndex() - 1); });

        this->nextButton->setIconSize(buttonIconSize);
        this->nextButton->setObjectName("nextButton");
        constexpr QRect nextButtonGeom(256, 562, 128, 50);
        this->nextButton->setGeometry(nextButtonGeom);
        this->nextButton->setFont(buttonFont);
        this->nextButton->setStyleSheet(QStringLiteral("color: black; background-color: rgb(239, 239, 239)"));
        StepByStepGUI::connect(nextButton, &QPushButton::clicked, this, [this]()
                               { stepsStack->setCurrentIndex(stepsStack->currentIndex() + 1); });

        this->lastButton->setIconSize(buttonIconSize);
        this->lastButton->setObjectName("lastButton");
        constexpr QRect lastButtonGeom(384, 562, 128, 50);
        this->lastButton->setGeometry(lastButtonGeom);
        this->lastButton->setFont(buttonFont);
        this->lastButton->setStyleSheet(QStringLiteral("color: black; background-color: rgb(239, 239, 239)"));
        StepByStepGUI::connect(lastButton, &QPushButton::clicked, this, [this]()
                               { stepsStack->setCurrentIndex(stepsStack->count() - 1); });
    }
} // namespace sudoku
