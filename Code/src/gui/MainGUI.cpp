#include <QtCore/QSettings>
#include <QtCore/QTextStream>
#include <QtGui/QIcon>
#include <QtGui/QShortcut>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>

#include "globals.h"
#include "gui/MainGUI.h"
#include "gui/TechniquesDialog.h"

namespace sudoku
{
    // Main GUI setup
    MainGUI::MainGUI(const std::string& version, QWidget* parent)
        : QMainWindow(parent, Qt::WindowFlags()),
          logScrollArea(new QScrollArea(this)),
          logTextBrowser(new QLogTextBrowser(logScrollArea)),
          titleLabel(new QLabel(this, Qt::WindowFlags())),
          gridWidget(new QWidget(this, Qt::WindowFlags())),
          fields(new std::array<QInputField*, static_cast<uint8_t>(global::order* global::order)>),
          techniquesButton(new QPushButton(this)),
          loadButton(new QPushButton(this)),
          saveButton(new QPushButton(this)),
          candidatesButton(new QPushButton(this)),
          stepByStepButton(new QPushButton(this)),
          solveButton(new QPushButton(this)),
          clearButton(new QPushButton(this)),
          quitButton(new QPushButton(this))
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

        this->logScrollArea->setObjectName("logScrollArea");
        constexpr QRect logScrollAreaGeom(537, 0, 488, 637);
        this->logScrollArea->setGeometry(logScrollAreaGeom);

        this->logTextBrowser->setObjectName("logTextBrowser");
        constexpr QRect logTextBrowserGeom(0, 0, 488, 637);
        this->logTextBrowser->setGeometry(logTextBrowserGeom);
        this->logTextBrowser->setStyleSheet(QStringLiteral("color: black; background: white"));
        this->logTextBrowser->setWordWrapMode(QTextOption::NoWrap);
        this->logTextBrowser->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        this->logTextBrowser->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        this->logTextBrowser->setFont(logFont);

        // Title label
        this->titleLabel->setObjectName("titleLabel");
        constexpr QRect titleLabelGeom(0, 0, 367, 50);
        this->titleLabel->setGeometry(titleLabelGeom);
        this->titleLabel->setStyleSheet(QStringLiteral("color: black; background: rgb(239, 239, 239)"));
        const QFont titleFont(QStringLiteral("Open Sans"), 14, QFont::Bold, false);
        this->titleLabel->setFont(titleFont);
        this->titleLabel->setAlignment(Qt::AlignCenter);
        this->titleLabel->setText(QStringLiteral("Please fill in the initially given fields"));

        // Set up the fields
        this->gridWidget->setObjectName("gridWidget");
        constexpr QRect gridWidgetGeom(0, 50, 537, 537);
        this->gridWidget->setGeometry(gridWidgetGeom);

        const QFont fieldsFont(QStringLiteral("Liberation Mono"), 32, QFont::Bold, false);
        const QFont rcLabelFont(QStringLiteral("Open Sans"), 16, QFont::Bold, false);

