#include "gui/TechniquesDialog.h"

namespace sudoku
{
    TechniquesDialog::TechniquesDialog(bool& nakedSinglesEnabled, bool& hiddenSinglesEnabled, bool& nakedPairsEnabled, bool& hiddenPairsEnabled, bool& nakedTriplesEnabled, bool& hiddenTriplesEnabled, bool& blockLineChecksEnabled, bool& lineBlockChecksEnabled, bool& backtrackingEnabled, QWidget* parent)
        : QDialog(parent, Qt::WindowFlags()),
          _nakedSinglesCheckBox(new QCheckBox(QStringLiteral("Use Naked Singles"), this)),
          _hiddenSinglesCheckBox(new QCheckBox(QStringLiteral("Use Hidden Singles"), this)),
          _hLine0(new QFrame(this, Qt::WindowFlags())),
          _nakedPairsCheckBox(new QCheckBox(QStringLiteral("Use Naked Pairs"), this)),
          _hiddenPairsCheckBox(new QCheckBox(QStringLiteral("Use Hidden Pairs"), this)),
          _hLine1(new QFrame(this, Qt::WindowFlags())),
          _nakedTriplesCheckBox(new QCheckBox(QStringLiteral("Use Naked Triples"), this)),
          _hiddenTriplesCheckBox(new QCheckBox(QStringLiteral("Use Hidden Triples"), this)),
          _hLine2(new QFrame(this, Qt::WindowFlags())),
          _blockLineChecksCheckBox(new QCheckBox(QStringLiteral("Use Block-Line-Interactions"), this)),
          _lineBlockChecksCheckBox(new QCheckBox(QStringLiteral("Use Line-Block-Interactions"), this)),
          _hLine3(new QFrame(this, Qt::WindowFlags())),
          _backtrackingCheckBox(new QCheckBox(QStringLiteral("Use Try && Error (Backtracking)"), this)),
          _hLine4(new QFrame(this, Qt::WindowFlags())),
          _acceptButton(new QPushButton(QStringLiteral("&Accept"), this)),
          _cancelButton(new QPushButton(QStringLiteral("&Cancel"), this))
    {
        this->setWindowTitle(QStringLiteral("Select solving techniques"));
        this->setObjectName("techniquesDialog");
        this->setWindowIcon(QIcon(QStringLiteral(":/res/Qudoku.ico")));
        this->setStyleSheet(QStringLiteral("background: rgb(239, 239, 239)"));
        constexpr QSize dim(320, 375);
        this->setFixedSize(dim);

        const QFont checkBoxFont(QStringLiteral("Open Sans"), 12, QFont::Normal, false);
        const QString checkBoxStyleSheet = QStringLiteral("color: black; background: rgb(239, 239, 239)");
        const QString buttonStyleSheet = QStringLiteral("QPushButton {color: black; background: rgb(239, 239, 239)}"
                                                        "QPushButton:hover {color: black; background: rgb(171, 171, 171)}"
                                                        "QPushButton:pressed {color: black; background: rgb(171, 171, 171)}");

        this->_nakedSinglesCheckBox->setObjectName("_nakedSinglesCheckBox");
        constexpr QRect nakedSinglesCheckBoxGeom(10, 10, 300, 20);
        this->_nakedSinglesCheckBox->setGeometry(nakedSinglesCheckBoxGeom);
        this->_nakedSinglesCheckBox->setFont(checkBoxFont);
        this->_nakedSinglesCheckBox->setStyleSheet(checkBoxStyleSheet);
        this->_nakedSinglesCheckBox->setChecked(nakedSinglesEnabled);

        this->_hiddenSinglesCheckBox->setObjectName("_hiddenSinglesCheckBox");
        constexpr QRect hiddenSinglesCheckBoxGeom(10, 40, 300, 20);
        this->_hiddenSinglesCheckBox->setGeometry(hiddenSinglesCheckBoxGeom);
        this->_hiddenSinglesCheckBox->setFont(checkBoxFont);
        this->_hiddenSinglesCheckBox->setStyleSheet(checkBoxStyleSheet);
        this->_hiddenSinglesCheckBox->setChecked(hiddenSinglesEnabled);

        this->_hLine0->setObjectName("_hLine0");
        constexpr QRect hLine0Geom(10, 70, 300, 1);
        this->_hLine0->setGeometry(hLine0Geom);
        this->_hLine0->setLineWidth(1);
        this->_hLine0->setFrameShape(QFrame::HLine);
        this->_hLine0->setStyleSheet(QStringLiteral("color: rgb(171, 171, 171)"));

        this->_nakedPairsCheckBox->setObjectName("_nakedPairsCheckBox");
        constexpr QRect nakedPairsCheckBoxGeom(10, 81, 300, 20);
        this->_nakedPairsCheckBox->setGeometry(nakedPairsCheckBoxGeom);
        this->_nakedPairsCheckBox->setFont(checkBoxFont);
        this->_nakedPairsCheckBox->setStyleSheet(checkBoxStyleSheet);
        this->_nakedPairsCheckBox->setChecked(nakedPairsEnabled);

        this->_hiddenPairsCheckBox->setObjectName("_hiddenPairsCheckBox");
        constexpr QRect hiddenPairsCheckBoxGeom(10, 111, 300, 20);
        this->_hiddenPairsCheckBox->setGeometry(hiddenPairsCheckBoxGeom);
        this->_hiddenPairsCheckBox->setFont(checkBoxFont);
        this->_hiddenPairsCheckBox->setStyleSheet(checkBoxStyleSheet);
        this->_hiddenPairsCheckBox->setChecked(hiddenPairsEnabled);

        this->_hLine1->setObjectName("_hLine1");
        constexpr QRect hLine1Geom(10, 141, 300, 1);
        this->_hLine1->setGeometry(hLine1Geom);
        this->_hLine1->setLineWidth(1);
        this->_hLine1->setFrameShape(QFrame::HLine);
        this->_hLine1->setStyleSheet(QStringLiteral("color: rgb(171, 171, 171)"));

        this->_nakedTriplesCheckBox->setObjectName("_nakedTriplesCheckBox");
        constexpr QRect nakedTriplesCheckBoxGeom(10, 152, 300, 20);
        this->_nakedTriplesCheckBox->setGeometry(nakedTriplesCheckBoxGeom);
        this->_nakedTriplesCheckBox->setFont(checkBoxFont);
        this->_nakedTriplesCheckBox->setStyleSheet(checkBoxStyleSheet);
        this->_nakedTriplesCheckBox->setChecked(nakedTriplesEnabled);

        this->_hiddenTriplesCheckBox->setObjectName("_hiddenTriplesCheckBox");
        constexpr QRect hiddenTriplesCheckBoxGeom(10, 182, 300, 20);
        this->_hiddenTriplesCheckBox->setGeometry(hiddenTriplesCheckBoxGeom);
        this->_hiddenTriplesCheckBox->setFont(checkBoxFont);
        this->_hiddenTriplesCheckBox->setStyleSheet(checkBoxStyleSheet);
        this->_hiddenTriplesCheckBox->setChecked(hiddenTriplesEnabled);

        this->_hLine2->setObjectName("_hLine2");
        constexpr QRect hLine2Geom(10, 212, 300, 1);
        this->_hLine2->setGeometry(hLine2Geom);
        this->_hLine2->setLineWidth(1);
        this->_hLine2->setFrameShape(QFrame::HLine);
        this->_hLine2->setStyleSheet(QStringLiteral("color: rgb(171, 171, 171)"));

        this->_blockLineChecksCheckBox->setObjectName("_blockLineChecksCheckBox");
        constexpr QRect blockLineChecksCheckBoxGeom(10, 223, 300, 20);
        this->_blockLineChecksCheckBox->setGeometry(blockLineChecksCheckBoxGeom);
        this->_blockLineChecksCheckBox->setFont(checkBoxFont);
        this->_blockLineChecksCheckBox->setStyleSheet(checkBoxStyleSheet);
        this->_blockLineChecksCheckBox->setChecked(blockLineChecksEnabled);

        this->_lineBlockChecksCheckBox->setObjectName("_lineBlockChecksCheckBox");
        constexpr QRect lineBlockChecksCheckBoxGeom(10, 253, 300, 20);
        this->_lineBlockChecksCheckBox->setGeometry(lineBlockChecksCheckBoxGeom);
        this->_lineBlockChecksCheckBox->setFont(checkBoxFont);
        this->_lineBlockChecksCheckBox->setStyleSheet(checkBoxStyleSheet);
        this->_lineBlockChecksCheckBox->setChecked(lineBlockChecksEnabled);

        this->_hLine3->setObjectName("_hLine3");
        constexpr QRect hLine3Geom(10, 283, 300, 1);
        this->_hLine3->setGeometry(hLine3Geom);
        this->_hLine3->setLineWidth(1);
        this->_hLine3->setFrameShape(QFrame::HLine);
        this->_hLine3->setStyleSheet(QStringLiteral("color: rgb(171, 171, 171)"));

        this->_backtrackingCheckBox->setObjectName("_lineBlockChecksCheckBox");
        constexpr QRect backtrackingCheckBoxGeom(10, 294, 300, 20);
        this->_backtrackingCheckBox->setGeometry(backtrackingCheckBoxGeom);
        this->_backtrackingCheckBox->setFont(checkBoxFont);
        this->_backtrackingCheckBox->setStyleSheet(checkBoxStyleSheet);
        this->_backtrackingCheckBox->setChecked(backtrackingEnabled);

        this->_hLine4->setObjectName("_hLine4");
        constexpr QRect hLine4Geom(10, 324, 300, 1);
        this->_hLine4->setGeometry(hLine4Geom);
        this->_hLine4->setLineWidth(1);
        this->_hLine4->setFrameShape(QFrame::HLine);
        this->_hLine4->setStyleSheet(QStringLiteral("color: rgb(171, 171, 171)"));

        const QFont buttonFont(QStringLiteral("Open Sans"), 11, QFont::Bold, false);

        this->_acceptButton->setObjectName("_acceptButton");
        constexpr QRect acceptButtonGeom(10, 335, 144, 30);
        this->_acceptButton->setGeometry(acceptButtonGeom);
        this->_acceptButton->setFont(buttonFont);
        this->_acceptButton->setStyleSheet(buttonStyleSheet);
        this->_acceptButton->setShortcut(QKeySequence(Qt::ALT | Qt::Key_A, QKeyCombination::fromCombined(0), QKeyCombination::fromCombined(0), QKeyCombination::fromCombined(0)));
        QObject::connect(
            this->_acceptButton, &QPushButton::clicked, this, [this, &nakedSinglesEnabled, &hiddenSinglesEnabled, &nakedPairsEnabled, &hiddenPairsEnabled, &nakedTriplesEnabled, &hiddenTriplesEnabled, &blockLineChecksEnabled, &lineBlockChecksEnabled, &backtrackingEnabled]()
            { this->acceptChanges(nakedSinglesEnabled, hiddenSinglesEnabled, nakedPairsEnabled, hiddenPairsEnabled, nakedTriplesEnabled, hiddenTriplesEnabled, blockLineChecksEnabled, lineBlockChecksEnabled, backtrackingEnabled); },
            Qt::AutoConnection);

        this->_cancelButton->setObjectName("_cancelButton");
        constexpr QRect cancelButtonGeom(166, 335, 144, 30);
        this->_cancelButton->setGeometry(cancelButtonGeom);
        this->_cancelButton->setFont(buttonFont);
        this->_cancelButton->setShortcut(QKeySequence(Qt::ALT | Qt::Key_C, QKeyCombination::fromCombined(0), QKeyCombination::fromCombined(0), QKeyCombination::fromCombined(0)));
        this->_cancelButton->setStyleSheet(buttonStyleSheet);
        QObject::connect(this->_cancelButton, &QPushButton::clicked, this, &TechniquesDialog::reject, Qt::AutoConnection);
    }

    void TechniquesDialog::acceptChanges(bool& nakedSinglesEnabled, bool& hiddenSinglesEnabled, bool& nakedPairsEnabled, bool& hiddenPairsEnabled, bool& nakedTriplesEnabled, bool& hiddenTriplesEnabled, bool& blockLineChecksEnabled, bool& lineBlockChecksEnabled, bool& backtrackingEnabled)
    {
        nakedSinglesEnabled = this->_nakedSinglesCheckBox->isChecked();
        hiddenSinglesEnabled = this->_hiddenSinglesCheckBox->isChecked();
        nakedPairsEnabled = this->_nakedPairsCheckBox->isChecked();
        hiddenPairsEnabled = this->_hiddenPairsCheckBox->isChecked();
        nakedTriplesEnabled = this->_nakedTriplesCheckBox->isChecked();
        hiddenTriplesEnabled = this->_hiddenTriplesCheckBox->isChecked();
        blockLineChecksEnabled = this->_blockLineChecksCheckBox->isChecked();
        lineBlockChecksEnabled = this->_lineBlockChecksCheckBox->isChecked();
        backtrackingEnabled = this->_backtrackingCheckBox->isChecked();
        this->accept();
    }
} // namespace sudoku
