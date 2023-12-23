#include "gui/SolvedGUI.h"
#include "gui/StepByStepGUI.h"

namespace sudoku
{
    StepByStepGUI::StepByStepGUI(Sudoku* sudoku, const std::array<uint8_t, static_cast<uint8_t>(global::order* global::order)>& initVals, QWidget* parent)
        : QMainWindow(parent, Qt::WindowFlags()),
          sudoku(sudoku),
          initVals(initVals),
          currentStep(0),
          currentPage(0),
          isPreview(false),
          stepsStack(new QWidget(this, Qt::WindowFlags())),
          previewLabel(new QLabel(stepsStack, Qt::WindowFlags())),
          msgLabel(new QLabel(stepsStack, Qt::WindowFlags())),
          fieldsWidget(new QWidget(stepsStack, Qt::WindowFlags())),
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
        this->stepsStack->installEventFilter(this);

        constexpr QRect messageLabelGeom(25, 0, 512, 50);
        const QFont messageFont(QStringLiteral("Open Sans"), 14, QFont::Bold, false);
        constexpr QRect stepFieldsGeom(0, 50, 537, 537);
        const QString msg = this->getStepMsg(this->currentStep);

        this->fieldsWidget->setGeometry(stepFieldsGeom);

        this->msgLabel->setObjectName("msgLabel");
        this->msgLabel->setGeometry(messageLabelGeom);
        this->msgLabel->setStyleSheet(QStringLiteral("color: black; background-color: rgba(239, 239, 239, 0.0)"));
        this->msgLabel->setFont(messageFont);
        this->msgLabel->setAlignment(Qt::AlignCenter);
        this->msgLabel->setText(msg);

        this->previewLabel->setObjectName("p");
        constexpr QRect previewLabelGeom(0, 0, 100, 25);
        this->previewLabel->setGeometry(previewLabelGeom);
        this->previewLabel->setStyleSheet(QStringLiteral("color: rgb(239, 239, 239); background-color: rgba(100, 100, 100, 1.0)"));
        this->previewLabel->setFont(messageFont);
        this->previewLabel->setAlignment(Qt::AlignCenter);
        this->previewLabel->setText(QStringLiteral("Preview"));
        this->previewLabel->hide();

        SolvedGUI::drawFields(this->sudoku->getSteps()->at(this->currentStep), this->sudoku->getSteps()->at(this->currentStep), this->initVals, this->fieldsWidget);
        SolvedGUI::drawFrame(this->fieldsWidget);
        //        }
        //        }

        // Scrollbar to scroll through steps
        this->stepsScrollBar->setObjectName("stepsScrollBar");
        constexpr QRect stepsScrollBarGeom(0, 587, 537, 15);
        this->stepsScrollBar->setGeometry(stepsScrollBarGeom);
        this->stepsScrollBar->setContextMenuPolicy(Qt::NoContextMenu);
        this->stepsScrollBar->setRange(0, static_cast<uint8_t>(2) * static_cast<uint8_t>(this->sudoku->getSteps()->size()) - 2);
        this->stepsScrollBar->setPageStep(1);
        QObject::connect(
            this->stepsScrollBar, &QScrollBar::valueChanged, this, [this](int value)
            {
                if (value < this->currentPage)
                {
                    this->drawPrevStep();
                }
                else if (value > this->currentPage)
                {
                    this->drawNextStep();
                } },
            Qt::AutoConnection);

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
            this->firstButton, &QPushButton::pressed, this, [this]()
            {
                this->currentStep = 0;
                const QString msg = this->getStepMsg(this->currentStep);
                this->msgLabel->setText(msg);
                SolvedGUI::drawFields(this->sudoku->getSteps()->front(), this->sudoku->getSteps()->front(), this->initVals, this->fieldsWidget);
                this->currentPage = 0;
                this->stepsScrollBar->setValue(this->currentPage);
                this->isPreview = false;
                this->previewLabel->hide(); },
            Qt::AutoConnection);

        this->prevButton->setIconSize(buttonIconSize);
        this->prevButton->setObjectName("prevButton");
        constexpr QRect prevButtonGeom(134, 602, 134, 35);
        this->prevButton->setGeometry(prevButtonGeom);
        this->prevButton->setFont(buttonFont);
        this->prevButton->setStyleSheet(buttonStyleSheet);
        this->prevButton->setAutoRepeat(true);
        this->prevButton->setShortcut(QKeySequence(Qt::Key_PageUp, 0, 0, 0));
        QObject::connect(this->prevButton, &QPushButton::clicked, this, &StepByStepGUI::drawPrevStep, Qt::AutoConnection);

        this->nextButton->setIconSize(buttonIconSize);
        this->nextButton->setObjectName("nextButton");
        constexpr QRect nextButtonGeom(269, 602, 134, 35);
        this->nextButton->setGeometry(nextButtonGeom);
        this->nextButton->setFont(buttonFont);
        this->nextButton->setStyleSheet(buttonStyleSheet);
        this->nextButton->setAutoRepeat(true);
        this->nextButton->setShortcut(QKeySequence(Qt::Key_PageDown, 0, 0, 0));
        QObject::connect(this->nextButton, &QPushButton::clicked, this, &StepByStepGUI::drawNextStep, Qt::AutoConnection);

        this->lastButton->setIconSize(buttonIconSize);
        this->lastButton->setObjectName("lastButton");
        constexpr QRect lastButtonGeom(403, 602, 134, 35);
        this->lastButton->setGeometry(lastButtonGeom);
        this->lastButton->setFont(buttonFont);
        this->lastButton->setStyleSheet(buttonStyleSheet);
        this->lastButton->setShortcut(QKeySequence(Qt::Key_End, 0, 0, 0));
        QObject::connect(
            lastButton, &QPushButton::pressed, this, [this]()
            {
                this->currentStep = this->sudoku->getSteps()->size()-1;
                const QString msg = this->getStepMsg(this->currentStep);
                this->msgLabel->setText(msg);
                SolvedGUI::drawFields(this->sudoku->getSteps()->back(), this->sudoku->getSteps()->back(), this->initVals, this->fieldsWidget);
                this->currentPage = 2 * static_cast<uint8_t>(this->sudoku->getSteps()->size()) - 2;
                this->stepsScrollBar->setValue(this->currentPage);
                this->isPreview = false;
                this->previewLabel->hide(); },
            Qt::AutoConnection);
    }

