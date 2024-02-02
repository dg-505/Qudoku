#pragma once

#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QScrollBar>

#include "globals.h"
#include "sudoku/Sudoku.h"

namespace sudoku
{
    class StepByStepGUI : public QMainWindow
    {
            Q_OBJECT // NOLINT
        public:
            StepByStepGUI(const StepByStepGUI&) = delete;
            StepByStepGUI(StepByStepGUI&&) = delete;
            auto operator=(const StepByStepGUI&) -> StepByStepGUI& = delete;
            auto operator=(StepByStepGUI&&) -> StepByStepGUI& = delete;
            StepByStepGUI(Sudoku* sudoku, const std::string& name, const std::array<uint8_t, static_cast<uint8_t>(global::order* global::order)>& initVals, QWidget* parent);
            ~StepByStepGUI() override = default;

            void drawPrevStep();
            void drawNextStep();
            [[nodiscard]] auto getStepMsg(uint8_t step) const -> QString;

            // eventFilter to scroll through steps with mouse wheel
            auto eventFilter(QObject* watched, QEvent* event) -> bool override;

        private:
            Sudoku* _sudoku;
            std::array<uint8_t, static_cast<uint8_t>(global::order* global::order)> _initVals;
            uint8_t _currentStep;
            uint8_t _currentPage;
            bool _isPreview;
            QWidget* _stepsStack;
            QLabel* _previewLabel;
            QLabel* _msgLabel;
            QWidget* _fieldsWidget;
            QScrollBar* _stepsScrollBar;
            QPushButton* _firstButton;
            QPushButton* _prevButton;
            QPushButton* _nextButton;
            QPushButton* _lastButton;
            QPushButton* _exportButton;

        protected:
            void keyPressEvent(QKeyEvent* event) override;
    };
} // namespace sudoku
