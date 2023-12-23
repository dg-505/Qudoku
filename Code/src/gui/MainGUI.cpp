#include <QtCore/QSettings>
#include <QtCore/QTextStream>
#include <QtGui/QIcon>
#include <QtGui/QShortcut>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>

#include "globals.h"
#include "gui/CandidatesGUI.h"
#include "gui/MainGUI.h"
#include "gui/SolvedGUI.h"
#include "gui/StepByStepGUI.h"
#include "gui/TechniquesDialog.h"

namespace sudoku
{
    // Main GUI setup
    MainGUI::MainGUI(const std::string& version, QWidget* parent)
        : QMainWindow(parent, Qt::WindowFlags()),
          _logScrollArea(new QScrollArea(this)),
          _logTextBrowser(new QLogTextBrowser(_logScrollArea)),
          _titleLabel(new QLabel(this, Qt::WindowFlags())),
          _gridWidget(new QWidget(this, Qt::WindowFlags())),
          _fields(new std::array<QInputField*, static_cast<uint8_t>(global::order* global::order)>),
          _techniquesButton(new QPushButton(this)),
          _loadButton(new QPushButton(this)),
          _saveButton(new QPushButton(this)),
          _candidatesButton(new QPushButton(this)),
          _stepByStepButton(new QPushButton(this)),
          _solveButton(new QPushButton(this)),
          _clearButton(new QPushButton(this)),
          _quitButton(new QPushButton(this))
    {
        // Main window properties
        constexpr QSize guiDim(1025, 637);
        this->setFixedSize(guiDim);
        this->setObjectName("MainGUI");
        const std::string title = "Qudoku - Qt based cross platform sudoku solver (Version " + version + ")";
        this->setWindowTitle(QString::fromStdString(title));
        this->setWindowIcon(QIcon(QStringLiteral(":/res/Qudoku.ico")));
        this->setStyleSheet(QStringLiteral("background: rgb(239, 239, 239)"));

        const QString buttonStyleSheet = QStringLiteral("QPushButton {color: black; background: rgb(239, 239, 239)}"
                                                        "QPushButton:hover {color: black; background: rgb(171, 171, 171)}"
                                                        "QPushButton:pressed {color: black; background: rgb(171, 171, 171)}");

        // Log scroll area and text browser
        const QFont logFont(QStringLiteral("Liberation Mono"), 10, QFont::Bold, false);
        
        this->_logScrollArea->setObjectName("logScrollArea");
        constexpr QRect logScrollAreaGeom(537, 0, 488, 637);
        this->_logScrollArea->setGeometry(logScrollAreaGeom);

        this->_logTextBrowser->setObjectName("logTextBrowser");
        constexpr QRect logTextBrowserGeom(0, 0, 488, 637);
        this->_logTextBrowser->setGeometry(logTextBrowserGeom);
        this->_logTextBrowser->setStyleSheet(QStringLiteral("color: black; background: white"));
        this->_logTextBrowser->setWordWrapMode(QTextOption::NoWrap);
        this->_logTextBrowser->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        this->_logTextBrowser->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        this->_logTextBrowser->setFont(logFont);

        // Title label
        this->_titleLabel->setObjectName("titleLabel");
        constexpr QRect titleLabelGeom(0, 0, 367, 50);
        this->_titleLabel->setGeometry(titleLabelGeom);
        this->_titleLabel->setStyleSheet(QStringLiteral("color: black; background: rgb(239, 239, 239)"));
        const QFont titleFont(QStringLiteral("Open Sans"), 14, QFont::Bold, false);
        this->_titleLabel->setFont(titleFont);
        this->_titleLabel->setAlignment(Qt::AlignCenter);
        this->_titleLabel->setText(QStringLiteral("Please fill in the initially given fields"));

        // Set up the fields
        this->_gridWidget->setObjectName("gridWidget");
        constexpr QRect gridWidgetGeom(0, 50, 537, 537);
        this->_gridWidget->setGeometry(gridWidgetGeom);

        const QFont fieldsFont(QStringLiteral("Liberation Mono"), 32, QFont::Bold, false);
        const QFont rcLabelFont(QStringLiteral("Open Sans"), 16, QFont::Bold, false);

        uint8_t fID = 1;
        constexpr uint8_t offset = 27;
        uint16_t posY = offset;
        for (uint8_t rID = 1; rID <= global::order; rID++)
        {
            auto* rLabel = std::make_unique<QLabel>(_gridWidget, Qt::WindowFlags()).release();
            rLabel->setObjectName("rLabel" + QString::number(rID, global::base));
            const QRect rLabelGeom(posY, 1, global::fieldDim, 25);
            rLabel->setGeometry(rLabelGeom);
            rLabel->setStyleSheet(QStringLiteral("color: rgb(100, 100, 100); background-color: rgba(239, 239, 239, 1.0); border: 1px solid rgb(171, 171, 171)"));
            rLabel->setFont(rcLabelFont);
            rLabel->setAlignment(Qt::AlignCenter);
            rLabel->setText(QString::number(rID, global::base));

            auto* cLabel = std::make_unique<QLabel>(_gridWidget, Qt::WindowFlags()).release();
            cLabel->setObjectName("rLabel" + QString::number(rID, global::base));
            const QRect cLabelGeom(1, posY, 25, global::fieldDim);
            cLabel->setGeometry(cLabelGeom);
            cLabel->setStyleSheet(QStringLiteral("color: rgb(100, 100, 100); background-color: rgba(239, 239, 239, 1.0); border: 1px solid rgb(171, 171, 171)"));
            cLabel->setFont(rcLabelFont);
            cLabel->setAlignment(Qt::AlignCenter);
            cLabel->setText(QString::number(rID, global::base));

            uint16_t posX = offset;
#pragma unroll static_cast < short>(global::order)
            for (uint8_t cID = 1; cID <= global::order; cID++)
            {
                auto* field = std::make_unique<QInputField>(_gridWidget).release();
                field->setObjectName("field" + QString::number(fID, global::base));
                field->setGeometry(posX, posY, global::fieldDim, global::fieldDim);
                field->setStyleSheet(QStringLiteral("QLineEdit {color: black; background: white} QLineEdit:focus{color: black; background: white; border: 1px solid black}"));
                field->setFont(fieldsFont);
                field->setMaxLength(1);
                field->setAlignment(Qt::AlignCenter);
                this->_fields->at(fID - 1) = field;
                fID++;
                posX += global::fieldDim;
                if (cID % 3 == 0)
                {
                    posX += 2;
                }
            }
            posY += global::fieldDim;
            if (rID % 3 == 0)
            {
                posY += 2;
            }
        }

        // Buttons
        const QFont buttonFont(QStringLiteral("Open Sans"), 11, QFont::Bold, false);

        this->_techniquesButton->setObjectName("techniquesButton");
        constexpr QRect tehniquesButtonGeom(367, 0, 100, 50);
        this->_techniquesButton->setGeometry(tehniquesButtonGeom);
        this->_techniquesButton->setFont(buttonFont);
        this->_techniquesButton->setStyleSheet(buttonStyleSheet);
        this->_techniquesButton->setText(QStringLiteral("Select\n&techniques"));
        this->_techniquesButton->setShortcut(QKeySequence(Qt::ALT | Qt::Key_T, QKeyCombination::fromCombined(0), QKeyCombination::fromCombined(0), QKeyCombination::fromCombined(0)));
        MainGUI::connect(_techniquesButton, &QPushButton::clicked, this, &MainGUI::techniquesButtonClicked, Qt::AutoConnection);
        
        this->_loadButton->setObjectName("loadButton");
        constexpr QRect loadButtonGeom(467, 0, 70, 25);
        this->_loadButton->setGeometry(loadButtonGeom);
        this->_loadButton->setFont(buttonFont);
        this->_loadButton->setStyleSheet(buttonStyleSheet);
        this->_loadButton->setText(QStringLiteral("Lo&ad"));
        this->_loadButton->setShortcut(QKeySequence(Qt::ALT | Qt::Key_A, QKeyCombination::fromCombined(0), QKeyCombination::fromCombined(0), QKeyCombination::fromCombined(0)));
        MainGUI::connect(_loadButton, &QPushButton::clicked, this, &MainGUI::loadButtonClicked, Qt::AutoConnection);

        // validateButton = new QPushButton(this);
        // validateButton->setObjectName("validateButton");
        // validateButton->setGeometry(412, 17, 100, 16);
        // validateButton->setFont(buttonFont);
        // validateButton->setText("Validate");
        
        this->_saveButton->setObjectName("saveButton");
        constexpr QRect saveButtonGeom(467, 25, 70, 25);
        this->_saveButton->setGeometry(saveButtonGeom);
        this->_saveButton->setFont(buttonFont);
        this->_saveButton->setStyleSheet(buttonStyleSheet);
        this->_saveButton->setText(QStringLiteral("&Save"));
        this->_saveButton->setShortcut(QKeySequence(Qt::ALT | Qt::Key_S, QKeyCombination::fromCombined(0), QKeyCombination::fromCombined(0), QKeyCombination::fromCombined(0)));
        MainGUI::connect(_saveButton, &QPushButton::clicked, this, &MainGUI::saveButtonClicked, Qt::AutoConnection);

        this->_candidatesButton->setObjectName("candidatesButton");
        constexpr QRect candidatesButtonGeom(0, 587, 107, 50);
        this->_candidatesButton->setGeometry(candidatesButtonGeom);
        this->_candidatesButton->setFont(buttonFont);
        this->_candidatesButton->setStyleSheet(buttonStyleSheet);
        this->_candidatesButton->setText(QStringLiteral("&Candidates"));
        this->_candidatesButton->setShortcut(QKeySequence(Qt::ALT | Qt::Key_C, QKeyCombination::fromCombined(0), QKeyCombination::fromCombined(0), QKeyCombination::fromCombined(0)));
        MainGUI::connect(_candidatesButton, &QPushButton::clicked, this, &MainGUI::candidatesButtonClicked, Qt::AutoConnection);

        this->_stepByStepButton->setObjectName("stepByStepButton");
        constexpr QRect stepByStepButtonGeom(107, 587, 108, 50);
        this->_stepByStepButton->setGeometry(stepByStepButtonGeom);
        this->_stepByStepButton->setFont(buttonFont);
        this->_stepByStepButton->setStyleSheet(buttonStyleSheet);
        this->_stepByStepButton->setText(QStringLiteral("St&ep by Step"));
        this->_stepByStepButton->setShortcut(QKeySequence(Qt::ALT | Qt::Key_E, QKeyCombination::fromCombined(0), QKeyCombination::fromCombined(0), QKeyCombination::fromCombined(0)));
        MainGUI::connect(_stepByStepButton, &QPushButton::clicked, this, &MainGUI::stepByStepButtonClicked, Qt::AutoConnection);
        
        this->_solveButton->setObjectName("solveButton");
        constexpr QRect solveButtonGeom(215, 587, 107, 50);
        this->_solveButton->setGeometry(solveButtonGeom);
        this->_solveButton->setFont(buttonFont);
        this->_solveButton->setStyleSheet(buttonStyleSheet);
        this->_solveButton->setText(QStringLiteral("Sol&ve"));
        this->_solveButton->setShortcut(QKeySequence(Qt::ALT | Qt::Key_V, QKeyCombination::fromCombined(0), QKeyCombination::fromCombined(0), QKeyCombination::fromCombined(0)));
        MainGUI::connect(_solveButton, &QPushButton::clicked, this, &MainGUI::solveButtonClicked, Qt::AutoConnection);
        
        this->_clearButton->setObjectName("clearButton");
        constexpr QRect clearButtonGeom(322, 587, 108, 50);
        this->_clearButton->setGeometry(clearButtonGeom);
        this->_clearButton->setFont(buttonFont);
        this->_clearButton->setStyleSheet(buttonStyleSheet);
        this->_clearButton->setText(QStringLiteral("Clea&r"));
        this->_clearButton->setShortcut(QKeySequence(Qt::ALT | Qt::Key_R, QKeyCombination::fromCombined(0), QKeyCombination::fromCombined(0), QKeyCombination::fromCombined(0)));
        MainGUI::connect(_clearButton, &QPushButton::clicked, this, &MainGUI::clearButtonClicked, Qt::AutoConnection);
        
        this->_quitButton->setObjectName("quitButton");
        constexpr QRect quitButtonGeom(430, 587, 107, 50);
        this->_quitButton->setGeometry(quitButtonGeom);
        this->_quitButton->setFont(buttonFont);
        this->_quitButton->setStyleSheet(buttonStyleSheet);
        this->_quitButton->setText(QStringLiteral("&Quit"));
        this->_quitButton->setShortcut(QKeySequence(Qt::ALT | Qt::Key_Q, QKeyCombination::fromCombined(0), QKeyCombination::fromCombined(0), QKeyCombination::fromCombined(0)));
        MainGUI::connect(_quitButton, &QPushButton::clicked, [=]()
                         { QApplication::quit(); });

        SolvedGUI::drawFrame(_gridWidget);

        // center window
        this->move(screen()->geometry().center() - frameGeometry().center());
        
        MainGUI::setTabOrder(_loadButton, _saveButton);
        MainGUI::setTabOrder(_saveButton, _fields->at(0));
#pragma unroll static_cast < short>(global::order * global::order)
        for (uint8_t fID_ = 1; fID_ < global::order * global::order; fID_++)
        {
            MainGUI::setTabOrder(_fields->at(fID_ - 1), _fields->at(fID_));
        }
        MainGUI::setTabOrder(_fields->at((global::order * global::order) - 1), _candidatesButton);
        MainGUI::setTabOrder(_candidatesButton, _stepByStepButton);
        MainGUI::setTabOrder(_stepByStepButton, _solveButton);
        MainGUI::setTabOrder(_solveButton, _clearButton);
        MainGUI::setTabOrder(_clearButton, _quitButton);
        MainGUI::setTabOrder(_quitButton, _loadButton);
    }

