#pragma once

#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QPushButton>

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
            TechniquesDialog(bool& nakedSinglesEnabled, bool& hiddenSinglesEnabled, bool& nakedPairsEnabled, bool& hiddenPairsEnabled, bool& nakedTriplesEnabled, bool& hiddenTriplesEnabled, bool& blockLineChecksEnabled, bool& lineBlockChecksEnabled);
            ~TechniquesDialog() override = default;

        private:
            QCheckBox* nakedSinglesCheckBox;
            QCheckBox* hiddenSinglesCheckBox;
            QFrame* hLine0;
            QCheckBox* nakedPairsCheckBox;
            QCheckBox* hiddenPairsCheckBox;
            QFrame* hLine1;
            QCheckBox* nakedTriplesCheckBox;
            QCheckBox* hiddenTriplesCheckBox;
            QFrame* hLine2;
            QCheckBox* blockLineChecksCheckBox;
            QCheckBox* lineBlockChecksCheckBox;
            QFrame* hLine3;
            QPushButton* acceptButton;
            QPushButton* cancelButton;

            void acceptChanges(bool& nakedSinglesEnabled, bool& hiddenSinglesEnabled, bool& nakedPairsEnabled, bool& hiddenPairsEnabled, bool& nakedTriplesEnabled, bool& hiddenTriplesEnabled, bool& blockLineChecksEnabled, bool& lineBlockChecksEnabled);
    };
} // namespace sudoku
