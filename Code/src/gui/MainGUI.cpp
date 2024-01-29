#include <QApplication>
#include <QFileDialog>
#include <QIcon>
#include <QMessageBox>
#include <QScreen>
#include <QShortcut>
#include <QTextStream>

#include "globals.h"
#include "gui/CandidatesGUI.h"
#include "gui/MainGUI.h"
#include "gui/SolvedGUI.h"
#include "gui/StepByStepGUI.h"
#include "gui/TechniquesDialog.h"

namespace sudoku
{
    // Main GUI setup
    MainGUI::MainGUI(std::string version, QWidget* parent)
        : QMainWindow(parent, Qt::WindowFlags()),
          _logScrollArea(new QScrollArea(this)),
          _logTextBrowser(new QLogTextBrowser(_logScrollArea)),
          _titleLabel(new QLabel(this, Qt::WindowFlags())),
          _languageLabel(new QLabel(this, Qt::WindowFlags())),
          _languageComboBox(new QComboBox(this)),
          _gridWidget(new QWidget(this, Qt::WindowFlags())),
          _fields(new std::array<QInputField*, static_cast<uint8_t>(global::order* global::order)>),
          _techniquesButton(new QPushButton(this)),
          _loadButton(new QPushButton(this)),
          _saveButton(new QPushButton(this)),
          _candidatesButton(new QPushButton(this)),
          _stepByStepButton(new QPushButton(this)),
          _solveButton(new QPushButton(this)),
          _clearButton(new QPushButton(this)),
          _quitButton(new QPushButton(this)),
          _version(std::move(version)),
          _translator(parent)
    {

        // Read settings from ini file
#ifdef _WIN32
        // On Windows, ini location is next to the executable
        _settings = std::make_unique<QSettings>(QStringLiteral("./Qudoku.ini"), QSettings::IniFormat, nullptr).release();
#else
        // On Linux, ini location is /home/<USER>/.qudoku/Qudoku.ini
        _settings = std::make_unique<QSettings>(QDir::homePath() + "/.qudoku/Qudoku.ini", QSettings::IniFormat, nullptr).release();
#endif

        // Main window properties
        constexpr QSize guiDim(1025, 637);
        this->setFixedSize(guiDim);
        this->setObjectName(QStringLiteral("MainGUI"));
        const QString i18nTitle = QCoreApplication::translate("MainGUI", "Qudoku - Qt based cross platform sudoku solver (Version ");
        const std::string versionTitle = _version + ")";
        this->setWindowTitle(i18nTitle + QString::fromStdString(versionTitle));
        this->setWindowIcon(QIcon(QStringLiteral(":/res/Qudoku.ico")));
        this->setStyleSheet(QStringLiteral("background: rgb(239, 239, 239)"));

        const QString buttonStyleSheet = QStringLiteral("QPushButton {color: black; background: rgb(239, 239, 239)}"
                                                        "QPushButton:hover {color: black; background: rgb(171, 171, 171)}"
                                                        "QPushButton:pressed {color: black; background: rgb(171, 171, 171)}");

        // Log scroll area and text browser
        const QFont logFont(QStringLiteral("Liberation Mono"), 10, QFont::Bold, false);

        _logScrollArea->setObjectName(QStringLiteral("logScrollArea"));
        constexpr QRect logScrollAreaGeom(537, 0, 488, 637);
        _logScrollArea->setGeometry(logScrollAreaGeom);

        _logTextBrowser->setObjectName(QStringLiteral("logTextBrowser"));
        constexpr QRect logTextBrowserGeom(0, 0, 488, 637);
        _logTextBrowser->setGeometry(logTextBrowserGeom);
        _logTextBrowser->setStyleSheet(QStringLiteral("color: black; background: white"));
        _logTextBrowser->setWordWrapMode(QTextOption::NoWrap);
        _logTextBrowser->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        _logTextBrowser->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        _logTextBrowser->setFont(logFont);

        // Title label
        _titleLabel->setObjectName(QStringLiteral("titleLabel"));
        constexpr QRect titleLabelGeom(0, 25, 300, 25);
        _titleLabel->setGeometry(titleLabelGeom);
        _titleLabel->setStyleSheet(QStringLiteral("color: black; background: rgb(239, 239, 239)"));
        const QFont titleFont(QStringLiteral("Open Sans"), 10, QFont::Bold, false);
        _titleLabel->setFont(titleFont);
        _titleLabel->setAlignment(Qt::AlignCenter);
        _titleLabel->setText(QCoreApplication::translate("MainGUI", "Enter predefined fields"));

        const QFont buttonFont(QStringLiteral("Open Sans"), 10, QFont::Normal, false);

        _languageLabel->setObjectName(QStringLiteral("languageLabel"));
        constexpr QRect languageLabelGeom(50, 0, 125, 25);
        _languageLabel->setGeometry(languageLabelGeom);
        _languageLabel->setStyleSheet(QStringLiteral("color: black; background: rgb(239, 239, 239)"));
        _languageLabel->setFont(buttonFont);
        _languageLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        _languageLabel->setText(QCoreApplication::translate("MainGUI", "Language: "));

        // Read language from settings
        _language = _settings->value(QStringLiteral("I18N/lang"), QVariant()).toString();

        // Specify directory containing the translation "*.qm" files
#ifdef _WIN32
        // On Windows, the directory containing the translation files is next to the executable
        _i18nDir = QStringLiteral("./i18n");
#else
        // On Linux, the directory containing the translation files is /opt/qudoku/i18n
        _i18nDir = QStringLiteral("/opt/qudoku/i18n");
#endif

        _languageComboBox->setObjectName(QStringLiteral("languageComboBox"));
        constexpr QRect languageComboBoxGeom(175, 0, 125, 25);
        _languageComboBox->setGeometry(languageComboBoxGeom);
        _languageComboBox->setStyleSheet(QStringLiteral("color: black; background: rgb(239, 239, 239)"));
        _languageComboBox->setFont(buttonFont);
        _languageComboBox->addItem(QIcon(QStringLiteral(":/res/i18n/CZ.png")), QStringLiteral("cs_CZ"), QVariant());
        _languageComboBox->addItem(QIcon(QStringLiteral(":/res/i18n/DK.png")), QStringLiteral("da_DK"), QVariant());
        _languageComboBox->addItem(QIcon(QStringLiteral(":/res/i18n/DE.png")), QStringLiteral("de_DE"), QVariant());
        _languageComboBox->addItem(QIcon(QStringLiteral(":/res/i18n/US.png")), QStringLiteral("en_US"), QVariant());
        _languageComboBox->addItem(QIcon(QStringLiteral(":/res/i18n/ES.png")), QStringLiteral("es_ES"), QVariant());
        _languageComboBox->addItem(QIcon(QStringLiteral(":/res/i18n/FI.png")), QStringLiteral("fi_FI"), QVariant());
        _languageComboBox->addItem(QIcon(QStringLiteral(":/res/i18n/FR.png")), QStringLiteral("fr_FR"), QVariant());
        _languageComboBox->addItem(QIcon(QStringLiteral(":/res/i18n/IT.png")), QStringLiteral("it_IT"), QVariant());
        _languageComboBox->addItem(QIcon(QStringLiteral(":/res/i18n/JP.png")), QStringLiteral("ja_JP"), QVariant());
        _languageComboBox->addItem(QIcon(QStringLiteral(":/res/i18n/KR.png")), QStringLiteral("ko_KR"), QVariant());
        _languageComboBox->addItem(QIcon(QStringLiteral(":/res/i18n/NO.png")), QStringLiteral("nb_NO"), QVariant());
        _languageComboBox->addItem(QIcon(QStringLiteral(":/res/i18n/NL.png")), QStringLiteral("nl_NL"), QVariant());
        _languageComboBox->addItem(QIcon(QStringLiteral(":/res/i18n/PL.png")), QStringLiteral("pl_PL"), QVariant());
        _languageComboBox->addItem(QIcon(QStringLiteral(":/res/i18n/BR.png")), QStringLiteral("pt_BR"), QVariant());
        _languageComboBox->addItem(QIcon(QStringLiteral(":/res/i18n/PT.png")), QStringLiteral("pt_PT"), QVariant());
        _languageComboBox->addItem(QIcon(QStringLiteral(":/res/i18n/RU.png")), QStringLiteral("ru_RU"), QVariant());
        _languageComboBox->addItem(QIcon(QStringLiteral(":/res/i18n/SE.png")), QStringLiteral("sv_SE"), QVariant());
        _languageComboBox->addItem(QIcon(QStringLiteral(":/res/i18n/TR.png")), QStringLiteral("tr_TR"), QVariant());
        _languageComboBox->addItem(QIcon(QStringLiteral(":/res/i18n/UA.png")), QStringLiteral("uk_UA"), QVariant());
        _languageComboBox->addItem(QIcon(QStringLiteral(":/res/i18n/CN.png")), QStringLiteral("zh_CN"), QVariant());
        constexpr QSize iconSize(28, 28);
        _languageComboBox->setIconSize(iconSize);
        _languageComboBox->setCurrentText(_language);
        MainGUI::connect(_languageComboBox, static_cast<void (QComboBox::*)(const int)>(&QComboBox::currentIndexChanged), this,
                         [this](const int langIdx)
                         {
                             _translator.load(_languageComboBox->itemText(langIdx), _i18nDir, QString(), QStringLiteral(".qm"));
                             qApp->installTranslator(&_translator);
                         }, Qt::AutoConnection);

        // Load the translator
        _translator.load(_language, _i18nDir, QString(), QStringLiteral(".qm"));
        qApp->installTranslator(&_translator);

        // Set up the fields
        _gridWidget->setObjectName(QStringLiteral("gridWidget"));
        constexpr QRect gridWidgetGeom(0, 50, 537, 537);
        _gridWidget->setGeometry(gridWidgetGeom);

        const QFont fieldsFont(QStringLiteral("Liberation Mono"), 28, QFont::Bold, false);
        const QFont rcLabelFont(QStringLiteral("Open Sans"), 14, QFont::Bold, false);

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
                _fields->at(fID - 1) = field;
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
        _techniquesButton->setObjectName(QStringLiteral("techniquesButton"));
        constexpr QRect tehniquesButtonGeom(300, 0, 100, 50);
        _techniquesButton->setGeometry(tehniquesButtonGeom);
        _techniquesButton->setFont(buttonFont);
        _techniquesButton->setStyleSheet(buttonStyleSheet);
        _techniquesButton->setText(QCoreApplication::translate("MainGUI", "Select\n&techniques"));
        MainGUI::connect(_techniquesButton, &QPushButton::clicked, this, &MainGUI::techniquesButtonClicked, Qt::AutoConnection);

        _loadButton->setObjectName(QStringLiteral("loadButton"));
        constexpr QRect loadButtonGeom(400, 0, 137, 25);
        _loadButton->setGeometry(loadButtonGeom);
        _loadButton->setFont(buttonFont);
        _loadButton->setStyleSheet(buttonStyleSheet);
        _loadButton->setText(QCoreApplication::translate("MainGUI", "Lo&ad"));
        MainGUI::connect(_loadButton, &QPushButton::clicked, this, &MainGUI::loadButtonClicked, Qt::AutoConnection);

        // validateButton = new QPushButton(this);
        // validateButton->setObjectName("validateButton");
        // validateButton->setGeometry(412, 17, 100, 16);
        // validateButton->setFont(buttonFont);
        // validateButton->setText("Validate");

        _saveButton->setObjectName(QStringLiteral("saveButton"));
        constexpr QRect saveButtonGeom(400, 25, 137, 25);
        _saveButton->setGeometry(saveButtonGeom);
        _saveButton->setFont(buttonFont);
        _saveButton->setStyleSheet(buttonStyleSheet);
        _saveButton->setText(QCoreApplication::translate("MainGUI", "&Save"));
        MainGUI::connect(_saveButton, &QPushButton::clicked, this, &MainGUI::saveButtonClicked, Qt::AutoConnection);

        _candidatesButton->setObjectName(QStringLiteral("candidatesButton"));
        constexpr QRect candidatesButtonGeom(0, 587, 107, 50);
        _candidatesButton->setGeometry(candidatesButtonGeom);
        _candidatesButton->setFont(buttonFont);
        _candidatesButton->setStyleSheet(buttonStyleSheet);
        _candidatesButton->setText(QCoreApplication::translate("MainGUI", "&Candidates"));
        MainGUI::connect(_candidatesButton, &QPushButton::clicked, this, &MainGUI::candidatesButtonClicked, Qt::AutoConnection);

        _stepByStepButton->setObjectName(QStringLiteral("stepByStepButton"));
        constexpr QRect stepByStepButtonGeom(107, 587, 108, 50);
        _stepByStepButton->setGeometry(stepByStepButtonGeom);
        _stepByStepButton->setFont(buttonFont);
        _stepByStepButton->setStyleSheet(buttonStyleSheet);
        _stepByStepButton->setText(QCoreApplication::translate("MainGUI", "St&ep by Step"));
        MainGUI::connect(_stepByStepButton, &QPushButton::clicked, this, &MainGUI::stepByStepButtonClicked, Qt::AutoConnection);

        _solveButton->setObjectName(QStringLiteral("solveButton"));
        constexpr QRect solveButtonGeom(215, 587, 107, 50);
        _solveButton->setGeometry(solveButtonGeom);
        _solveButton->setFont(buttonFont);
        _solveButton->setStyleSheet(buttonStyleSheet);
        _solveButton->setText(QCoreApplication::translate("MainGUI", "Sol&ve"));
        MainGUI::connect(_solveButton, &QPushButton::clicked, this, &MainGUI::solveButtonClicked, Qt::AutoConnection);

        _clearButton->setObjectName(QStringLiteral("clearButton"));
        constexpr QRect clearButtonGeom(322, 587, 108, 50);
        _clearButton->setGeometry(clearButtonGeom);
        _clearButton->setFont(buttonFont);
        _clearButton->setStyleSheet(buttonStyleSheet);
        _clearButton->setText(QCoreApplication::translate("MainGUI", "Clea&r"));
        MainGUI::connect(_clearButton, &QPushButton::clicked, this, &MainGUI::clearButtonClicked, Qt::AutoConnection);

        _quitButton->setObjectName(QStringLiteral("quitButton"));
        constexpr QRect quitButtonGeom(430, 587, 107, 50);
        _quitButton->setGeometry(quitButtonGeom);
        _quitButton->setFont(buttonFont);
        _quitButton->setStyleSheet(buttonStyleSheet);
        _quitButton->setText(QCoreApplication::translate("MainGUI", "&Quit"));
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

    void MainGUI::changeEvent(QEvent* event)
    {
        if (event->type() == QEvent::LanguageChange)
        {
            const QString i18nTitle = QCoreApplication::translate("MainGUI", "Qudoku - Qt based cross platform sudoku solver (Version ");
            const std::string versionTitle = _version + ")";
            this->setWindowTitle(i18nTitle + QString::fromStdString(versionTitle));
            _titleLabel->setText(QCoreApplication::translate("MainGUI", "Enter predefined fields"));
            _languageLabel->setText(QCoreApplication::translate("MainGUI", "Language: "));
            _techniquesButton->setText(QCoreApplication::translate("MainGUI", "Select\n&techniques"));
            _loadButton->setText(QCoreApplication::translate("MainGUI", "Lo&ad"));
            _saveButton->setText(QCoreApplication::translate("MainGUI", "&Save"));

            // Write changed language to ini file
            _settings->setValue(QStringLiteral("I18N/lang"), _languageComboBox->currentText());
        }
    }

    // Button functions

    void MainGUI::techniquesButtonClicked()
    {
        std::unique_ptr<TechniquesDialog> techniquesDialog = std::make_unique<TechniquesDialog>(_nakedSinglesEnabled, _hiddenSinglesEnabled, _nakedPairsEnabled, _hiddenPairsEnabled, _nakedTriplesEnabled, _hiddenTriplesEnabled, _blockLineChecksEnabled, _lineBlockChecksEnabled, _backtrackingEnabled, this);
        if (techniquesDialog->exec() == QDialog::Accepted)
        {
            _logTextBrowser->clear();
            _logTextBrowser->append(QStringLiteral("Selected solving techniques:"));
            _logTextBrowser->append(QStringLiteral("Naked Singles: ") + (_nakedSinglesEnabled ? QStringLiteral("          true") : QStringLiteral("          false")));
            _logTextBrowser->append(QStringLiteral("Hidden Singles: ") + (_hiddenSinglesEnabled ? QStringLiteral("         true") : QStringLiteral("         false")));
            _logTextBrowser->append(QStringLiteral("Naked Pairs: ") + (_nakedPairsEnabled ? QStringLiteral("            true") : QStringLiteral("            false")));
            _logTextBrowser->append(QStringLiteral("Hidden Pairs: ") + (_hiddenPairsEnabled ? QStringLiteral("           true") : QStringLiteral("           false")));
            _logTextBrowser->append(QStringLiteral("Naked Triples: ") + (_nakedTriplesEnabled ? QStringLiteral("          true") : QStringLiteral("          false")));
            _logTextBrowser->append(QStringLiteral("Hidden Triples: ") + (_hiddenTriplesEnabled ? QStringLiteral("         true") : QStringLiteral("         false")));
            _logTextBrowser->append(QStringLiteral("Block-Line-Interactions: ") + (_blockLineChecksEnabled ? QStringLiteral("true") : QStringLiteral("false")));
            _logTextBrowser->append(QStringLiteral("Line-Block-Interactions: ") + (_lineBlockChecksEnabled ? QStringLiteral("true") : QStringLiteral("false")));
            _logTextBrowser->append(QStringLiteral("Try & Error: ") + (_backtrackingEnabled ? QStringLiteral("            true") : QStringLiteral("            false")));
        }
    }

    auto MainGUI::loadButtonClicked() -> std::string
    {
        const QString msgBoxStyleSheet = QStringLiteral("color: black; background: rgb(239, 239, 239)");
        // Read the data directory from the Qudoku.ini file created during installation
        const QString dataDir = _settings->value(QStringLiteral("DIRS/DataDir"), QVariant()).toString() + QStringLiteral("/data");
        _logTextBrowser->append("Data directory: " + dataDir);
        const QString filepath = QFileDialog::getOpenFileName(this, QStringLiteral("Open Sudoku from file"), dataDir, QString(), nullptr, QFileDialog::Options());
        if (filepath.isEmpty())
        {
            // User cancelled file selection
            _logTextBrowser->append(QStringLiteral("File opening cancelled!"));
            return {};
        }

        QFile file(filepath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            _logTextBrowser->append(QStringLiteral("Failed to open file:"));
            return {};
        }

        QTextStream loadFile(&file);
        const QString sudokuString = loadFile.readAll().trimmed();

        if (sudokuString.isEmpty())
        {
            QMessageBox msgBox(QMessageBox::Critical, QStringLiteral("Empty file"), QStringLiteral("Error reading \"") + filepath + QStringLiteral("\":\nEmpty file."), QMessageBox::Ok, _gridWidget, Qt::WindowFlags());
            msgBox.setStyleSheet(msgBoxStyleSheet);
            msgBox.exec();
#pragma unroll static_cast < short>(global::order * global::order)
            for (auto* inputField : *_fields)
            {
                inputField->clear();
            }
            return {};
        }

        if (sudokuString.length() != static_cast<uint8_t>(global::order * global::order))
        {
            QMessageBox msgBox(QMessageBox::Critical, QStringLiteral("Empty file"), QStringLiteral("Error reading \"") + filepath + QStringLiteral("\":\nEmpty file."), QMessageBox::Ok, _gridWidget, Qt::WindowFlags());
            msgBox.setStyleSheet(msgBoxStyleSheet);
            msgBox.exec();
#pragma unroll static_cast < short>(global::order * global::order)
            for (auto* inputField : *_fields)
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
                    QMessageBox msgBox(QMessageBox::Critical, QStringLiteral("Empty file"), QStringLiteral("Error reading \"") + filepath + QStringLiteral("\":\nEmpty file."), QMessageBox::Ok, _gridWidget, Qt::WindowFlags());
                    msgBox.setStyleSheet(msgBoxStyleSheet);
                    msgBox.exec();
#pragma unroll static_cast < short>(global::order * global::order)
                    for (auto* inputField : *_fields)
                    {
                        inputField->clear();
                    }
                    return {};
                }
                _fields->at(fID - 1)->setText(inputChar);
            }
            else if (inputChar == '.')
            {
                _fields->at(fID - 1)->clear();
            }
            else
            {
                QMessageBox msgBox(QMessageBox::Critical, QStringLiteral("Empty file"), QStringLiteral("Error reading \"") + filepath + QStringLiteral("\":\nEmpty file."), QMessageBox::Ok, _gridWidget, Qt::WindowFlags());
                msgBox.setStyleSheet(msgBoxStyleSheet);
                msgBox.exec();
#pragma unroll static_cast < short>(global::order * global::order)
                for (auto* inputField : *_fields)
                {
                    inputField->clear();
                }
                return {};
            }
        }