        uint8_t fID = 1;
        constexpr uint8_t offset = 27;
        uint16_t posY = offset;
        for (uint8_t rID = 1; rID <= global::order; rID++)
        {
            auto* rLabel = std::make_unique<QLabel>(gridWidget, Qt::WindowFlags()).release();
            rLabel->setObjectName("rLabel" + QString::number(rID, global::base));
            const QRect rLabelGeom(posY, 1, global::fieldDim, 25);
            rLabel->setGeometry(rLabelGeom);
            rLabel->setStyleSheet(QStringLiteral("color: rgb(100, 100, 100); background-color: rgba(239, 239, 239, 1.0); border: 1px solid rgb(171, 171, 171)"));
            rLabel->setFont(rcLabelFont);
            rLabel->setAlignment(Qt::AlignCenter);
            rLabel->setText(QString::number(rID, global::base));

            auto* cLabel = std::make_unique<QLabel>(gridWidget, Qt::WindowFlags()).release();
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
                auto* field = std::make_unique<QInputField>(gridWidget).release();
                field->setObjectName("field" + QString::number(fID, global::base));
                field->setGeometry(posX, posY, global::fieldDim, global::fieldDim);
                field->setStyleSheet(QStringLiteral("QLineEdit {color: black; background: white} QLineEdit:focus{color: black; background: white; border: 1px solid black}"));
                field->setFont(fieldsFont);
                field->setMaxLength(1);
                field->setAlignment(Qt::AlignCenter);
                this->fields->at(fID - 1) = field;
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

        this->techniquesButton->setObjectName("techniquesButton");
        constexpr QRect tehniquesButtonGeom(367, 0, 100, 50);
        this->techniquesButton->setGeometry(tehniquesButtonGeom);
        this->techniquesButton->setFont(buttonFont);
        this->techniquesButton->setStyleSheet(buttonStyleSheet);
        this->techniquesButton->setText(QStringLiteral("Select\n&techniques"));
        this->techniquesButton->setShortcut(QKeySequence(Qt::ALT | Qt::Key_T, QKeyCombination::fromCombined(0), QKeyCombination::fromCombined(0), QKeyCombination::fromCombined(0)));
        MainGUI::connect(techniquesButton, &QPushButton::clicked, this, &MainGUI::techniquesButtonClicked, Qt::AutoConnection);

        this->loadButton->setObjectName("loadButton");
        constexpr QRect loadButtonGeom(467, 0, 70, 25);
        this->loadButton->setGeometry(loadButtonGeom);
        this->loadButton->setFont(buttonFont);
        this->loadButton->setStyleSheet(buttonStyleSheet);
        this->loadButton->setText(QStringLiteral("Lo&ad"));
        this->loadButton->setShortcut(QKeySequence(Qt::ALT | Qt::Key_A, QKeyCombination::fromCombined(0), QKeyCombination::fromCombined(0), QKeyCombination::fromCombined(0)));
        MainGUI::connect(loadButton, &QPushButton::clicked, this, &MainGUI::loadButtonClicked, Qt::AutoConnection);

        // validateButton = new QPushButton(this);
        // validateButton->setObjectName("validateButton");
        // validateButton->setGeometry(412, 17, 100, 16);
        // validateButton->setFont(buttonFont);
        // validateButton->setText("Validate");

        this->saveButton->setObjectName("saveButton");
        constexpr QRect saveButtonGeom(467, 25, 70, 25);
        this->saveButton->setGeometry(saveButtonGeom);
        this->saveButton->setFont(buttonFont);
        this->saveButton->setStyleSheet(buttonStyleSheet);
        this->saveButton->setText(QStringLiteral("&Save"));
        this->saveButton->setShortcut(QKeySequence(Qt::ALT | Qt::Key_S, QKeyCombination::fromCombined(0), QKeyCombination::fromCombined(0), QKeyCombination::fromCombined(0)));
        MainGUI::connect(saveButton, &QPushButton::clicked, this, &MainGUI::saveButtonClicked, Qt::AutoConnection);

        this->candidatesButton->setObjectName("candidatesButton");
        constexpr QRect candidatesButtonGeom(0, 587, 107, 50);
        this->candidatesButton->setGeometry(candidatesButtonGeom);
        this->candidatesButton->setFont(buttonFont);
        this->candidatesButton->setStyleSheet(buttonStyleSheet);
        this->candidatesButton->setText(QStringLiteral("&Candidates"));
        this->candidatesButton->setShortcut(QKeySequence(Qt::ALT | Qt::Key_C, QKeyCombination::fromCombined(0), QKeyCombination::fromCombined(0), QKeyCombination::fromCombined(0)));
        MainGUI::connect(candidatesButton, &QPushButton::clicked, this, &MainGUI::candidatesButtonClicked, Qt::AutoConnection);

        this->stepByStepButton->setObjectName("stepByStepButton");
        constexpr QRect stepByStepButtonGeom(107, 587, 108, 50);
        this->stepByStepButton->setGeometry(stepByStepButtonGeom);
        this->stepByStepButton->setFont(buttonFont);
        this->stepByStepButton->setStyleSheet(buttonStyleSheet);
        this->stepByStepButton->setText(QStringLiteral("St&ep by Step"));
        this->stepByStepButton->setShortcut(QKeySequence(Qt::ALT | Qt::Key_E, QKeyCombination::fromCombined(0), QKeyCombination::fromCombined(0), QKeyCombination::fromCombined(0)));
        MainGUI::connect(stepByStepButton, &QPushButton::clicked, this, &MainGUI::stepByStepButtonButtonClicked, Qt::AutoConnection);

        this->solveButton->setObjectName("solveButton");
        constexpr QRect solveButtonGeom(215, 587, 107, 50);
        this->solveButton->setGeometry(solveButtonGeom);
        this->solveButton->setFont(buttonFont);
        this->solveButton->setStyleSheet(buttonStyleSheet);
        this->solveButton->setText(QStringLiteral("Sol&ve"));
        this->solveButton->setShortcut(QKeySequence(Qt::ALT | Qt::Key_V, QKeyCombination::fromCombined(0), QKeyCombination::fromCombined(0), QKeyCombination::fromCombined(0)));
        MainGUI::connect(solveButton, &QPushButton::clicked, this, &MainGUI::solveButtonClicked, Qt::AutoConnection);

        this->clearButton->setObjectName("clearButton");
        constexpr QRect clearButtonGeom(322, 587, 108, 50);
        this->clearButton->setGeometry(clearButtonGeom);
        this->clearButton->setFont(buttonFont);
        this->clearButton->setStyleSheet(buttonStyleSheet);
        this->clearButton->setText(QStringLiteral("Clea&r"));
        this->clearButton->setShortcut(QKeySequence(Qt::ALT | Qt::Key_R, QKeyCombination::fromCombined(0), QKeyCombination::fromCombined(0), QKeyCombination::fromCombined(0)));
        MainGUI::connect(clearButton, &QPushButton::clicked, this, &MainGUI::clearButtonClicked, Qt::AutoConnection);

        this->quitButton->setObjectName("quitButton");
        constexpr QRect quitButtonGeom(430, 587, 107, 50);
        this->quitButton->setGeometry(quitButtonGeom);
        this->quitButton->setFont(buttonFont);
        this->quitButton->setStyleSheet(buttonStyleSheet);
        this->quitButton->setText(QStringLiteral("&Quit"));
        this->quitButton->setShortcut(QKeySequence(Qt::ALT | Qt::Key_Q, QKeyCombination::fromCombined(0), QKeyCombination::fromCombined(0), QKeyCombination::fromCombined(0)));
        MainGUI::connect(quitButton, &QPushButton::clicked, [=]()
                         { QApplication::quit(); });

        SolvedGUI::drawFrame(gridWidget);

        // center window
        this->move(screen()->geometry().center() - frameGeometry().center());

        MainGUI::setTabOrder(loadButton, saveButton);
        MainGUI::setTabOrder(saveButton, fields->at(0));
#pragma unroll static_cast < short>(global::order * global::order)
        for (uint8_t fID_ = 1; fID_ < global::order * global::order; fID_++)
        {
            MainGUI::setTabOrder(fields->at(fID_ - 1), fields->at(fID_));
        }
        MainGUI::setTabOrder(fields->at((global::order * global::order) - 1), candidatesButton);
        MainGUI::setTabOrder(candidatesButton, stepByStepButton);
        MainGUI::setTabOrder(stepByStepButton, solveButton);
        MainGUI::setTabOrder(solveButton, clearButton);
        MainGUI::setTabOrder(clearButton, quitButton);
        MainGUI::setTabOrder(quitButton, loadButton);
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
        std::unique_ptr<TechniquesDialog> techniquesDialog = std::make_unique<TechniquesDialog>(this->nakedSinglesEnabled, this->hiddenSinglesEnabled, this->nakedPairsEnabled, this->hiddenPairsEnabled, this->nakedTriplesEnabled, this->hiddenTriplesEnabled, this->blockLineChecksEnabled, this->lineBlockChecksEnabled, this);
        techniquesDialog->exec();
        this->logTextBrowser->clear();
        this->logTextBrowser->append(QStringLiteral("Selected solving techniques:"));
        this->logTextBrowser->append(QStringLiteral("Naked Singles: ") + (this->nakedSinglesEnabled ? QStringLiteral("          true") : QStringLiteral("          false")));
        this->logTextBrowser->append(QStringLiteral("Hidden Singles: ") + (this->hiddenSinglesEnabled ? QStringLiteral("         true") : QStringLiteral("         false")));
        this->logTextBrowser->append(QStringLiteral("Naked Pairs: ") + (this->nakedPairsEnabled ? QStringLiteral("            true") : QStringLiteral("            false")));
        this->logTextBrowser->append(QStringLiteral("Hidden Pairs: ") + (this->hiddenPairsEnabled ? QStringLiteral("           true") : QStringLiteral("           false")));
        this->logTextBrowser->append(QStringLiteral("Naked Triples: ") + (this->nakedTriplesEnabled ? QStringLiteral("          true") : QStringLiteral("          false")));
        this->logTextBrowser->append(QStringLiteral("Hidden Triples: ") + (this->hiddenTriplesEnabled ? QStringLiteral("         true") : QStringLiteral("         false")));
        this->logTextBrowser->append(QStringLiteral("Block-Line-Interactions: ") + (this->blockLineChecksEnabled ? QStringLiteral("true") : QStringLiteral("false")));
        this->logTextBrowser->append(QStringLiteral("Line-Block-Interactions: ") + (this->lineBlockChecksEnabled ? QStringLiteral("true") : QStringLiteral("false")));
    }

