#include <QDir>
#include <QFileDialog>
#include <QMessageBox>

#include "gui/DataDirDialog.h"

namespace sudoku
{
    DataDirDialog::DataDirDialog(QWidget* parent)
        : QDialog(parent, Qt::WindowFlags()),
          _descLabel(new QLabel(QStringLiteral("Specify a directory to store data and exported files:"), this, Qt::WindowFlags())),
          _lineEdit(new QLineEdit(this)),
          _browseButton(new QPushButton(QStringLiteral("Browse"), this)),
          _defaultButton(new QPushButton(QStringLiteral("Use default"), this)),
          _cancelButton(new QPushButton(QStringLiteral("Cancel"), this)),
          _proceedButton(new QPushButton(QStringLiteral("Proceed"), this)),
          _defaultDir(QDir::homePath() + QStringLiteral("/.qudoku")),
          _dataDir(_defaultDir)
    {
        this->setWindowTitle(QStringLiteral("Specify data directory"));
        this->setObjectName(QStringLiteral("dataDirDialog"));
        this->setWindowIcon(QIcon(QStringLiteral(":/res/Qudoku.ico")));
        this->setStyleSheet(QStringLiteral("background: rgb(239, 239, 239)"));
        constexpr QSize dim(500, 115);
        this->setFixedSize(dim);

        _descLabel->setObjectName(QStringLiteral("descLabel"));
        constexpr QRect descLabelGeom(10, 10, 480, 25);
        _descLabel->setGeometry(descLabelGeom);

        _lineEdit->setObjectName(QStringLiteral("lineEdit"));
        constexpr QRect lineEditGeom(10, 45, 360, 25);
        _lineEdit->setGeometry(lineEditGeom);

        _browseButton->setObjectName(QStringLiteral("browseButton"));
        constexpr QRect browseButtonGeom(390, 45, 100, 25);
        _browseButton->setGeometry(browseButtonGeom);
        DataDirDialog::connect(_browseButton, &QPushButton::clicked, this, &DataDirDialog::browse, Qt::AutoConnection);

        _defaultButton->setObjectName(QStringLiteral("defaultButton"));
        constexpr QRect defaultButtonGeom(10, 80, 147, 25);
        _defaultButton->setGeometry(defaultButtonGeom);
        DataDirDialog::connect(_defaultButton, &QPushButton::clicked, this, &DataDirDialog::useDefault, Qt::AutoConnection);

        _cancelButton->setObjectName(QStringLiteral("cancelButton"));
        constexpr QRect _cancelButtonGeom(177, 80, 146, 25);
        _cancelButton->setGeometry(_cancelButtonGeom);
        DataDirDialog::connect(_cancelButton, &QPushButton::clicked, this, &DataDirDialog::reject, Qt::AutoConnection);

        _proceedButton->setObjectName(QStringLiteral("proceedButton"));
        constexpr QRect continueButtonGeom(343, 80, 147, 25);
        _proceedButton->setGeometry(continueButtonGeom);
        DataDirDialog::connect(_proceedButton, &QPushButton::clicked, this, &DataDirDialog::proceed, Qt::AutoConnection);
    }

    void DataDirDialog::browse()
    {
        _dataDir = QFileDialog::getExistingDirectory(this, QStringLiteral("Specify data directory"), _defaultDir, QFileDialog::ShowDirsOnly);
        _lineEdit->setText(_dataDir);
    }

    void DataDirDialog::useDefault()
    {
        _dataDir = _defaultDir;
        _lineEdit->setText(_dataDir);
    }

    void DataDirDialog::proceed()
    {
        if (_lineEdit->text().isEmpty())
        {
            QMessageBox msgBox(QMessageBox::Information, QStringLiteral("No directory specified"), QStringLiteral("No directory specified.\nUse default directory \n'") + _defaultDir + QStringLiteral("'?"), QMessageBox::No | QMessageBox::Yes, this, Qt::WindowFlags());
            auto res = msgBox.exec();
            if (res == QMessageBox::Yes)
            {
                Q_EMIT directorySelected(_defaultDir);
                this->accept();
            }
        }
        else if (!QDir(_lineEdit->text()).exists())
        {
            QMessageBox msgBox(QMessageBox::Information, QStringLiteral("Directory does not exist"), QStringLiteral("The specified directory '") + _lineEdit->text() + QStringLiteral("'\ndoes not exist. Create it now?"), QMessageBox::No | QMessageBox::Yes, this, Qt::WindowFlags());
            auto res = msgBox.exec();
            if (res == QMessageBox::Yes)
            {
                QDir(QString()).mkdir(_lineEdit->text());
                Q_EMIT directorySelected(_lineEdit->text());
                this->accept();
            }
        }
        else
        {
            Q_EMIT directorySelected(_lineEdit->text());
            this->accept();
        }
    }

    auto DataDirDialog::getDataDir() const -> QString
    {
        return _lineEdit->text();
    }
} // namespace sudoku