        const QString extraLine = loadFile.readLine(0);

        if (!extraLine.isEmpty())
        {
            QMessageBox msgBox(QMessageBox::Warning, QStringLiteral("Empty file"), QStringLiteral("Error reading \"") + filepath + QStringLiteral("\":\nEmpty file."), QMessageBox::Ok, _gridWidget, Qt::WindowFlags());
            msgBox.setStyleSheet(msgBoxStyleSheet);
            msgBox.exec();
        }

        // extract the raw file name
        size_t const lastSeparator = filepath.toStdString().find_last_of("/\\", -1);
        size_t const lastDot = filepath.toStdString().find_last_of('.', -1);
        _name = filepath.toStdString().substr(lastSeparator + 1, lastDot - lastSeparator - 1);

        _logTextBrowser->clear();
        _logTextBrowser->append("Sudoku \"" + filepath + "\" successfully loaded");
        file.close();

        return _name;
    }

    void MainGUI::saveButtonClicked() const
    {
        const QString dataDir = _settings->value(QStringLiteral("DIRS/DataDir"), QVariant()).toString() + QStringLiteral("/data");
        const QString filepath = QFileDialog::getSaveFileName(this->centralWidget(), QStringLiteral("Save Sudoku to file"), dataDir, QString(), nullptr, QFileDialog::Options());

        if (filepath.isEmpty())
        {
            // User cancelled file selection
            return;
        }

        QFile file(filepath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            _logTextBrowser->append(QStringLiteral("Failed to open file for writing:"));
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

        _logTextBrowser->append("Saved to \"" + filepath + "\"");
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
        _logTextBrowser->clear();
        _logTextBrowser->append(QStringLiteral("Solving in progress. Please wait..."));
        _logTextBrowser->repaint();

        auto loadingLabel = std::make_unique<QLabel>(_gridWidget, Qt::WindowFlags());
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
        sudoku->solve();
        auto* stepByStepGUI = std::make_unique<StepByStepGUI>(sudoku, initVals, this).release();
        stepByStepGUI->move(QPoint(this->pos().x(), this->pos().y()));
        stepByStepGUI->show();
    }

    void MainGUI::solveButtonClicked()
    {
        _logTextBrowser->clear();
        _logTextBrowser->append(QStringLiteral("Solving in progress. Please wait..."));
        _logTextBrowser->repaint();

        auto loadingLabel = std::make_unique<QLabel>(_gridWidget, Qt::WindowFlags());
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
        sudoku->solve();
        auto* solvedGUI = std::make_unique<SolvedGUI>(sudoku, initVals, _name, _settings, this).release();
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
        for (auto* inputField : *_fields)
        {
            inputField->clear();
        }
        _logTextBrowser->clear();
        _name = "";
    }

    auto MainGUI::init(std::array<uint8_t, static_cast<uint8_t>(global::order* global::order)>* initVals) const -> Sudoku*
    {
#pragma unroll static_cast < short>(global::order * global::order)
        for (uint8_t i = 1; i <= global::order * global::order; i++)
        {
            try
            {
                initVals->at(i - 1) = static_cast<uint8_t>(std::stoi(_fields->at(i - 1)->text().toStdString(), nullptr, global::base));
            }
            catch (const std::exception& e)
            {
                initVals->at(i - 1) = 0;
            }
        }
        return std::make_unique<Sudoku>(_name, initVals, _logTextBrowser, _nakedSinglesEnabled, _hiddenSinglesEnabled, _nakedPairsEnabled, _hiddenPairsEnabled, _nakedTriplesEnabled, _hiddenTriplesEnabled, _blockLineChecksEnabled, _lineBlockChecksEnabled, _backtrackingEnabled).release();
    }
} // namespace sudoku