    auto MainGUI::loadButtonClicked() -> std::string
    {
        const QString msgBoxStyleSheet = QStringLiteral("color: black; background: rgb(239, 239, 239)");
        // Read the data directory from the Qudoku.ini file created during installation
        const QString filepath = QFileDialog::getOpenFileName(this, QStringLiteral("Open Sudoku from file"), (QSettings(QStringLiteral("./Qudoku.ini"), QSettings::IniFormat, nullptr)).value("DIRS/DataDir").toString(), QString(), nullptr, QFileDialog::Options());
        if (filepath.isEmpty())
        {
            // User cancelled file selection
            this->logTextBrowser->append(QStringLiteral("File opening cancelled!"));
            return {};
        }

        QFile file(filepath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            this->logTextBrowser->append(QStringLiteral("Failed to open file:"));
            return {};
        }

        QTextStream loadFile(&file);
        const QString sudokuString = loadFile.readAll().trimmed();

        if (sudokuString.isEmpty())
        {
            QMessageBox msgBox(QMessageBox::Critical, QStringLiteral("Empty file"), QStringLiteral("Error reading \"") + filepath + QStringLiteral("\":\nEmpty file."), QMessageBox::Ok, this->gridWidget, Qt::WindowFlags());
            msgBox.setStyleSheet(msgBoxStyleSheet);
            msgBox.exec();
#pragma unroll static_cast < short>(global::order * global::order)
            for (auto* inputField : *this->fields)
            {
                inputField->clear();
            }
            return {};
        }

        if (sudokuString.length() != static_cast<uint8_t>(global::order * global::order))
        {
            QMessageBox msgBox(QMessageBox::Critical, QStringLiteral("Empty file"), QStringLiteral("Error reading \"") + filepath + QStringLiteral("\":\nEmpty file."), QMessageBox::Ok, this->gridWidget, Qt::WindowFlags());
            msgBox.setStyleSheet(msgBoxStyleSheet);
            msgBox.exec();
#pragma unroll static_cast < short>(global::order * global::order)
            for (auto* inputField : *this->fields)
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
                    QMessageBox msgBox(QMessageBox::Critical, QStringLiteral("Empty file"), QStringLiteral("Error reading \"") + filepath + QStringLiteral("\":\nEmpty file."), QMessageBox::Ok, this->gridWidget, Qt::WindowFlags());
                    msgBox.setStyleSheet(msgBoxStyleSheet);
                    msgBox.exec();
#pragma unroll static_cast < short>(global::order * global::order)
                    for (auto* inputField : *this->fields)
                    {
                        inputField->clear();
                    }
                    return {};
                }
                this->fields->at(fID - 1)->setText(inputChar);
            }
            else if (inputChar == '.')
            {
                this->fields->at(fID - 1)->clear();
            }
            else
            {
                QMessageBox msgBox(QMessageBox::Critical, QStringLiteral("Empty file"), QStringLiteral("Error reading \"") + filepath + QStringLiteral("\":\nEmpty file."), QMessageBox::Ok, this->gridWidget, Qt::WindowFlags());
                msgBox.setStyleSheet(msgBoxStyleSheet);
                msgBox.exec();
#pragma unroll static_cast < short>(global::order * global::order)
                for (auto* inputField : *this->fields)
                {
                    inputField->clear();
                }
                return {};
            }
        }

        const QString extraLine = loadFile.readLine(0);

        if (!extraLine.isEmpty())
        {
            QMessageBox msgBox(QMessageBox::Warning, QStringLiteral("Empty file"), QStringLiteral("Error reading \"") + filepath + QStringLiteral("\":\nEmpty file."), QMessageBox::Ok, this->gridWidget, Qt::WindowFlags());
            msgBox.setStyleSheet(msgBoxStyleSheet);
            msgBox.exec();
        }

        // extract the raw file name
        // size_t const lastSeparator = filepath.toStdString().find_last_of("/\\");
        // size_t const lastDot = filepath.toStdString().find_last_of('.');
        // filename = filepath.toStdString().substr(lastSeparator + 1, lastDot - lastSeparator - 1);

        this->logTextBrowser->clear();
        this->logTextBrowser->append("Sudoku \"" + filepath + "\" successfully loaded");
        file.close();

        return filename;
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
            this->logTextBrowser->append(QStringLiteral("Failed to open file for writing:"));
            return;
        }

        QTextStream out(&file);