    void MainGUI::keyPressEvent(QKeyEvent* event)
    {
        if (event->key() == Qt::Key_Escape)
        {
            this->close();
        }
    }

    // Button functions

    void MainGUI::techniquesButtonClicked()
    {
        std::unique_ptr<TechniquesDialog> techniquesDialog = std::make_unique<TechniquesDialog>(this->_nakedSinglesEnabled, this->_hiddenSinglesEnabled, this->_nakedPairsEnabled, this->_hiddenPairsEnabled, this->_nakedTriplesEnabled, this->_hiddenTriplesEnabled, this->_blockLineChecksEnabled, this->_lineBlockChecksEnabled, this->_backtrackingEnabled, this);
        if (techniquesDialog->exec() == QDialog::Accepted)
        {
            this->_logTextBrowser->clear();
            this->_logTextBrowser->append(QStringLiteral("Selected solving techniques:"));
            this->_logTextBrowser->append(QStringLiteral("Naked Singles: ") + (this->_nakedSinglesEnabled ? QStringLiteral("          true") : QStringLiteral("          false")));
            this->_logTextBrowser->append(QStringLiteral("Hidden Singles: ") + (this->_hiddenSinglesEnabled ? QStringLiteral("         true") : QStringLiteral("         false")));
            this->_logTextBrowser->append(QStringLiteral("Naked Pairs: ") + (this->_nakedPairsEnabled ? QStringLiteral("            true") : QStringLiteral("            false")));
            this->_logTextBrowser->append(QStringLiteral("Hidden Pairs: ") + (this->_hiddenPairsEnabled ? QStringLiteral("           true") : QStringLiteral("           false")));
            this->_logTextBrowser->append(QStringLiteral("Naked Triples: ") + (this->_nakedTriplesEnabled ? QStringLiteral("          true") : QStringLiteral("          false")));
            this->_logTextBrowser->append(QStringLiteral("Hidden Triples: ") + (this->_hiddenTriplesEnabled ? QStringLiteral("         true") : QStringLiteral("         false")));
            this->_logTextBrowser->append(QStringLiteral("Block-Line-Interactions: ") + (this->_blockLineChecksEnabled ? QStringLiteral("true") : QStringLiteral("false")));
            this->_logTextBrowser->append(QStringLiteral("Line-Block-Interactions: ") + (this->_lineBlockChecksEnabled ? QStringLiteral("true") : QStringLiteral("false")));
            this->_logTextBrowser->append(QStringLiteral("Try & Error: ") + (this->_backtrackingEnabled ? QStringLiteral("            true") : QStringLiteral("            false")));
        }
    }

