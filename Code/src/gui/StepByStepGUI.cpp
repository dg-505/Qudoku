#include "globals.h"
#include "gui/SolvedGUI.h"
#include "gui/StepByStepGUI.h"

namespace sudoku
{
    StepByStepGUI::StepByStepGUI(Sudoku* sudoku, const std::array<uint8_t, static_cast<uint8_t>(global::order* global::order)>& initVals, QWidget* parent)
        : QMainWindow(parent, Qt::WindowFlags()),
          sudoku(sudoku),
          stepsStack(new QStepsStack(this)),
          stepsScrollBar(new QScrollBar(Qt::Horizontal, this)),
          firstButton(new QPushButton(QIcon(QStringLiteral(":/res/first.png")), QStringLiteral("  (Home)"), this)),
          prevButton(new QPushButton(QIcon(QStringLiteral(":/res/prev.png")), QStringLiteral("  (PgUp)"), this)),
          nextButton(new QPushButton(QIcon(QStringLiteral(":/res/next.png")), QStringLiteral("  (PgDn)"), this)),
          lastButton(new QPushButton(QIcon(QStringLiteral(":/res/last.png")), QStringLiteral("  (End)"), this))
    {
        constexpr QSize guiDim(537, 637);
        this->setFixedSize(guiDim);
        this->setObjectName("StepByStepGUI");
        this->setWindowTitle(QStringLiteral("Step-by-step solution"));
        this->setWindowIcon(QIcon(QStringLiteral(":/res/Qudoku.ico")));
        this->setStyleSheet(QStringLiteral("background: rgb(239, 239, 239)"));

        this->stepsStack->setObjectName("stepsStack");
        constexpr QRect stepsStackGeom(0, 0, 537, 587);
        this->stepsStack->setGeometry(stepsStackGeom);

        constexpr QRect messageLabelGeom(25, 0, 512, 50);
        const QFont messageFont(QStringLiteral("Open Sans"), 14, QFont::Bold, false);
        constexpr QRect stepFieldsGeom(0, 50, 537, 537);
#pragma unroll 100
        for (int numStep = 0; numStep < sudoku->getSteps()->size(); numStep++)
        {
            const QString msg = "Run " + QString::number(*sudoku->getSteps()->at(numStep).getFoundInRunNo(), global::base) + "/" +
                                QString::number(*sudoku->getSteps()->back().getFoundInRunNo(), global::base) + ", Step " +
                                QString::number(numStep, global::base) + " of " + QString::number(sudoku->getSteps()->size() - 1, global::base) + ":\n" +
                                QString::fromStdString(*sudoku->getSteps()->at(numStep).getFoundByType());

            // Preview of step
            if (numStep > 0)
            {
                auto* previewWidget = std::make_unique<QWidget>(stepsStack, Qt::WindowFlags()).release();
                this->stepsStack->addWidget(previewWidget);

                auto* msgPreviewLabel = std::make_unique<QLabel>(previewWidget, Qt::WindowFlags()).release();
                msgPreviewLabel->setObjectName("msgPreviewLabel");
                msgPreviewLabel->setGeometry(messageLabelGeom);
                msgPreviewLabel->setStyleSheet(QStringLiteral("color: black; background-color: rgba(239, 239, 239, 1.0)"));
                msgPreviewLabel->setFont(messageFont);
                msgPreviewLabel->setAlignment(Qt::AlignCenter);
                msgPreviewLabel->setText(msg);
                auto* previewFields = std::make_unique<QWidget>(previewWidget, Qt::WindowFlags()).release();
                previewFields->setGeometry(stepFieldsGeom);

                auto* previewLabel = std::make_unique<QLabel>(previewWidget, Qt::WindowFlags()).release();
                previewLabel->setObjectName("p");
                constexpr QRect previewLabelGeom(0, 0, 100, 25);
                previewLabel->setGeometry(previewLabelGeom);
                previewLabel->setStyleSheet(QStringLiteral("color: rgb(239, 239, 239); background-color: rgba(100, 100, 100, 1.0)"));
                previewLabel->setFont(messageFont);
                previewLabel->setAlignment(Qt::AlignCenter);
                previewLabel->setText(QStringLiteral("Preview"));

                SolvedGUI::drawFields(sudoku->getSteps()->at(numStep - 1), sudoku->getSteps()->at(numStep), initVals, previewFields);
                SolvedGUI::drawFrame(previewFields);
            }

            // executed step
            auto* stepWidget = std::make_unique<QWidget>(stepsStack, Qt::WindowFlags()).release();
            this->stepsStack->addWidget(stepWidget);

            auto* messageLabel = std::make_unique<QLabel>(stepWidget, Qt::WindowFlags()).release();
            messageLabel->setGeometry(messageLabelGeom);
            messageLabel->setStyleSheet(QStringLiteral("color: black; background-color: rgba(239, 239, 239, 1.0)"));
            messageLabel->setFont(messageFont);
            messageLabel->setAlignment(Qt::AlignCenter);
            messageLabel->setText(msg);
            auto* stepFields = std::make_unique<QWidget>(stepWidget, Qt::WindowFlags()).release();
            stepFields->setGeometry(stepFieldsGeom);

            SolvedGUI::drawFields(sudoku->getSteps()->at(numStep), sudoku->getSteps()->at(numStep), initVals, stepFields);
            SolvedGUI::drawFrame(stepFields);
        }

        // Scrollbar to scroll through steps
        this->stepsScrollBar->setObjectName("stepsScrollBar");
        constexpr QRect stepsScrollBarGeom(0, 587, 537, 15);
        this->stepsScrollBar->setGeometry(stepsScrollBarGeom);
        this->stepsScrollBar->setContextMenuPolicy(Qt::NoContextMenu);
        this->stepsScrollBar->setRange(0, stepsStack->count() - 1);
        this->stepsScrollBar->setPageStep(1);
        QObject::connect(
            this->stepsScrollBar, &QScrollBar::valueChanged, this, [this](int value)
            { this->stepsStack->setCurrentIndex(value); },
            Qt::AutoConnection);
        QObject::connect(this->stepsStack, &QStepsStack::stepChanged, this->stepsScrollBar, &QScrollBar::setValue, Qt::AutoConnection);

        // Buttons
        const QFont buttonFont(QStringLiteral("Open Sans"), 10, QFont::Bold, false);
        constexpr QSize buttonIconSize(20, 20);
        const QString buttonStyleSheet = QStringLiteral("QPushButton {color: black; background: rgb(239, 239, 239)}"
                                                        "QPushButton:hover {color: black; background: rgb(171, 171, 171)}"
                                                        "QPushButton:pressed {color: black; background: rgb(171, 171, 171)}");

        this->firstButton->setIconSize(buttonIconSize);
        this->firstButton->setObjectName("firstButton");
        constexpr QRect firstButtonGeom(0, 602, 134, 35);
        this->firstButton->setGeometry(firstButtonGeom);
        this->firstButton->setFont(buttonFont);
        this->firstButton->setStyleSheet(buttonStyleSheet);
        this->firstButton->setShortcut(QKeySequence(Qt::Key_Home, 0, 0, 0));
        QObject::connect(
            firstButton, &QPushButton::pressed, this, [this]()
            { constexpr int firstStep = 0; this->stepsStack->setCurrentIndex(firstStep); stepsScrollBar->setValue(firstStep); },
            Qt::AutoConnection);

        this->prevButton->setIconSize(buttonIconSize);
        this->prevButton->setObjectName("prevButton");
        constexpr QRect prevButtonGeom(134, 602, 134, 35);
        this->prevButton->setGeometry(prevButtonGeom);
        this->prevButton->setFont(buttonFont);
        this->prevButton->setStyleSheet(buttonStyleSheet);
        this->prevButton->setAutoRepeat(true);
        this->prevButton->setShortcut(QKeySequence(Qt::Key_PageUp, 0, 0, 0));
        QObject::connect(
            prevButton, &QPushButton::pressed, this, [this]()
            { const int prevStep = this->stepsStack->currentIndex() - 1; this->stepsStack->setCurrentIndex(prevStep); stepsScrollBar->setValue(prevStep); },
            Qt::AutoConnection);

        this->nextButton->setIconSize(buttonIconSize);
        this->nextButton->setObjectName("nextButton");
        constexpr QRect nextButtonGeom(269, 602, 134, 35);
        this->nextButton->setGeometry(nextButtonGeom);
        this->nextButton->setFont(buttonFont);
        this->nextButton->setStyleSheet(buttonStyleSheet);
        this->nextButton->setAutoRepeat(true);
        this->nextButton->setShortcut(QKeySequence(Qt::Key_PageDown, 0, 0, 0));
        QObject::connect(
            nextButton, &QPushButton::pressed, this, [this]()
            { const int nextStep = this->stepsStack->currentIndex() + 1; this->stepsStack->setCurrentIndex(nextStep); stepsScrollBar->setValue(nextStep); },
            Qt::AutoConnection);

        this->lastButton->setIconSize(buttonIconSize);
        this->lastButton->setObjectName("lastButton");
        constexpr QRect lastButtonGeom(403, 602, 134, 35);
        this->lastButton->setGeometry(lastButtonGeom);
        this->lastButton->setFont(buttonFont);
        this->lastButton->setStyleSheet(buttonStyleSheet);
        this->lastButton->setShortcut(QKeySequence(Qt::Key_End, 0, 0, 0));
        QObject::connect(
            lastButton, &QPushButton::pressed, this, [this]()
            { const int lastStep = this->stepsStack->count() - 1; this->stepsStack->setCurrentIndex(lastStep); stepsScrollBar->setValue(lastStep); },
            Qt::AutoConnection);
    }

    void StepByStepGUI::keyPressEvent(QKeyEvent* event)
    {
        if (event->key() == Qt::Key_Escape)
        {
            this->close();
        }
    }
} // namespace sudoku
