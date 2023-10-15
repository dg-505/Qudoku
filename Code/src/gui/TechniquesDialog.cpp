#include "gui/TechniquesDialog.h"

namespace sudoku
{
    TechniquesDialog::TechniquesDialog(bool& nakedSinglesEnabled, bool& hiddenSinglesEnabled, bool& nakedPairsEnabled, bool& hiddenPairsEnabled, bool& nakedTriplesEnabled, bool& hiddenTriplesEnabled, bool& blockLineChecksEnabled, bool& lineBlockChecksEnabled, bool& backtrackingEnabled, QWidget* parent)
        : QDialog(parent, Qt::WindowFlags()),
          nakedSinglesCheckBox(new QCheckBox(QStringLiteral("Use Naked Singles"), this)),
          hiddenSinglesCheckBox(new QCheckBox(QStringLiteral("Use Hidden Singles"), this)),
          hLine0(new QFrame(this, Qt::WindowFlags())),
          nakedPairsCheckBox(new QCheckBox(QStringLiteral("Use Naked Pairs"), this)),
          hiddenPairsCheckBox(new QCheckBox(QStringLiteral("Use Hidden Pairs"), this)),
          hLine1(new QFrame(this, Qt::WindowFlags())),
          nakedTriplesCheckBox(new QCheckBox(QStringLiteral("Use Naked Triples"), this)),
          hiddenTriplesCheckBox(new QCheckBox(QStringLiteral("Use Hidden Triples"), this)),
          hLine2(new QFrame(this, Qt::WindowFlags())),
          blockLineChecksCheckBox(new QCheckBox(QStringLiteral("Use Block-Line-Interactions"), this)),
          lineBlockChecksCheckBox(new QCheckBox(QStringLiteral("Use Line-Block-Interactions"), this)),
          hLine3(new QFrame(this, Qt::WindowFlags())),
          backtrackingCheckBox(new QCheckBox(QStringLiteral("Use Try && Error (Backtracking)"), this)),
          hLine4(new QFrame(this, Qt::WindowFlags())),
          acceptButton(new QPushButton(QStringLiteral("Accept"), this)),
          cancelButton(new QPushButton(QStringLiteral("Cancel"), this))
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

        this->nakedSinglesCheckBox->setObjectName("nakedSinglesCheckBox");
        constexpr QRect nakedSinglesCheckBoxGeom(10, 10, 300, 20);
        this->nakedSinglesCheckBox->setGeometry(nakedSinglesCheckBoxGeom);
        this->nakedSinglesCheckBox->setFont(checkBoxFont);
        this->nakedSinglesCheckBox->setStyleSheet(checkBoxStyleSheet);
        this->nakedSinglesCheckBox->setChecked(nakedSinglesEnabled);

        this->hiddenSinglesCheckBox->setObjectName("hiddenSinglesCheckBox");
        constexpr QRect hiddenSinglesCheckBoxGeom(10, 40, 300, 20);
        this->hiddenSinglesCheckBox->setGeometry(hiddenSinglesCheckBoxGeom);
        this->hiddenSinglesCheckBox->setFont(checkBoxFont);
        this->hiddenSinglesCheckBox->setStyleSheet(checkBoxStyleSheet);
        this->hiddenSinglesCheckBox->setChecked(hiddenSinglesEnabled);

        this->hLine0->setObjectName("hLine0");
        constexpr QRect hLine0Geom(10, 70, 300, 1);
        this->hLine0->setGeometry(hLine0Geom);
        this->hLine0->setLineWidth(1);
        this->hLine0->setFrameShape(QFrame::HLine);
        this->hLine0->setStyleSheet(QStringLiteral("color: rgb(171, 171, 171)"));

