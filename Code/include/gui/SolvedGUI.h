#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QSettings>

#include "sudoku/Sudoku.h"

namespace sudoku
{
    class SolvedGUI : public QMainWindow
    {
            Q_OBJECT
        public:
            SolvedGUI(const SolvedGUI&) = delete;
            SolvedGUI(SolvedGUI&&) = delete;
            auto operator=(const SolvedGUI&) -> SolvedGUI& = delete;
            auto operator=(SolvedGUI&&) -> SolvedGUI& = delete;
            SolvedGUI(Sudoku* sudoku, const std::array<uint8_t, static_cast<uint8_t>(global::order* global::order)>& initVals, std::string  name, QSettings* settings, QWidget* parent);
            ~SolvedGUI() override = default;

            static auto renderPixmap(QWidget* parent) -> QPixmap;
            auto exportPNG(QWidget* parent) -> void;
            auto exportPDF(QWidget* parent) -> void;

            // Helper functions
            static void drawFields(Step& currStep, Step& nextStep, const std::array<uint8_t, static_cast<uint8_t>(global::order* global::order)>& initVals, QWidget* parent);
            static void drawFrame(QWidget* parent);

        protected:
            void keyPressEvent(QKeyEvent* event) override;

        private:
            std::string _name;
            QWidget* _gridWidget;
            QSettings* _settings;
            QPushButton* _exportPngButton;
            QPushButton* _exportPdfButton;
    };
} // namespace sudoku