#pragma unroll static_cast < short>(global::order * global::order)
        for (uint8_t fID = 1; fID <= global::order * global::order; fID++)
        {
            QString const val = fields->at(fID - 1)->text();
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

        this->logTextBrowser->append("Saved to \"" + filepath + "\"");
    }

    void MainGUI::candidatesButtonClicked()
    {
        std::array<uint8_t, static_cast<uint8_t>(global::order * global::order)> initVals{};
        auto sudoku = init(&initVals);
        auto* candidatesGUI = std::make_unique<CandidatesGUI>(&sudoku, initVals, this).release();
        const QPoint candidatesGUIpos(this->pos().x(), this->pos().y() + 50);
        candidatesGUI->move(candidatesGUIpos);
        candidatesGUI->show();
    }

    void MainGUI::stepByStepButtonButtonClicked()
    {
        this->logTextBrowser->clear();
        this->logTextBrowser->append(QStringLiteral("Solving in progress. Please wait..."));
        this->logTextBrowser->repaint();
        std::array<uint8_t, static_cast<uint8_t>(global::order * global::order)> initVals{};
        auto sudoku = init(&initVals);
        sudoku.solve(filename);
        auto* stepByStepGUI = std::make_unique<StepByStepGUI>(&sudoku, initVals, this).release();
        stepByStepGUI->move(QPoint(this->pos().x(), this->pos().y()));
        stepByStepGUI->show();
    }

    void MainGUI::solveButtonClicked()
    {
        this->logTextBrowser->clear();
        this->logTextBrowser->append(QStringLiteral("Solving in progress. Please wait..."));
        this->logTextBrowser->repaint();
        std::array<uint8_t, static_cast<uint8_t>(global::order * global::order)> initVals{};
        auto sudoku = init(&initVals);
        sudoku.solve(filename);
        auto* solvedGUI = std::make_unique<SolvedGUI>(&sudoku, initVals, this).release();
        const QPoint solvedGUIpos(this->pos().x(), this->pos().y() + 50);
        solvedGUI->move(solvedGUIpos);
        solvedGUI->show();
    }

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
        for (auto* inputField : *this->fields)
        {
            inputField->clear();
        }
        this->logTextBrowser->clear();
        filename = "";
    }

    auto MainGUI::init(std::array<uint8_t, static_cast<uint8_t>(global::order* global::order)>* initVals) const -> Sudoku
    {
#pragma unroll static_cast < short>(global::order * global::order)
        for (uint8_t i = 1; i <= global::order * global::order; i++)
        {

            try
            {
                initVals->at(i - 1) = static_cast<uint8_t>(std::stoi(this->fields->at(i - 1)->text().toStdString(), nullptr, global::base));
            }
            catch (const std::exception& e)
            {
                initVals->at(i - 1) = 0;
            }
        }
        return {initVals, *this->logTextBrowser, this->nakedSinglesEnabled, this->hiddenSinglesEnabled, this->nakedPairsEnabled, this->hiddenPairsEnabled, this->nakedTriplesEnabled, this->hiddenTriplesEnabled, this->blockLineChecksEnabled, this->lineBlockChecksEnabled};
    }
} // namespace sudoku