        this->nakedPairsCheckBox->setObjectName("nakedPairsCheckBox");
        constexpr QRect nakedPairsCheckBoxGeom(10, 81, 300, 20);
        this->nakedPairsCheckBox->setGeometry(nakedPairsCheckBoxGeom);
        this->nakedPairsCheckBox->setFont(checkBoxFont);
        this->nakedPairsCheckBox->setStyleSheet(checkBoxStyleSheet);
        this->nakedPairsCheckBox->setChecked(nakedPairsEnabled);

        this->hiddenPairsCheckBox->setObjectName("hiddenPairsCheckBox");
        constexpr QRect hiddenPairsCheckBoxGeom(10, 111, 300, 20);
        this->hiddenPairsCheckBox->setGeometry(hiddenPairsCheckBoxGeom);
        this->hiddenPairsCheckBox->setFont(checkBoxFont);
        this->hiddenPairsCheckBox->setStyleSheet(checkBoxStyleSheet);
        this->hiddenPairsCheckBox->setChecked(hiddenPairsEnabled);

        this->hLine1->setObjectName("hLine1");
        constexpr QRect hLine1Geom(10, 141, 300, 1);
        this->hLine1->setGeometry(hLine1Geom);
        this->hLine1->setLineWidth(1);
        this->hLine1->setFrameShape(QFrame::HLine);
        this->hLine1->setStyleSheet(QStringLiteral("color: rgb(171, 171, 171)"));

        this->nakedTriplesCheckBox->setObjectName("nakedTriplesCheckBox");
        constexpr QRect nakedTriplesCheckBoxGeom(10, 152, 300, 20);
        this->nakedTriplesCheckBox->setGeometry(nakedTriplesCheckBoxGeom);
        this->nakedTriplesCheckBox->setFont(checkBoxFont);
        this->nakedTriplesCheckBox->setStyleSheet(checkBoxStyleSheet);
        this->nakedTriplesCheckBox->setChecked(nakedTriplesEnabled);

        this->hiddenTriplesCheckBox->setObjectName("hiddenTriplesCheckBox");
        constexpr QRect hiddenTriplesCheckBoxGeom(10, 182, 300, 20);
        this->hiddenTriplesCheckBox->setGeometry(hiddenTriplesCheckBoxGeom);
        this->hiddenTriplesCheckBox->setFont(checkBoxFont);
        this->hiddenTriplesCheckBox->setStyleSheet(checkBoxStyleSheet);
        this->hiddenTriplesCheckBox->setChecked(hiddenTriplesEnabled);

        this->hLine2->setObjectName("hLine2");
        constexpr QRect hLine2Geom(10, 212, 300, 1);
        this->hLine2->setGeometry(hLine2Geom);
        this->hLine2->setLineWidth(1);
        this->hLine2->setFrameShape(QFrame::HLine);
        this->hLine2->setStyleSheet(QStringLiteral("color: rgb(171, 171, 171)"));

        this->blockLineChecksCheckBox->setObjectName("blockLineChecksCheckBox");
        constexpr QRect blockLineChecksCheckBoxGeom(10, 223, 300, 20);
        this->blockLineChecksCheckBox->setGeometry(blockLineChecksCheckBoxGeom);
        this->blockLineChecksCheckBox->setFont(checkBoxFont);
        this->blockLineChecksCheckBox->setStyleSheet(checkBoxStyleSheet);
        this->blockLineChecksCheckBox->setChecked(blockLineChecksEnabled);

        this->lineBlockChecksCheckBox->setObjectName("lineBlockChecksCheckBox");
        constexpr QRect lineBlockChecksCheckBoxGeom(10, 253, 300, 20);
        this->lineBlockChecksCheckBox->setGeometry(lineBlockChecksCheckBoxGeom);
        this->lineBlockChecksCheckBox->setFont(checkBoxFont);
        this->lineBlockChecksCheckBox->setStyleSheet(checkBoxStyleSheet);
        this->lineBlockChecksCheckBox->setChecked(lineBlockChecksEnabled);