    auto MainGUI::loadButtonClicked() -> std::string
    {
        const QString msgBoxStyleSheet = QStringLiteral("color: black; background: rgb(239, 239, 239)");
        // Read the data directory from the Qudoku.ini file created during installation
        const QString filepath = QFileDialog::getOpenFileName(this, QStringLiteral("Open Sudoku from file"), (QSettings(QStringLiteral("./Qudoku.ini"), QSettings::IniFormat, nullptr)).value("DIRS/DataDir").toString(), QString(), nullptr, QFileDialog::Options());
        if (filepath.isEmpty())
        {
            // User cancelled file selection
            this->_logTextBrowser->append(QStringLiteral("File opening cancelled!"));
            return {};
        }

        QFile file(filepath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            this->_logTextBrowser->append(QStringLiteral("Failed to open file:"));
            return {};
        }

        QTextStream loadFile(&file);
        const QString sudokuString = loadFile.readAll().trimmed();

        if (sudokuString.isEmpty())
        {
            QMessageBox msgBox(QMessageBox::Critical, QStringLiteral("Empty file"), QStringLiteral("Error reading \"") + filepath + QStringLiteral("\":\nEmpty file."), QMessageBox::Ok, this->_gridWidget, Qt::WindowFlags());
            msgBox.setStyleSheet(msgBoxStyleSheet);
            msgBox.exec();
#pragma unroll static_cast < short>(global::order * global::order)
            for (auto* inputField : *this->_fields)
            {
                inputField->clear();
            }
            return {};
        }

        if (sudokuString.length() != static_cast<uint8_t>(global::order * global::order))
        {
            QMessageBox msgBox(QMessageBox::Critical, QStringLiteral("Empty file"), QStringLiteral("Error reading \"") + filepath + QStringLiteral("\":\nEmpty file."), QMessageBox::Ok, this->_gridWidget, Qt::WindowFlags());
            msgBox.setStyleSheet(msgBoxStyleSheet);
            msgBox.exec();
#pragma unroll static_cast < short>(global::order * global::order)
            for (auto* inputField : *this->_fields)
            {
                inputField->clear();
            }
            return {};
        }

        for (uint8_t fID = 1; fID <= global::order * global::order; fID++)
        {
            const QChar inputChar = sudokuString.at(fID - 1);
            if (inputChar.isDigit())
            {
                const uint8_t value = inputChar.digitValue();
                if (value == 0)
                {
                    QMessageBox msgBox(QMessageBox::Critical, QStringLiteral("Empty file"), QStringLiteral("Error reading \"") + filepath + QStringLiteral("\":\nEmpty file."), QMessageBox::Ok, this->_gridWidget, Qt::WindowFlags());
                    msgBox.setStyleSheet(msgBoxStyleSheet);
                    msgBox.exec();
#pragma unroll static_cast < short>(global::order * global::order)
                    for (auto* inputField : *this->_fields)
                    {
                        inputField->clear();
                    }
                    return {};
                }
                this->_fields->at(fID - 1)->setText(inputChar);
            }
            else if (inputChar == '.')
            {
                this->_fields->at(fID - 1)->clear();
            }
            else
            {
                QMessageBox msgBox(QMessageBox::Critical, QStringLiteral("Empty file"), QStringLiteral("Error reading \"") + filepath + QStringLiteral("\":\nEmpty file."), QMessageBox::Ok, this->_gridWidget, Qt::WindowFlags());
                msgBox.setStyleSheet(msgBoxStyleSheet);
                msgBox.exec();
#pragma unroll static_cast < short>(global::order * global::order)
                for (auto* inputField : *this->_fields)
                {
                    inputField->clear();
                }
                return {};
            }
        }

        const QString extraLine = loadFile.readLine(0);

        if (!extraLine.isEmpty())
        {
            QMessageBox msgBox(QMessageBox::Warning, QStringLiteral("Empty file"), QStringLiteral("Error reading \"") + filepath + QStringLiteral("\":\nEmpty file."), QMessageBox::Ok, this->_gridWidget, Qt::WindowFlags());
            msgBox.setStyleSheet(msgBoxStyleSheet);
            msgBox.exec();
        }

        // extract the raw file name
        // size_t const lastSeparator = filepath.toStdString().find_last_of("/\\");
        // size_t const lastDot = filepath.toStdString().find_last_of('.');
        // filename = filepath.toStdString().substr(lastSeparator + 1, lastDot - lastSeparator - 1);

        this->_logTextBrowser->clear();
        this->_logTextBrowser->append("Sudoku \"" + filepath + "\" successfully loaded");
        file.close();
        
        return _filename;
    }

