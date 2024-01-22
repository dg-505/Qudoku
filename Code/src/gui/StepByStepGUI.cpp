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
          _lastButton(new QPushButton(QIcon(QStringLiteral(":/res/last.png")), QStringLiteral("  (End)"), this)),
          _exportButton(new QPushButton(this))
    {
        constexpr QSize guiDim(537, 637);
        this->setFixedSize(guiDim);
        this->setObjectName(QStringLiteral("StepByStepGUI"));
        this->setWindowTitle(QStringLiteral("Step-by-step solution"));
        this->setWindowIcon(QIcon(QStringLiteral(":/res/Qudoku.ico")));
        this->setStyleSheet(QStringLiteral("background: rgb(239, 239, 239)"));

        _stepsStack->setObjectName(QStringLiteral("stepsStack"));
        constexpr QRect stepsStackGeom(0, 0, 537, 587);
        _stepsStack->setGeometry(stepsStackGeom);
        _stepsStack->installEventFilter(this);

        constexpr QRect messageLabelGeom(25, 0, 512, 50);
        const QFont messageFont(QStringLiteral("Open Sans"), 12, QFont::Bold, false);
        constexpr QRect stepFieldsGeom(0, 50, 537, 537);
        const QString msg = this->getStepMsg(_currentStep);

        _fieldsWidget->setGeometry(stepFieldsGeom);

        _msgLabel->setObjectName(QStringLiteral("msgLabel"));
        _msgLabel->setGeometry(messageLabelGeom);
        _msgLabel->setStyleSheet(QStringLiteral("color: black; background-color: rgba(239, 239, 239, 0.0)"));
        _msgLabel->setFont(messageFont);
        _msgLabel->setAlignment(Qt::AlignCenter);
        _msgLabel->setText(msg);

        _previewLabel->setObjectName(QStringLiteral("previewLabel"));
        constexpr QRect previewLabelGeom(0, 0, 100, 25);
        _previewLabel->setGeometry(previewLabelGeom);
        _previewLabel->setStyleSheet(QStringLiteral("color: rgb(239, 239, 239); background-color: rgba(100, 100, 100, 1.0)"));
        _previewLabel->setFont(messageFont);
        _previewLabel->setAlignment(Qt::AlignCenter);
        _previewLabel->setText(QStringLiteral("Preview"));
        _previewLabel->hide();

        SolvedGUI::drawFields(_sudoku->getSteps()->at(_currentStep), _sudoku->getSteps()->at(_currentStep), _initVals, _fieldsWidget);
        SolvedGUI::drawFrame(_fieldsWidget);

        // Scrollbar to scroll through steps
        _stepsScrollBar->setObjectName(QStringLiteral("stepsScrollBar"));
        constexpr QRect stepsScrollBarGeom(0, 587, 537, 15);
        _stepsScrollBar->setGeometry(stepsScrollBarGeom);
        _stepsScrollBar->setContextMenuPolicy(Qt::NoContextMenu);
        _stepsScrollBar->setRange(0, static_cast<uint8_t>(2) * static_cast<uint8_t>(_sudoku->getSteps()->size()) - 2);
        _stepsScrollBar->setPageStep(1);
        QObject::connect(
            _stepsScrollBar, &QScrollBar::valueChanged, this, [this](int value)
            {
                if (value < _currentPage)
                {
                    this->drawPrevStep();
                }
                else if (value > _currentPage)
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

        _firstButton->setIconSize(buttonIconSize);
        _firstButton->setObjectName(QStringLiteral("firstButton"));
        constexpr QRect firstButtonGeom(0, 602, 134, 35);
        _firstButton->setGeometry(firstButtonGeom);
        _firstButton->setFont(buttonFont);
        _firstButton->setStyleSheet(buttonStyleSheet);
        _firstButton->setShortcut(QKeySequence(Qt::Key_Home, 0, 0, 0));
        QObject::connect(
            _firstButton, &QPushButton::pressed, this, [this]()
            {
                _currentStep = 0;
                const QString msg = this->getStepMsg(_currentStep);
                _msgLabel->setText(msg);
                SolvedGUI::drawFields(_sudoku->getSteps()->front(), _sudoku->getSteps()->front(), _initVals, _fieldsWidget);
                _currentPage = 0;
                _stepsScrollBar->setValue(_currentPage);
                _isPreview = false;
                _previewLabel->hide(); },
            Qt::AutoConnection);

        _prevButton->setIconSize(buttonIconSize);
        _prevButton->setObjectName(QStringLiteral("prevButton"));
        constexpr QRect prevButtonGeom(134, 602, 134, 35);
        _prevButton->setGeometry(prevButtonGeom);
        _prevButton->setFont(buttonFont);
        _prevButton->setStyleSheet(buttonStyleSheet);
        _prevButton->setAutoRepeat(true);
        _prevButton->setShortcut(QKeySequence(Qt::Key_PageUp, 0, 0, 0));
        QObject::connect(_prevButton, &QPushButton::clicked, this, &StepByStepGUI::drawPrevStep, Qt::AutoConnection);

        _nextButton->setIconSize(buttonIconSize);
        _nextButton->setObjectName(QStringLiteral("nextButton"));
        constexpr QRect nextButtonGeom(269, 602, 134, 35);
        _nextButton->setGeometry(nextButtonGeom);
        _nextButton->setFont(buttonFont);
        _nextButton->setStyleSheet(buttonStyleSheet);
        _nextButton->setAutoRepeat(true);
        _nextButton->setShortcut(QKeySequence(Qt::Key_PageDown, 0, 0, 0));
        QObject::connect(_nextButton, &QPushButton::clicked, this, &StepByStepGUI::drawNextStep, Qt::AutoConnection);

        _lastButton->setIconSize(buttonIconSize);
        _lastButton->setObjectName(QStringLiteral("lastButton"));
        constexpr QRect lastButtonGeom(403, 602, 134, 35);
        _lastButton->setGeometry(lastButtonGeom);
        _lastButton->setFont(buttonFont);
        _lastButton->setStyleSheet(buttonStyleSheet);
        _lastButton->setShortcut(QKeySequence(Qt::Key_End, 0, 0, 0));
        QObject::connect(
            _lastButton, &QPushButton::pressed, this, [this]()
            {
                _currentStep = _sudoku->getSteps()->size()-1;
                const QString msg = this->getStepMsg(_currentStep);
                _msgLabel->setText(msg);
                SolvedGUI::drawFields(_sudoku->getSteps()->back(), _sudoku->getSteps()->back(), _initVals, _fieldsWidget);
                _currentPage = 2 * static_cast<uint8_t>(_sudoku->getSteps()->size()) - 2;
                _stepsScrollBar->setValue(_currentPage);
                _isPreview = false;
                _previewLabel->hide(); },
            Qt::AutoConnection);

        _exportButton->setObjectName(QStringLiteral("exportButton"));
        constexpr QRect tehniquesButtonGeom(437, 0, 100, 25);
        _exportButton->setGeometry(tehniquesButtonGeom);
        _exportButton->setFont(buttonFont);
        _exportButton->setStyleSheet(buttonStyleSheet);
        _exportButton->setText(QStringLiteral("&Export"));
    }

#pragma clang diagnostic push
#pragma ide diagnostic ignored "ConstantConditionsOC"
#pragma ide diagnostic ignored "UnreachableCode"
    void StepByStepGUI::drawPrevStep()
    {
        if (_currentStep >= 0)
        {
            if (_currentStep == 0)
            {
                _isPreview = false;
                _currentPage = 0;
            }
            else
            {
                _isPreview = !_isPreview;
                _currentPage -= 1;
            }
            _stepsScrollBar->setValue(_currentPage);
            const QString msg = this->getStepMsg(_currentStep);
            _msgLabel->setText(msg);
            if (!_isPreview) // Show executed prev step
            {
                _previewLabel->hide();
                SolvedGUI::drawFields(_sudoku->getSteps()->at(_currentStep), _sudoku->getSteps()->at(_currentStep), _initVals, _fieldsWidget);
            }
            else if (_currentStep > 0) // Show preview of prev step
            {
                _previewLabel->show();
                _currentStep -= 1;
                SolvedGUI::drawFields(_sudoku->getSteps()->at(_currentStep), _sudoku->getSteps()->at(_currentStep + 1), _initVals, _fieldsWidget);
            }
        }
    }

    void StepByStepGUI::drawNextStep()
    {
        if (_currentStep < _sudoku->getSteps()->size() - 1)
        {
            _isPreview = !_isPreview;
            const QString msg = this->getStepMsg(_currentStep + 1);
            _msgLabel->setText(msg);
            if (_isPreview) // Show preview of next step
            {
                _previewLabel->show();
                SolvedGUI::drawFields(_sudoku->getSteps()->at(_currentStep), _sudoku->getSteps()->at(_currentStep + 1), _initVals, _fieldsWidget);
            }
            else // Show executed next step
            {
                _previewLabel->hide();
                _currentStep += 1;
                SolvedGUI::drawFields(_sudoku->getSteps()->at(_currentStep), _sudoku->getSteps()->at(_currentStep), _initVals, _fieldsWidget);
            }
            _currentPage += 1;
            _stepsScrollBar->setValue(_currentPage);
        }
    }
#pragma clang diagnostic pop

    auto StepByStepGUI::getStepMsg(uint8_t step) const -> QString
    {
        return "Run " + QString::number(*_sudoku->getSteps()->at(step).getFoundInRunNo(), global::base) + "/" +
               QString::number(*_sudoku->getSteps()->back().getFoundInRunNo(), global::base) + ", Step " +
               QString::number(step, global::base) + " of " + QString::number(_sudoku->getSteps()->size() - 1, global::base) + ":\n" +
               QString::fromStdString(*_sudoku->getSteps()->at(step).getFoundByType());
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
        if (watched == _stepsStack && event->type() == QEvent::Wheel)
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