        this->hLine3->setObjectName("hLine3");
        constexpr QRect hLine3Geom(10, 283, 300, 1);
        this->hLine3->setGeometry(hLine3Geom);
        this->hLine3->setLineWidth(1);
        this->hLine3->setFrameShape(QFrame::HLine);
        this->hLine3->setStyleSheet(QStringLiteral("color: rgb(171, 171, 171)"));

        this->backtrackingCheckBox->setObjectName("lineBlockChecksCheckBox");
        constexpr QRect backtrackingCheckBoxGeom(10, 294, 300, 20);
        this->backtrackingCheckBox->setGeometry(backtrackingCheckBoxGeom);
        this->backtrackingCheckBox->setFont(checkBoxFont);
        this->backtrackingCheckBox->setStyleSheet(checkBoxStyleSheet);
        this->backtrackingCheckBox->setChecked(backtrackingEnabled);

        this->hLine4->setObjectName("hLine4");
        constexpr QRect hLine4Geom(10, 324, 300, 1);
        this->hLine4->setGeometry(hLine4Geom);
        this->hLine4->setLineWidth(1);
        this->hLine4->setFrameShape(QFrame::HLine);
        this->hLine4->setStyleSheet(QStringLiteral("color: rgb(171, 171, 171)"));

        const QFont buttonFont(QStringLiteral("Open Sans"), 11, QFont::Bold, false);

        this->acceptButton->setObjectName("acceptButton");
        constexpr QRect acceptButtonGeom(10, 335, 144, 30);
        this->acceptButton->setGeometry(acceptButtonGeom);
        this->acceptButton->setFont(buttonFont);
        this->acceptButton->setStyleSheet(buttonStyleSheet);
        QObject::connect(
            this->acceptButton, &QPushButton::clicked, this, [this, &nakedSinglesEnabled, &hiddenSinglesEnabled, &nakedPairsEnabled, &hiddenPairsEnabled, &nakedTriplesEnabled, &hiddenTriplesEnabled, &blockLineChecksEnabled, &lineBlockChecksEnabled, &backtrackingEnabled]()
            { this->acceptChanges(nakedSinglesEnabled, hiddenSinglesEnabled, nakedPairsEnabled, hiddenPairsEnabled, nakedTriplesEnabled, hiddenTriplesEnabled, blockLineChecksEnabled, lineBlockChecksEnabled, backtrackingEnabled); },
            Qt::AutoConnection);

        this->cancelButton->setObjectName("cancelButton");
        constexpr QRect cancelButtonGeom(166, 335, 144, 30);
        this->cancelButton->setGeometry(cancelButtonGeom);
        this->cancelButton->setFont(buttonFont);
        this->cancelButton->setStyleSheet(buttonStyleSheet);
        QObject::connect(this->cancelButton, &QPushButton::clicked, this, &TechniquesDialog::reject, Qt::AutoConnection);
    }

    void TechniquesDialog::acceptChanges(bool& nakedSinglesEnabled, bool& hiddenSinglesEnabled, bool& nakedPairsEnabled, bool& hiddenPairsEnabled, bool& nakedTriplesEnabled, bool& hiddenTriplesEnabled, bool& blockLineChecksEnabled, bool& lineBlockChecksEnabled, bool& backtrackingEnabled)
    {
        nakedSinglesEnabled = this->nakedSinglesCheckBox->isChecked();
        hiddenSinglesEnabled = this->hiddenSinglesCheckBox->isChecked();
        nakedPairsEnabled = this->nakedPairsCheckBox->isChecked();
        hiddenPairsEnabled = this->hiddenPairsCheckBox->isChecked();
        nakedTriplesEnabled = this->nakedTriplesCheckBox->isChecked();
        hiddenTriplesEnabled = this->hiddenTriplesCheckBox->isChecked();
        blockLineChecksEnabled = this->blockLineChecksCheckBox->isChecked();
        lineBlockChecksEnabled = this->lineBlockChecksCheckBox->isChecked();
        backtrackingEnabled = this->backtrackingCheckBox->isChecked();
        this->accept();
    }
} // namespace sudoku