    void MainGUI::saveButtonClicked() const
    {
        // Read the data directory from the Qudoku.ini file created during installation
        const QSettings settings(QStringLiteral("./Qudoku.ini"), QSettings::IniFormat, nullptr);
        const QString dataDir = settings.value("DIRS/DataDir").toString();
        const QString filepath = QFileDialog::getSaveFileName(this->centralWidget(), QStringLiteral("Save Sudoku to file"), dataDir, QString(), nullptr, QFileDialog::Options());

        if (filepath.isEmpty())
        {
            // User cancelled file selection
            return;
        }

        QFile file(filepath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            this->_logTextBrowser->append(QStringLiteral("Failed to open file for writing:"));
            return;
        }

        QTextStream out(&file);

#pragma unroll static_cast < short>(global::order * global::order)
        for (uint8_t fID = 1; fID <= global::order * global::order; fID++)
        {
            QString const val = _fields->at(fID - 1)->text();
            if (val.isEmpty())
            {
                out << ".";
            }
            else
            {
                out << val;
            }
        }
        out << "\n";
        file.close();

        this->_logTextBrowser->append("Saved to \"" + filepath + "\"");
    }

    void MainGUI::candidatesButtonClicked()
    {
        std::array<uint8_t, static_cast<uint8_t>(global::order * global::order)> initVals{};
        auto* sudoku = init(&initVals);
        auto* candidatesGUI = std::make_unique<CandidatesGUI>(sudoku, initVals, this).release();
        const QPoint candidatesGUIpos(this->pos().x(), this->pos().y() + 50);
        candidatesGUI->move(candidatesGUIpos);
        candidatesGUI->show();
    }

