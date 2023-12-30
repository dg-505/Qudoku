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
        this->setObjectName(QStringLiteral("techniquesDialog"));
        this->setWindowIcon(QIcon(QStringLiteral(":/res/Qudoku.ico")));
        this->setStyleSheet(QStringLiteral("background: rgb(239, 239, 239)"));
        constexpr QSize dim(320, 375);
        this->setFixedSize(dim);

        const QFont checkBoxFont(QStringLiteral("Open Sans"), 11, QFont::Normal, false);
        const QString checkBoxStyleSheet = QStringLiteral("color: black; background: rgb(239, 239, 239)");
        const QString buttonStyleSheet = QStringLiteral("QPushButton {color: black; background: rgb(239, 239, 239)}"
                                                        "QPushButton:hover {color: black; background: rgb(171, 171, 171)}"
                                                        "QPushButton:pressed {color: black; background: rgb(171, 171, 171)}");

        _nakedSinglesCheckBox->setObjectName(QStringLiteral("_nakedSinglesCheckBox"));
        constexpr QRect nakedSinglesCheckBoxGeom(10, 10, 300, 20);
        _nakedSinglesCheckBox->setGeometry(nakedSinglesCheckBoxGeom);
        _nakedSinglesCheckBox->setFont(checkBoxFont);
        _nakedSinglesCheckBox->setStyleSheet(checkBoxStyleSheet);
        _nakedSinglesCheckBox->setChecked(nakedSinglesEnabled);

        _hiddenSinglesCheckBox->setObjectName(QStringLiteral("_hiddenSinglesCheckBox"));
        constexpr QRect hiddenSinglesCheckBoxGeom(10, 40, 300, 20);
        _hiddenSinglesCheckBox->setGeometry(hiddenSinglesCheckBoxGeom);
        _hiddenSinglesCheckBox->setFont(checkBoxFont);
        _hiddenSinglesCheckBox->setStyleSheet(checkBoxStyleSheet);
        _hiddenSinglesCheckBox->setChecked(hiddenSinglesEnabled);

        _hLine0->setObjectName(QStringLiteral("_hLine0"));
        constexpr QRect hLine0Geom(10, 70, 300, 1);
        _hLine0->setGeometry(hLine0Geom);
        _hLine0->setLineWidth(1);
        _hLine0->setFrameShape(QFrame::HLine);
        _hLine0->setStyleSheet(QStringLiteral("color: rgb(171, 171, 171)"));

        _nakedPairsCheckBox->setObjectName(QStringLiteral("_nakedPairsCheckBox"));
        constexpr QRect nakedPairsCheckBoxGeom(10, 81, 300, 20);
        _nakedPairsCheckBox->setGeometry(nakedPairsCheckBoxGeom);
        _nakedPairsCheckBox->setFont(checkBoxFont);
        _nakedPairsCheckBox->setStyleSheet(checkBoxStyleSheet);
        _nakedPairsCheckBox->setChecked(nakedPairsEnabled);

        _hiddenPairsCheckBox->setObjectName(QStringLiteral("_hiddenPairsCheckBox"));
        constexpr QRect hiddenPairsCheckBoxGeom(10, 111, 300, 20);
        _hiddenPairsCheckBox->setGeometry(hiddenPairsCheckBoxGeom);
        _hiddenPairsCheckBox->setFont(checkBoxFont);
        _hiddenPairsCheckBox->setStyleSheet(checkBoxStyleSheet);
        _hiddenPairsCheckBox->setChecked(hiddenPairsEnabled);

        _hLine1->setObjectName(QStringLiteral("_hLine1"));
        constexpr QRect hLine1Geom(10, 141, 300, 1);
        _hLine1->setGeometry(hLine1Geom);
        _hLine1->setLineWidth(1);
        _hLine1->setFrameShape(QFrame::HLine);
        _hLine1->setStyleSheet(QStringLiteral("color: rgb(171, 171, 171)"));

        _nakedTriplesCheckBox->setObjectName(QStringLiteral("_nakedTriplesCheckBox"));
        constexpr QRect nakedTriplesCheckBoxGeom(10, 152, 300, 20);
        _nakedTriplesCheckBox->setGeometry(nakedTriplesCheckBoxGeom);
        _nakedTriplesCheckBox->setFont(checkBoxFont);
        _nakedTriplesCheckBox->setStyleSheet(checkBoxStyleSheet);
        _nakedTriplesCheckBox->setChecked(nakedTriplesEnabled);

        _hiddenTriplesCheckBox->setObjectName(QStringLiteral("_hiddenTriplesCheckBox"));
        constexpr QRect hiddenTriplesCheckBoxGeom(10, 182, 300, 20);
        _hiddenTriplesCheckBox->setGeometry(hiddenTriplesCheckBoxGeom);
        _hiddenTriplesCheckBox->setFont(checkBoxFont);
        _hiddenTriplesCheckBox->setStyleSheet(checkBoxStyleSheet);
        _hiddenTriplesCheckBox->setChecked(hiddenTriplesEnabled);

        _hLine2->setObjectName(QStringLiteral("_hLine2"));
        constexpr QRect hLine2Geom(10, 212, 300, 1);
        _hLine2->setGeometry(hLine2Geom);
        _hLine2->setLineWidth(1);
        _hLine2->setFrameShape(QFrame::HLine);
        _hLine2->setStyleSheet(QStringLiteral("color: rgb(171, 171, 171)"));

        _blockLineChecksCheckBox->setObjectName(QStringLiteral("_blockLineChecksCheckBox"));
        constexpr QRect blockLineChecksCheckBoxGeom(10, 223, 300, 20);
        _blockLineChecksCheckBox->setGeometry(blockLineChecksCheckBoxGeom);
        _blockLineChecksCheckBox->setFont(checkBoxFont);
        _blockLineChecksCheckBox->setStyleSheet(checkBoxStyleSheet);
        _blockLineChecksCheckBox->setChecked(blockLineChecksEnabled);

        _lineBlockChecksCheckBox->setObjectName(QStringLiteral("_lineBlockChecksCheckBox"));
        constexpr QRect lineBlockChecksCheckBoxGeom(10, 253, 300, 20);
        _lineBlockChecksCheckBox->setGeometry(lineBlockChecksCheckBoxGeom);
        _lineBlockChecksCheckBox->setFont(checkBoxFont);
        _lineBlockChecksCheckBox->setStyleSheet(checkBoxStyleSheet);
        _lineBlockChecksCheckBox->setChecked(lineBlockChecksEnabled);

        _hLine3->setObjectName(QStringLiteral("_hLine3"));
        constexpr QRect hLine3Geom(10, 283, 300, 1);
        _hLine3->setGeometry(hLine3Geom);
        _hLine3->setLineWidth(1);
        _hLine3->setFrameShape(QFrame::HLine);
        _hLine3->setStyleSheet(QStringLiteral("color: rgb(171, 171, 171)"));

        _backtrackingCheckBox->setObjectName(QStringLiteral("_lineBlockChecksCheckBox"));
        constexpr QRect backtrackingCheckBoxGeom(10, 294, 300, 20);
        _backtrackingCheckBox->setGeometry(backtrackingCheckBoxGeom);
        _backtrackingCheckBox->setFont(checkBoxFont);
        _backtrackingCheckBox->setStyleSheet(checkBoxStyleSheet);
        _backtrackingCheckBox->setChecked(backtrackingEnabled);

        _hLine4->setObjectName(QStringLiteral("_hLine4"));
        constexpr QRect hLine4Geom(10, 324, 300, 1);
        _hLine4->setGeometry(hLine4Geom);
        _hLine4->setLineWidth(1);
        _hLine4->setFrameShape(QFrame::HLine);
        _hLine4->setStyleSheet(QStringLiteral("color: rgb(171, 171, 171)"));

        const QFont buttonFont(QStringLiteral("Open Sans"), 11, QFont::Bold, false);

        _acceptButton->setObjectName(QStringLiteral("_acceptButton"));
        constexpr QRect acceptButtonGeom(10, 335, 144, 30);
        _acceptButton->setGeometry(acceptButtonGeom);
        _acceptButton->setFont(buttonFont);
        _acceptButton->setStyleSheet(buttonStyleSheet);
        _acceptButton->setShortcut(QKeySequence(Qt::ALT + Qt::Key_A, 0, 0, 0));
        QObject::connect(
            _acceptButton, &QPushButton::clicked, this, [this, &nakedSinglesEnabled, &hiddenSinglesEnabled, &nakedPairsEnabled, &hiddenPairsEnabled, &nakedTriplesEnabled, &hiddenTriplesEnabled, &blockLineChecksEnabled, &lineBlockChecksEnabled, &backtrackingEnabled]()
            { this->acceptChanges(nakedSinglesEnabled, hiddenSinglesEnabled, nakedPairsEnabled, hiddenPairsEnabled, nakedTriplesEnabled, hiddenTriplesEnabled, blockLineChecksEnabled, lineBlockChecksEnabled, backtrackingEnabled); },
            Qt::AutoConnection);

        _cancelButton->setObjectName(QStringLiteral("_cancelButton"));
        constexpr QRect cancelButtonGeom(166, 335, 144, 30);
        _cancelButton->setGeometry(cancelButtonGeom);
        _cancelButton->setFont(buttonFont);
        _cancelButton->setShortcut(QKeySequence(Qt::ALT + Qt::Key_C, 0, 0, 0));
        _cancelButton->setStyleSheet(buttonStyleSheet);
        QObject::connect(_cancelButton, &QPushButton::clicked, this, &TechniquesDialog::reject, Qt::AutoConnection);
    }

    void TechniquesDialog::acceptChanges(bool& nakedSinglesEnabled, bool& hiddenSinglesEnabled, bool& nakedPairsEnabled, bool& hiddenPairsEnabled, bool& nakedTriplesEnabled, bool& hiddenTriplesEnabled, bool& blockLineChecksEnabled, bool& lineBlockChecksEnabled, bool& backtrackingEnabled)
    {
        nakedSinglesEnabled = _nakedSinglesCheckBox->isChecked();
        hiddenSinglesEnabled = _hiddenSinglesCheckBox->isChecked();
        nakedPairsEnabled = _nakedPairsCheckBox->isChecked();
        hiddenPairsEnabled = _hiddenPairsCheckBox->isChecked();
        nakedTriplesEnabled = _nakedTriplesCheckBox->isChecked();
        hiddenTriplesEnabled = _hiddenTriplesCheckBox->isChecked();
        blockLineChecksEnabled = _blockLineChecksCheckBox->isChecked();
        lineBlockChecksEnabled = _lineBlockChecksCheckBox->isChecked();
        backtrackingEnabled = _backtrackingCheckBox->isChecked();
        this->accept();
    }
} // namespace sudoku
