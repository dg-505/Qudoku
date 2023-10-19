#pragma once

#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>

#include "globals.h"
#include "gui/CandidatesGUI.h"
#include "gui/QInputField.h"
#include "gui/QLogTextBrowser.h"
#include "gui/SolvedGUI.h"
#include "gui/StepByStepGUI.h"
#include "sudoku/Sudoku.h"

namespace sudoku
{
    class MainGUI : public QMainWindow
    {
            Q_OBJECT
        public:
            MainGUI(const MainGUI&) = delete;
            MainGUI(MainGUI&&) = delete;
            auto operator=(const MainGUI&) -> MainGUI& = delete;
            auto operator=(MainGUI&&) -> MainGUI& = delete;
            MainGUI(const std::string& version, QWidget* parent);
            ~MainGUI() override = default;

        private:
            QScrollArea* logScrollArea;
            QLogTextBrowser* logTextBrowser;
            QLabel* titleLabel;
            QWidget* gridWidget;
            std::array<QInputField*, static_cast<uint8_t>(global::order* global::order)>* fields{};
            QPushButton* techniquesButton;
            QPushButton* loadButton;
            // QPushButton* validateButton;
            QPushButton* saveButton;
            QPushButton* candidatesButton;
            QPushButton* stepByStepButton;
            QPushButton* solveButton;
            QPushButton* clearButton;
            QPushButton* quitButton;
            std::string filename;

            bool nakedSinglesEnabled = true;
            bool hiddenSinglesEnabled = true;
            bool nakedPairsEnabled = true;
            bool hiddenPairsEnabled = true;
            bool nakedTriplesEnabled = true;
            bool hiddenTriplesEnabled = true;
            bool blockLineChecksEnabled = true;
            bool lineBlockChecksEnabled = true;
            bool backtrackingEnabled = true;

            [[nodiscard]] auto init(std::array<uint8_t, static_cast<uint8_t>(global::order* global::order)>* initVals) const -> Sudoku;
            void clear();

            void techniquesButtonClicked();
            auto loadButtonClicked() -> std::string;
            void saveButtonClicked() const;
            void candidatesButtonClicked();
            void stepByStepButtonClicked();
            void solveButtonClicked();
            void clearButtonClicked();

            void closeEvent(QCloseEvent* /*event*/) override;

        protected:
            void keyPressEvent(QKeyEvent* event) override;
    };
} // namespace sudoku