#pragma clang diagnostic push
#pragma ide diagnostic ignored "ConstantConditionsOC"
#pragma ide diagnostic ignored "UnreachableCode"
    void StepByStepGUI::drawPrevStep()
    {
        if (this->currentStep >= 0)
        {
            if (this->currentStep == 0)
            {
                this->isPreview = false;
                this->currentPage = 0;
            }
            else
            {
                this->isPreview = !this->isPreview;
                this->currentPage -= 1;
            }
            this->stepsScrollBar->setValue(this->currentPage);
            const QString msg = this->getStepMsg(this->currentStep);
            this->msgLabel->setText(msg);
            if (!this->isPreview) // Show executed prev step
            {
                this->previewLabel->hide();
                SolvedGUI::drawFields(this->sudoku->getSteps()->at(this->currentStep), this->sudoku->getSteps()->at(this->currentStep), this->initVals, this->fieldsWidget);
            }
            else if (this->currentStep > 0) // Show preview of prev step
            {
                this->previewLabel->show();
                this->currentStep -= 1;
                SolvedGUI::drawFields(this->sudoku->getSteps()->at(this->currentStep), this->sudoku->getSteps()->at(this->currentStep + 1), this->initVals, this->fieldsWidget);
            }
        }
    }

    void StepByStepGUI::drawNextStep()
    {
        if (this->currentStep < this->sudoku->getSteps()->size() - 1)
        {
            this->isPreview = !this->isPreview;
            const QString msg = this->getStepMsg(this->currentStep + 1);
            this->msgLabel->setText(msg);
            if (this->isPreview) // Show preview of next step
            {
                this->previewLabel->show();
                SolvedGUI::drawFields(this->sudoku->getSteps()->at(this->currentStep), this->sudoku->getSteps()->at(this->currentStep + 1), this->initVals, this->fieldsWidget);
            }
            else // Show executed next step
            {
                this->previewLabel->hide();
                this->currentStep += 1;
                SolvedGUI::drawFields(this->sudoku->getSteps()->at(this->currentStep), this->sudoku->getSteps()->at(this->currentStep), this->initVals, this->fieldsWidget);
            }
            this->currentPage += 1;
            this->stepsScrollBar->setValue(this->currentPage);
        }
    }
#pragma clang diagnostic pop

    auto StepByStepGUI::getStepMsg(uint8_t step) const -> QString
    {
        return "Run " + QString::number(*this->sudoku->getSteps()->at(step).getFoundInRunNo(), global::base) + "/" +
               QString::number(*this->sudoku->getSteps()->back().getFoundInRunNo(), global::base) + ", Step " +
               QString::number(step, global::base) + " of " + QString::number(this->sudoku->getSteps()->size() - 1, global::base) + ":\n" +
               QString::fromStdString(*this->sudoku->getSteps()->at(step).getFoundByType());
    }

    void StepByStepGUI::keyPressEvent(QKeyEvent* event)
    {
        if (event->key() == Qt::Key_Escape)
        {
            this->close();
        }
    }

    // eventFilter to scroll through steps with mouse wheel
    auto StepByStepGUI::eventFilter(QObject* watched, QEvent* event) -> bool
    {
        if (watched == this->stepsStack && event->type() == QEvent::Wheel)
        {
            if (dynamic_cast<QWheelEvent*>(event)->angleDelta().y() > 0)
            {
                this->drawPrevStep();
            }
            else
            {
                this->drawNextStep();
            }
            return true;
        }
        return QMainWindow::eventFilter(watched, event);
    }
} // namespace sudoku
