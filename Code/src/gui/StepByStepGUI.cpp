#include "gui/SolvedGUI.h"
#include "gui/StepByStepGUI.h"

namespace sudoku
{
    StepByStepGUI::StepByStepGUI(Sudoku* sudoku, const std::array<uint8_t, static_cast<uint8_t>(global::order* global::order)>& initVals, QWidget* parent)
        : QMainWindow(parent, Qt::WindowFlags()),
          _sudoku(sudoku),
          _initVals(initVals),
          _currentStep(0),
          _currentPage(0),
          _isPreview(false),
          _stepsStack(new QWidget(this, Qt::WindowFlags())),
          _previewLabel(new QLabel(_stepsStack, Qt::WindowFlags())),
          _msgLabel(new QLabel(_stepsStack, Qt::WindowFlags())),
          _fieldsWidget(new QWidget(_stepsStack, Qt::WindowFlags())),
          _stepsScrollBar(new QScrollBar(Qt::Horizontal, this)),
          _firstButton(new QPushButton(QIcon(QStringLiteral(":/res/first.png")), QStringLiteral("  (Home)"), this)),
          _prevButton(new QPushButton(QIcon(QStringLiteral(":/res/prev.png")), QStringLiteral("  (PgUp)"), this)),
          _nextButton(new QPushButton(QIcon(QStringLiteral(":/res/next.png")), QStringLiteral("  (PgDn)"), this)),
          _lastButton(new QPushButton(QIcon(QStringLiteral(":/res/last.png")), QStringLiteral("  (End)"), this))
    {
        constexpr QSize guiDim(537, 637);
        this->setFixedSize(guiDim);
        this->setObjectName("StepByStepGUI");
        this->setWindowTitle(QStringLiteral("Step-by-step solution"));
        this->setWindowIcon(QIcon(QStringLiteral(":/res/Qudoku.ico")));
        this->setStyleSheet(QStringLiteral("background: rgb(239, 239, 239)"));
        
        this->_stepsStack->setObjectName("stepsStack");
        constexpr QRect stepsStackGeom(0, 0, 537, 587);
        this->_stepsStack->setGeometry(stepsStackGeom);
        this->_stepsStack->installEventFilter(this);

        constexpr QRect messageLabelGeom(25, 0, 512, 50);
        const QFont messageFont(QStringLiteral("Open Sans"), 14, QFont::Bold, false);
        constexpr QRect stepFieldsGeom(0, 50, 537, 537);
        const QString msg = this->getStepMsg(this->_currentStep);
        
        this->_fieldsWidget->setGeometry(stepFieldsGeom);
        
        this->_msgLabel->setObjectName("msgLabel");
        this->_msgLabel->setGeometry(messageLabelGeom);
        this->_msgLabel->setStyleSheet(QStringLiteral("color: black; background-color: rgba(239, 239, 239, 0.0)"));
        this->_msgLabel->setFont(messageFont);
        this->_msgLabel->setAlignment(Qt::AlignCenter);
        this->_msgLabel->setText(msg);
        
        this->_previewLabel->setObjectName("p");
        constexpr QRect previewLabelGeom(0, 0, 100, 25);
        this->_previewLabel->setGeometry(previewLabelGeom);
        this->_previewLabel->setStyleSheet(QStringLiteral("color: rgb(239, 239, 239); background-color: rgba(100, 100, 100, 1.0)"));
        this->_previewLabel->setFont(messageFont);
        this->_previewLabel->setAlignment(Qt::AlignCenter);
        this->_previewLabel->setText(QStringLiteral("Preview"));
        this->_previewLabel->hide();
        
        SolvedGUI::drawFields(this->_sudoku->getSteps()->at(this->_currentStep), this->_sudoku->getSteps()->at(this->_currentStep), this->_initVals, this->_fieldsWidget);
        SolvedGUI::drawFrame(this->_fieldsWidget);
        //        }
        //        }

        // Scrollbar to scroll through steps
        this->_stepsScrollBar->setObjectName("stepsScrollBar");
        constexpr QRect stepsScrollBarGeom(0, 587, 537, 15);
        this->_stepsScrollBar->setGeometry(stepsScrollBarGeom);
        this->_stepsScrollBar->setContextMenuPolicy(Qt::NoContextMenu);
        this->_stepsScrollBar->setRange(0, static_cast<uint8_t>(2) * static_cast<uint8_t>(this->_sudoku->getSteps()->size()) - 2);
        this->_stepsScrollBar->setPageStep(1);
        QObject::connect(
            this->_stepsScrollBar, &QScrollBar::valueChanged, this, [this](int value)
            {
                if (value < this->_currentPage)
                {
                    this->drawPrevStep();
                }
                else if (value > this->_currentPage)
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
        
        this->_firstButton->setIconSize(buttonIconSize);
        this->_firstButton->setObjectName("firstButton");
        constexpr QRect firstButtonGeom(0, 602, 134, 35);
        this->_firstButton->setGeometry(firstButtonGeom);
        this->_firstButton->setFont(buttonFont);
        this->_firstButton->setStyleSheet(buttonStyleSheet);
        this->_firstButton->setShortcut(QKeySequence(Qt::Key_Home, 0, 0, 0));
        QObject::connect(
            this->_firstButton, &QPushButton::pressed, this, [this]()
            {
                this->_currentStep = 0;
                const QString msg = this->getStepMsg(this->_currentStep);
                this->_msgLabel->setText(msg);
                SolvedGUI::drawFields(this->_sudoku->getSteps()->front(), this->_sudoku->getSteps()->front(), this->_initVals, this->_fieldsWidget);
                this->_currentPage = 0;
                this->_stepsScrollBar->setValue(this->_currentPage);
                this->_isPreview = false;
                this->_previewLabel->hide(); },
            Qt::AutoConnection);
        
        this->_prevButton->setIconSize(buttonIconSize);
        this->_prevButton->setObjectName("prevButton");
        constexpr QRect prevButtonGeom(134, 602, 134, 35);
        this->_prevButton->setGeometry(prevButtonGeom);
        this->_prevButton->setFont(buttonFont);
        this->_prevButton->setStyleSheet(buttonStyleSheet);
        this->_prevButton->setAutoRepeat(true);
        this->_prevButton->setShortcut(QKeySequence(Qt::Key_PageUp, 0, 0, 0));
        QObject::connect(this->_prevButton, &QPushButton::clicked, this, &StepByStepGUI::drawPrevStep, Qt::AutoConnection);
        
        this->_nextButton->setIconSize(buttonIconSize);
        this->_nextButton->setObjectName("nextButton");
        constexpr QRect nextButtonGeom(269, 602, 134, 35);
        this->_nextButton->setGeometry(nextButtonGeom);
        this->_nextButton->setFont(buttonFont);
        this->_nextButton->setStyleSheet(buttonStyleSheet);
        this->_nextButton->setAutoRepeat(true);
        this->_nextButton->setShortcut(QKeySequence(Qt::Key_PageDown, 0, 0, 0));
        QObject::connect(this->_nextButton, &QPushButton::clicked, this, &StepByStepGUI::drawNextStep, Qt::AutoConnection);
        
        this->_lastButton->setIconSize(buttonIconSize);
        this->_lastButton->setObjectName("lastButton");
        constexpr QRect lastButtonGeom(403, 602, 134, 35);
        this->_lastButton->setGeometry(lastButtonGeom);
        this->_lastButton->setFont(buttonFont);
        this->_lastButton->setStyleSheet(buttonStyleSheet);
        this->_lastButton->setShortcut(QKeySequence(Qt::Key_End, 0, 0, 0));
        QObject::connect(
            _lastButton, &QPushButton::pressed, this, [this]()
            {
                this->_currentStep = this->_sudoku->getSteps()->size()-1;
                const QString msg = this->getStepMsg(this->_currentStep);
                this->_msgLabel->setText(msg);
                SolvedGUI::drawFields(this->_sudoku->getSteps()->back(), this->_sudoku->getSteps()->back(), this->_initVals, this->_fieldsWidget);
                this->_currentPage = 2 * static_cast<uint8_t>(this->_sudoku->getSteps()->size()) - 2;
                this->_stepsScrollBar->setValue(this->_currentPage);
                this->_isPreview = false;
                this->_previewLabel->hide(); },
            Qt::AutoConnection);
    }

#pragma clang diagnostic push
#pragma ide diagnostic ignored "ConstantConditionsOC"
#pragma ide diagnostic ignored "UnreachableCode"
    void StepByStepGUI::drawPrevStep()
    {
        if (this->_currentStep >= 0)
        {
            if (this->_currentStep == 0)
            {
                this->_isPreview = false;
                this->_currentPage = 0;
            }
            else
            {
                this->_isPreview = !this->_isPreview;
                this->_currentPage -= 1;
            }
            this->_stepsScrollBar->setValue(this->_currentPage);
            const QString msg = this->getStepMsg(this->_currentStep);
            this->_msgLabel->setText(msg);
            if (!this->_isPreview) // Show executed prev step
            {
                this->_previewLabel->hide();
                SolvedGUI::drawFields(this->_sudoku->getSteps()->at(this->_currentStep), this->_sudoku->getSteps()->at(this->_currentStep), this->_initVals, this->_fieldsWidget);
            }
            else if (this->_currentStep > 0) // Show preview of prev step
            {
                this->_previewLabel->show();
                this->_currentStep -= 1;
                SolvedGUI::drawFields(this->_sudoku->getSteps()->at(this->_currentStep), this->_sudoku->getSteps()->at(this->_currentStep + 1), this->_initVals, this->_fieldsWidget);
            }
        }
    }

    void StepByStepGUI::drawNextStep()
    {
        if (this->_currentStep < this->_sudoku->getSteps()->size() - 1)
        {
            this->_isPreview = !this->_isPreview;
            const QString msg = this->getStepMsg(this->_currentStep + 1);
            this->_msgLabel->setText(msg);
            if (this->_isPreview) // Show preview of next step
            {
                this->_previewLabel->show();
                SolvedGUI::drawFields(this->_sudoku->getSteps()->at(this->_currentStep), this->_sudoku->getSteps()->at(this->_currentStep + 1), this->_initVals, this->_fieldsWidget);
            }
            else // Show executed next step
            {
                this->_previewLabel->hide();
                this->_currentStep += 1;
                SolvedGUI::drawFields(this->_sudoku->getSteps()->at(this->_currentStep), this->_sudoku->getSteps()->at(this->_currentStep), this->_initVals, this->_fieldsWidget);
            }
            this->_currentPage += 1;
            this->_stepsScrollBar->setValue(this->_currentPage);
        }
    }
#pragma clang diagnostic pop

    auto StepByStepGUI::getStepMsg(uint8_t step) const -> QString
    {
        return "Run " + QString::number(*this->_sudoku->getSteps()->at(step).getFoundInRunNo(), global::base) + "/" +
               QString::number(*this->_sudoku->getSteps()->back().getFoundInRunNo(), global::base) + ", Step " +
               QString::number(step, global::base) + " of " + QString::number(this->_sudoku->getSteps()->size() - 1, global::base) + ":\n" +
               QString::fromStdString(*this->_sudoku->getSteps()->at(step).getFoundByType());
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
        if (watched == this->_stepsStack && event->type() == QEvent::Wheel)
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
