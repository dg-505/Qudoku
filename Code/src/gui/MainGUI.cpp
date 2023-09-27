#include <QtCore/QSettings>
#include <QtCore/QTextStream>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
//#include <QtWidgets/QDesktopWidget>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
//#include <iomanip>
//#include <iostream>
//#include <sstream>

#include "globals.h"
#include "gui/CandidatesGUI.h"
#include "gui/MainGUI.h"
#include "gui/SolvedGUI.h"
#include "gui/StepByStepGUI.h"

namespace sudoku
{

    // Main GUI setup
    MainGUI::MainGUI(QWidget* parent)
        : QMainWindow(parent, Qt::WindowFlags()),
          logScrollArea(new QScrollArea(this)),
          logTextBrowser(new QLogTextBrowser(logScrollArea)),
          titleLabel(new QLabel(this, Qt::WindowFlags())),
          fields(new array<QInputField*, static_cast<uint8_t>(order* order)>),
          //          field(new QInputField(this)),
          loadButton(new QPushButton(this)),
          saveButton(new QPushButton(this)),
          candidatesButton(new QPushButton(this)),
          stepByStepButton(new QPushButton(this)),
          solveButton(new QPushButton(this)),
          clearButton(new QPushButton(this)),
          exitButton(new QPushButton(this))
    {
        // Main window properties
        setFixedSize(912, 612);
        setObjectName("MainGUI");
        setWindowTitle(QStringLiteral("Qudoku - Qt based cross platform sudoku solver"));
        QMainWindow::setWindowIcon(QIcon(QStringLiteral(":/res/Qudoku.ico")));

        QString const defaultStyleSheet = QStringLiteral("color: black; background: rgb(239, 239, 239)");

        // Log scroll area and text browser
        QFont const logFont(QStringLiteral("Liberation Mono"), 10, QFont::Bold, false);

        logScrollArea->setObjectName("logScrollArea");
        logScrollArea->setGeometry(512, 0, 400, 612);

        logTextBrowser->setObjectName("logTextBrowser");
        logTextBrowser->setGeometry(0, 0, 400, 612);
        // logTextBrowser->setStyleSheet(defaultStyleSheet);
        logTextBrowser->setStyleSheet(QStringLiteral("color: black; background: white"));
        logTextBrowser->setWordWrapMode(QTextOption::NoWrap);
        logTextBrowser->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        logTextBrowser->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        logTextBrowser->setFont(logFont);

        // Title label
        titleLabel->setObjectName("titleLabel");
        titleLabel->setGeometry(0, 0, 412, 50);
        titleLabel->setStyleSheet(defaultStyleSheet);
        titleLabel->setFont(QFont(QStringLiteral("Open Sans"), 14, QFont::Bold, false));
        titleLabel->setAlignment(Qt::AlignCenter);
        titleLabel->setText(QStringLiteral("Please fill in the initially given fields"));

        // Set up the fields
        QFont const fieldsFont(QStringLiteral("Liberation Mono"), 32, QFont::Bold, false);

        // QSudokuInputField* fields[81];
        uint8_t fID = 1;
        uint16_t posY = 50;
        for (uint8_t rID = 1; rID <= order; rID++)
        {
            uint16_t posX = 0;
#pragma unroll
            for (uint8_t cID = 1; cID <= order; cID++)
            {
                //                std::stringstream objectName;
                //                objectName << "field" << std::setw(2) << std::setfill('0') << fID;
                auto* field = new QInputField(this);
                field->setObjectName("field" + QString::number(fID));
                field->setGeometry(posX, posY, 56, 56);
                // field->setStyleSheet("color: black; background: rgb(239, 239, 239); QLinefocus: {background: black}");
                field->setStyleSheet(QStringLiteral("QLineEdit {color: black; background: white} QLineEdit:focus{color: black; background: white; border: 1px solid black}"));
                field->setFont(fieldsFont);
                field->setMaxLength(1);
                field->setAlignment(Qt::AlignCenter);
                fields->at(fID - 1) = field;
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

        // Buttons
        QFont const buttonFont(QStringLiteral("Open Sans"), 11, QFont::Bold, false);

        loadButton->setObjectName("loadButton");
        // loadButton->setGeometry(412, 0, 100, 17); // with validateButton
        loadButton->setGeometry(412, 0, 100, 25); // without validateButton
        loadButton->setFont(buttonFont);
        loadButton->setStyleSheet(defaultStyleSheet);
        loadButton->setText(QStringLiteral("Load"));
        connect(loadButton, &QPushButton::clicked, this, &MainGUI::loadButtonClicked, Qt::AutoConnection);
        // QObject::connect(loadButton, &QPushButton::clicked, [=]()
        //                  { logTextBrowser->append("loadButtonButtonClicked!"); });

        // validateButton = new QPushButton(this);
        // validateButton->setObjectName("validateButton");
        // validateButton->setGeometry(412, 17, 100, 16);
        // validateButton->setFont(buttonFont);
        // validateButton->setText("Validate");

        saveButton->setObjectName("saveButton");
        // saveButton->setGeometry(412, 33, 100, 17); // with validateButton
        saveButton->setGeometry(412, 25, 100, 25); // without validateButton
        saveButton->setFont(buttonFont);
        saveButton->setStyleSheet(defaultStyleSheet);
        saveButton->setText(QStringLiteral("Save"));
        connect(saveButton, &QPushButton::clicked, this, &MainGUI::saveButtonClicked, Qt::AutoConnection);
        // QObject::connect(saveButton, &QPushButton::clicked, [=]()
        //                  { logTextBrowser->append("saveButtonButtonClicked!"); });

        candidatesButton->setObjectName("candidatesButton");
        candidatesButton->setGeometry(0, 562, 102, 50);
        candidatesButton->setFont(buttonFont);
        candidatesButton->setStyleSheet(defaultStyleSheet);
        candidatesButton->setText(QStringLiteral("Candidates"));
        connect(candidatesButton, &QPushButton::clicked, this, &MainGUI::candidatesButtonClicked, Qt::AutoConnection);
        // QObject::connect(candidatesButton, &QPushButton::clicked, [=]()
        //                  { logTextBrowser->append("candidatesButtonClicked!"); });

        stepByStepButton->setObjectName("stepByStepButton");
        stepByStepButton->setGeometry(102, 562, 103, 50);
        stepByStepButton->setFont(buttonFont);
        stepByStepButton->setStyleSheet(defaultStyleSheet);
        stepByStepButton->setText(QStringLiteral("Step by Step"));
        connect(stepByStepButton, &QPushButton::clicked, this, &MainGUI::stepByStepButtonButtonClicked, Qt::AutoConnection);
        // QObject::connect(stepByStepButton, &QPushButton::clicked, [=]()
        //                  { logTextBrowser->append("stepByStepButtonClicked!"); });

        solveButton->setObjectName("solveButton");
        solveButton->setGeometry(205, 562, 102, 50);
        solveButton->setFont(buttonFont);
        solveButton->setStyleSheet(defaultStyleSheet);
        solveButton->setText(QStringLiteral("Solve"));
        connect(solveButton, &QPushButton::clicked, this, &MainGUI::solveButtonClicked, Qt::AutoConnection);
        // QObject::connect(solveButton, &QPushButton::clicked, [=]()
        //                  { logTextBrowser->append("solveButtonClicked!"); });

        clearButton->setObjectName("clearButton");
        clearButton->setGeometry(307, 562, 103, 50);
        clearButton->setFont(buttonFont);
        clearButton->setStyleSheet(defaultStyleSheet);
        clearButton->setText(QStringLiteral("Clear"));
        connect(clearButton, &QPushButton::clicked, this, &MainGUI::clearButtonClicked, Qt::AutoConnection);
        // QObject::connect(clearButton, &QPushButton::clicked, [=]()
        //                  { logTextBrowser->append("clearButtonClicked!"); });

        exitButton->setObjectName("exitButton");
        exitButton->setGeometry(410, 562, 102, 50);
        exitButton->setFont(buttonFont);
        exitButton->setStyleSheet(defaultStyleSheet);
        exitButton->setText(QStringLiteral("Exit"));
        // connect(exitButton, &QPushButton:#:clicked, this, &MainGUI::exitButtonClicked);
        QObject::connect(exitButton, &QPushButton::clicked, [=]()
                         { QApplication::quit(); });

        // Frame for the grid
        hLine0 = new QFrame(this);
        hLine0->setObjectName("hLine0");
        hLine0->setGeometry(0, 50, 512, 2);
        hLine0->setLineWidth(2);
        hLine0->setFrameShape(QFrame::HLine);
        hLine0->setStyleSheet(QStringLiteral("color: black"));
        hLine1 = new QFrame(this);
        hLine1->setObjectName("hLine1");
        hLine1->setGeometry(0, 219, 512, 2);
        hLine1->setLineWidth(2);
        hLine1->setFrameShape(QFrame::HLine);
        hLine1->setStyleSheet(QStringLiteral("color: black"));
        hLine2 = new QFrame(this);
        hLine2->setObjectName("hLine2");
        hLine2->setGeometry(0, 391, 512, 2);
        hLine2->setLineWidth(2);
        hLine2->setFrameShape(QFrame::HLine);
        hLine2->setStyleSheet(QStringLiteral("color: black"));
        hLine3 = new QFrame(this);
        hLine3->setObjectName("hLine3");
        hLine3->setGeometry(0, 561, 512, 2);
        hLine3->setLineWidth(2);
        hLine3->setFrameShape(QFrame::HLine);
        hLine3->setStyleSheet(QStringLiteral("color: black"));
        vLine0 = new QFrame(this);
        vLine0->setObjectName("vLine0");
        vLine0->setGeometry(0, 50, 2, 512);
        vLine0->setLineWidth(2);
        vLine0->setFrameShape(QFrame::VLine);
        vLine0->setStyleSheet(QStringLiteral("color: black"));
        vLine1 = new QFrame(this);
        vLine1->setObjectName("vLine1");
        vLine1->setGeometry(169, 50, 2, 512);
        vLine1->setLineWidth(2);
        vLine1->setFrameShape(QFrame::VLine);
        vLine1->setStyleSheet(QStringLiteral("color: black"));
        vLine2 = new QFrame(this);
        vLine2->setObjectName("vLine2");
        vLine2->setGeometry(341, 50, 2, 512);
        vLine2->setLineWidth(2);
        vLine2->setFrameShape(QFrame::VLine);
        vLine2->setStyleSheet(QStringLiteral("color: black"));
        vLine3 = new QFrame(this);
        vLine3->setObjectName("vLine3");
        vLine3->setGeometry(510, 50, 2, 512);
        vLine3->setLineWidth(2);
        vLine3->setFrameShape(QFrame::VLine);
        vLine3->setStyleSheet(QStringLiteral("color: black"));

        // center window
        //        short screenWidth = QApplication::desktop()->width();
        //        short screenHeight = QApplication::desktop()->height();
        //        short windowWidth = this->width();
        //        short windowHeight = this->height();
        //        short x = (screenWidth - windowWidth) / 2;
        //        short y = (screenHeight - windowHeight) / 2;
        //        this->setGeometry(x, y, windowWidth, windowHeight);
        move(screen()->geometry().center() - frameGeometry().center());

        QWidget::setTabOrder(loadButton, saveButton);
        QWidget::setTabOrder(saveButton, fields->at(0));
#pragma unroll
        for (uint8_t fID = 1; fID < order * order; fID++)
        {
            QWidget::setTabOrder(fields->at(fID - 1), fields->at(fID));
        }
        QWidget::setTabOrder(fields->at((order * order) - 1), candidatesButton);
        QWidget::setTabOrder(candidatesButton, stepByStepButton);
        QWidget::setTabOrder(stepByStepButton, solveButton);
        QWidget::setTabOrder(solveButton, clearButton);
        QWidget::setTabOrder(clearButton, exitButton);
        QWidget::setTabOrder(exitButton, logScrollArea);
        QWidget::setTabOrder(logScrollArea, loadButton);
    }

    //    MainGUI::~MainGUI()
    //    {
    //    }

    // Button functions

    std::string MainGUI::loadButtonClicked()
    {
        // Read the data directory from the Qudoku.ini file created during installation
        QString const filepath = QFileDialog::getOpenFileName(this, QStringLiteral("Open Sudoku from file"), (QSettings("./Qudoku.ini", QSettings::IniFormat)).value("DIRS/DataDir").toString());
        if (filepath.isEmpty())
        {
            // User cancelled file selection
            logTextBrowser->append(QStringLiteral("File opening cancelled!"));
            return "";
        }

        QFile file(filepath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            logTextBrowser->append(QStringLiteral("Failed to open file:"));
            return "";
        }

        QTextStream loadFile(&file);
        QString const sudokuString = loadFile.readAll().trimmed();

        if (sudokuString.isEmpty())
        {
            QMessageBox::critical(nullptr, QStringLiteral("Empty file"), QStringLiteral("Error reading \"%1\":\nEmpty file.").arg(filepath));
            for (auto* field : *fields)
            {
                field->clear();
            }
            return "";
        }

        if (sudokuString.length() != order * order)
        {
            QMessageBox::critical(nullptr, QStringLiteral("Invalid number of entries"), QStringLiteral("Error reading \"%1\":\nInvalid number of entries (%2).\n\nNumber of entries must be 81.").arg(filepath).arg(sudokuString.length()));
            for (auto* field : *fields)
            {
                field->clear();
            }
            return "";
        }

        for (uint8_t fID = 1; fID <= order * order; fID++)
        {
            QChar const inputChar = sudokuString.at(fID - 1);
            if (inputChar.isDigit())
            {
                uint8_t const value = inputChar.digitValue();
                if (value == 0)
                {
                    QMessageBox::critical(nullptr, QStringLiteral("Invalid value 0"), QStringLiteral("Error reading \"%1\":\nInvalid value \"0\" at entry %2.\n\nValues must be between 1 and 9.").arg(filepath).arg(fID + 1));
                    for (auto* field : *fields)
                    {
                        field->clear();
                    }
                    return "";
                }
                fields->at(fID - 1)->setText(inputChar);
            }
            else if (inputChar == '.')
            {
                fields->at(fID - 1)->clear();
            }
            else
            {
                QMessageBox::critical(nullptr, QStringLiteral("Invalid character"), QStringLiteral("Error reading \"%1\":\nInvalid character \"%2\" at entry %3.\n\nValid characters are numbers from 1 to 9 or dot ('.').").arg(filepath).arg(inputChar).arg(fID + 1));
                for (auto* field : *fields)
                {
                    field->clear();
                }
                return "";
            }
        }

        QString const extraLine = loadFile.readLine();

        if (!extraLine.isEmpty())
        {
            QMessageBox::warning(nullptr, QStringLiteral("Ignoring extra lines"), QStringLiteral("Ignoring extra lines in file \"%1\"").arg(filepath));
        }

        // extract the raw file name
        // size_t const lastSeparator = filepath.toStdString().find_last_of("/\\");
        // size_t const lastDot = filepath.toStdString().find_last_of('.');
        // filename = filepath.toStdString().substr(lastSeparator + 1, lastDot - lastSeparator - 1);

        logTextBrowser->clear();
        logTextBrowser->append("Sudoku \"" + filepath + "\" successfully loaded");
        // QMessageBox::information(nullptr, "Sudoku loaded", QString("Sudoku \"%1\" successfully loaded.").arg(QString::fromStdString(filename)));
        file.close();

        return filename;
    }

    const void MainGUI::saveButtonClicked() const
    {
        // Read the data directory from the Qudoku.ini file created during installation
        QSettings my_settings("./Qudoku.ini", QSettings::IniFormat);
        QString dataDir = my_settings.value("DIRS/DataDir").toString();
        QString const filepath = QFileDialog::getSaveFileName(this->centralWidget(), dataDir, QStringLiteral("Save Sudoku to file"));

        if (filepath.isEmpty())
        {
            // User cancelled file selection
            return;
        }

        QFile file(filepath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            logTextBrowser->append(QStringLiteral("Failed to open file for writing:"));
            return;
        }

        QTextStream out(&file);

        for (uint8_t fID = 1; fID <= order * order; fID++)
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

        logTextBrowser->append("Saved to \"" + filepath + "\"");
    }

    const void MainGUI::candidatesButtonClicked() const
    {
        uint8_t initVals[81];
        auto sudoku = init(initVals);
        // auto freeFields = sudoku.getFreeFields();
        auto* candidatesGUI = new sudoku::CandidatesGUI(&sudoku, this->centralWidget());
        candidatesGUI->move(this->pos().x(), this->pos().y() + 50);
        candidatesGUI->show();
    }

    const void MainGUI::stepByStepButtonButtonClicked() const
    {
        uint8_t initVals[81];
        auto sudoku = init(initVals);
        sudoku.solve(filename);
        // auto steps = sudoku.getSteps();
        auto* stepByStepGUI = new sudoku::StepByStepGUI(&sudoku, initVals, this->centralWidget());
        stepByStepGUI->move(this->pos().x(), this->pos().y());
        stepByStepGUI->show();
    }

    const void MainGUI::solveButtonClicked() const
    {
        uint8_t initVals[81];
        auto sudoku = init(initVals);
        sudoku.solve(filename);
        auto* solvedGUI = new sudoku::SolvedGUI(&sudoku, initVals, this->centralWidget());
        solvedGUI->move(this->pos().x(), this->pos().y() + 50);
        solvedGUI->show();
    }

    void MainGUI::clearButtonClicked()
    {
        clear();
    }

    void MainGUI::closeEvent(QCloseEvent *bar)
    {
        QApplication::quit();
    }

    // Other functions

    void MainGUI::clear()
    {
        for (auto* field : *fields)
        {
            field->clear();
        }
        logTextBrowser->clear();
        filename = "";
    }

    Sudoku MainGUI::init(uint8_t* initVals) const
    {
        // logTextBrowser->append("Initializing:");
        // uint8_t initVals[81];

        for (uint8_t i = 1; i <= order * order; i++)
        {
            try
            {
                initVals[i - 1] = static_cast<uint8_t>(std::stoi(fields->at(i - 1)->text().toStdString()));
            }
            catch (const std::exception& e)
            {
                initVals[i - 1] = 0;
            }
        }
        return Sudoku(initVals, *logTextBrowser);
    }
} // namespace sudoku
