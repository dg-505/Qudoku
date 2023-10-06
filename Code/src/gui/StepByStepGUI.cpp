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
        this->setStyleSheet(QStringLiteral("background: rgb(239, 239, 239)"));

        this->stepsStack->setObjectName("stepsStack");
        constexpr QRect stepsStackGeom(0, 0, 512, 562);
        this->stepsStack->setGeometry(stepsStackGeom);

        constexpr QRect messageLabelGeom(0, 0, 512, 50);
        const QFont messageFont(QStringLiteral("Open Sans"), 12, QFont::Bold);
        constexpr QRect stepFieldsGeom(0, 50, 512, 512);
        for (int numStep = 0; numStep < sudoku->getSteps()->size(); numStep++)
        {
            // Preview of step
            if (numStep > 0)
            {
                auto* previewWidget = new QWidget(stepsStack);
                this->stepsStack->addWidget(previewWidget);

                auto* previewLabel = new QLabel(previewWidget);
                previewLabel->setGeometry(messageLabelGeom);
                previewLabel->setStyleSheet(QStringLiteral("color: black; background-color: rgba(239, 239, 239, 1.0)"));
                previewLabel->setFont(messageFont);
                previewLabel->setAlignment(Qt::AlignCenter);
                previewLabel->setText("Preview: Run " + QString::number(*sudoku->getSteps()->at(numStep).getFoundInRunNo()) + "/" +
                                      QString::number(*sudoku->getSteps()->back().getFoundInRunNo()) + ", Step " +
                                      QString::number(numStep) + " of " + QString::number(sudoku->getSteps()->size() - 1) + ":\n" +
                                      QString::fromStdString(*sudoku->getSteps()->at(numStep).getFoundByType()));
                auto* previewFields = new QWidget(previewWidget);
                previewFields->setGeometry(stepFieldsGeom);
                SolvedGUI::drawFields(sudoku->getSteps()->at(numStep - 1), sudoku->getSteps()->at(numStep), initVals, previewFields);
                SolvedGUI::drawFrame(previewFields);
            }

            // executed step
            auto* stepWidget = new QWidget(stepsStack);
            this->stepsStack->addWidget(stepWidget);

            auto* messageLabel = new QLabel(stepWidget);
            messageLabel->setGeometry(messageLabelGeom);
            messageLabel->setStyleSheet(QStringLiteral("color: black; background-color: rgba(239, 239, 239, 1.0)"));
            messageLabel->setFont(messageFont);
            messageLabel->setAlignment(Qt::AlignCenter);
            messageLabel->setText("Run " + QString::number(*sudoku->getSteps()->at(numStep).getFoundInRunNo()) + "/" +
                                  QString::number(*sudoku->getSteps()->back().getFoundInRunNo()) + ", Step " +
                                  QString::number(numStep) + " of " + QString::number(sudoku->getSteps()->size() - 1) + ":\n" +
                                  QString::fromStdString(*sudoku->getSteps()->at(numStep).getFoundByType()));
            auto* stepFields = new QWidget(stepWidget);
            stepFields->setGeometry(stepFieldsGeom);

            SolvedGUI::drawFields(sudoku->getSteps()->at(numStep), sudoku->getSteps()->at(numStep), initVals, stepFields);
            SolvedGUI::drawFrame(stepFields);
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