    void MainGUI::stepByStepButtonClicked()
    {
        this->_logTextBrowser->clear();
        this->_logTextBrowser->append(QStringLiteral("Solving in progress. Please wait..."));
        this->_logTextBrowser->repaint();

        auto loadingLabel = std::make_unique<QLabel>(this->_gridWidget, Qt::WindowFlags());
        loadingLabel->setStyleSheet(QStringLiteral("background: rgba(0, 0, 0, 0)"));
        constexpr QSize loadingLabelSize(100, 100);
        loadingLabel->setFixedSize(loadingLabelSize);
        loadingLabel->setPixmap(QPixmap(QStringLiteral(":/res/loading.png"), "png", Qt::AutoColor).scaled(loadingLabelSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        constexpr uint16_t pos = 281;
        loadingLabel->move(pos - loadingLabel->width() / 2, pos - loadingLabel->height() / 2);
        loadingLabel->show();
        this->repaint();

        std::array<uint8_t, static_cast<uint8_t>(global::order * global::order)> initVals{};

        //        WorkerThread* workerThread = new WorkerThread(this->fields, initVals, this->logTextBrowser, this->nakedSinglesEnabled, this->hiddenSinglesEnabled, this->nakedPairsEnabled, this->hiddenPairsEnabled, this->nakedTriplesEnabled, this->hiddenTriplesEnabled, this->blockLineChecksEnabled, this->lineBlockChecksEnabled, this->backtrackingEnabled);
        //        connect(workerThread, &WorkerThread::resultReady, this, &MainGUI::handleResults);
        //        connect(workerThread, &WorkerThread::finished, workerThread, &QObject::deleteLater);
        //        workerThread->start();

        auto* sudoku = init(&initVals);
        sudoku->solve(_filename);
        auto* stepByStepGUI = std::make_unique<StepByStepGUI>(sudoku, initVals, this).release();
        stepByStepGUI->move(QPoint(this->pos().x(), this->pos().y()));
        stepByStepGUI->show();
    }

    void MainGUI::solveButtonClicked()
    {
        this->_logTextBrowser->clear();
        this->_logTextBrowser->append(QStringLiteral("Solving in progress. Please wait..."));
        this->_logTextBrowser->repaint();

        auto loadingLabel = std::make_unique<QLabel>(this->_gridWidget, Qt::WindowFlags());
        loadingLabel->setStyleSheet(QStringLiteral("background: rgba(0, 0, 0, 0)"));
        constexpr QSize loadingLabelSize(100, 100);
        loadingLabel->setFixedSize(loadingLabelSize);
        loadingLabel->setPixmap(QPixmap(QStringLiteral(":/res/loading.png"), "png", Qt::AutoColor).scaled(loadingLabelSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        constexpr uint16_t pos = 281;
        loadingLabel->move(pos - loadingLabel->width() / 2, pos - loadingLabel->height() / 2);
        loadingLabel->show();
        this->repaint();

        std::array<uint8_t, static_cast<uint8_t>(global::order * global::order)> initVals{};
        auto* sudoku = init(&initVals);
        sudoku->solve(_filename);
        auto* solvedGUI = std::make_unique<SolvedGUI>(sudoku, initVals, this).release();
        const QPoint solvedGUIpos(this->pos().x(), this->pos().y() + 50);
        solvedGUI->move(solvedGUIpos);
        solvedGUI->show();
    }

    //    void MainGUI::handleResults()
    //    {
    //        this->logTextBrowser->append(QStringLiteral("Handling Results..."));
    //    }

    void MainGUI::clearButtonClicked()
    {
        this->clear();
    }

    void MainGUI::closeEvent(QCloseEvent* /*event*/)
    {
        QApplication::quit();
    }

    // Other functions

    void MainGUI::clear()
    {
#pragma unroll static_cast < short>(global::order * global::order)
        for (auto* inputField : *this->_fields)
        {
            inputField->clear();
        }
        this->_logTextBrowser->clear();
        _filename = "";
    }

    auto MainGUI::init(std::array<uint8_t, static_cast<uint8_t>(global::order* global::order)>* initVals) const -> Sudoku*
    {
#pragma unroll static_cast < short>(global::order * global::order)
        for (uint8_t i = 1; i <= global::order * global::order; i++)
        {
            try
            {
                initVals->at(i - 1) = static_cast<uint8_t>(std::stoi(this->_fields->at(i - 1)->text().toStdString(), nullptr, global::base));
            }
            catch (const std::exception& e)
            {
                initVals->at(i - 1) = 0;
            }
        }
        return std::make_unique<Sudoku>(initVals, this->_logTextBrowser, this->_nakedSinglesEnabled, this->_hiddenSinglesEnabled, this->_nakedPairsEnabled, this->_hiddenPairsEnabled, this->_nakedTriplesEnabled, this->_hiddenTriplesEnabled, this->_blockLineChecksEnabled, this->_lineBlockChecksEnabled, this->_backtrackingEnabled).release();
    }
} // namespace sudoku
