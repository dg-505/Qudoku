#pragma once

#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QStackedWidget>

#include "globals.h"
#include "sudoku/Sudoku.h"

namespace sudoku
{
    class StepByStepGUI : public QMainWindow
    {
            Q_OBJECT
        public:
            StepByStepGUI(const StepByStepGUI&) = delete;
            StepByStepGUI(StepByStepGUI&&) = delete;
            auto operator=(const StepByStepGUI&) -> StepByStepGUI& = delete;
            auto operator=(StepByStepGUI&&) -> StepByStepGUI& = delete;
            StepByStepGUI(Sudoku* sudoku, const std::array<uint8_t, static_cast<uint8_t>(global::order* global::order)>& initVals, QWidget* parent);
            ~StepByStepGUI() override = default;

            void drawPrevStep();
            void drawNextStep();
            [[nodiscard]] auto getStepMsg(uint8_t step) const -> QString;

            // eventFilter to scroll through steps with mouse wheel
            auto eventFilter(QObject* watched, QEvent* event) -> bool override;

        private:
            Sudoku* sudoku;
            std::array<uint8_t, static_cast<uint8_t>(global::order* global::order)> initVals;
            uint8_t currentStep;
            uint8_t currentPage;
            bool isPreview;
            QWidget* stepsStack;
            QLabel* previewLabel;
            QLabel* msgLabel;
            QWidget* fieldsWidget;
            QScrollBar* stepsScrollBar;
            QPushButton* firstButton;
            QPushButton* prevButton;
            QPushButton* nextButton;
            QPushButton* lastButton;

        protected:
            void keyPressEvent(QKeyEvent* event) override;
    };
} // namespace sudoku
