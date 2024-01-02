#pragma once

#include <QCheckBox>
#include <QDialog>
#include <QFrame>
#include <QPushButton>

namespace sudoku
{
    class TechniquesDialog : public QDialog
    {
            Q_OBJECT
        public:
            TechniquesDialog(const TechniquesDialog&) = delete;
            TechniquesDialog(TechniquesDialog&&) = delete;
            auto operator=(const TechniquesDialog&) -> TechniquesDialog& = delete;
            auto operator=(TechniquesDialog&&) -> TechniquesDialog& = delete;
            TechniquesDialog(bool& nakedSinglesEnabled, bool& hiddenSinglesEnabled, bool& nakedPairsEnabled, bool& hiddenPairsEnabled, bool& nakedTriplesEnabled, bool& hiddenTriplesEnabled, bool& blockLineChecksEnabled, bool& lineBlockChecksEnabled, bool& backtrackingEnabled, QWidget* parent);
            ~TechniquesDialog() override = default;

        private:
            QCheckBox* _nakedSinglesCheckBox;
            QCheckBox* _hiddenSinglesCheckBox;
            QFrame* _hLine0;
            QCheckBox* _nakedPairsCheckBox;
            QCheckBox* _hiddenPairsCheckBox;
            QFrame* _hLine1;
            QCheckBox* _nakedTriplesCheckBox;
            QCheckBox* _hiddenTriplesCheckBox;
            QFrame* _hLine2;
            QCheckBox* _blockLineChecksCheckBox;
            QCheckBox* _lineBlockChecksCheckBox;
            QFrame* _hLine3;
            QCheckBox* _backtrackingCheckBox;
            QFrame* _hLine4;
            QPushButton* _acceptButton;
            QPushButton* _cancelButton;

            void acceptChanges(bool& nakedSinglesEnabled, bool& hiddenSinglesEnabled, bool& nakedPairsEnabled, bool& hiddenPairsEnabled, bool& nakedTriplesEnabled, bool& hiddenTriplesEnabled, bool& blockLineChecksEnabled, bool& lineBlockChecksEnabled, bool& backtrackingEnabled);
    };
} // namespace sudoku
