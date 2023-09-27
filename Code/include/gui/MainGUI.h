#pragma once

#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
// #include <string>

#include "globals.h"
#include "gui/QInputField.h"
#include "gui/QLogTextBrowser.h"
#include "sudoku/Sudoku.h"

namespace sudoku
{
    class MainGUI : public QMainWindow
    {
            Q_OBJECT
        public:
            explicit MainGUI(QWidget* parent);
            ~MainGUI() = default;

        private:
            QScrollArea* logScrollArea;
            QLogTextBrowser* logTextBrowser;
            QLabel* titleLabel;
            QWidget* sudokuPanel{};
            //            QInputField* field;
            std::array<QInputField*, static_cast<uint8_t>(order* order)>* fields{};
            QFrame* hLine0;
            QFrame* hLine1;
            QFrame* hLine2;
            QFrame* hLine3;
            QFrame* vLine0;
            QFrame* vLine1;
            QFrame* vLine2;
            QFrame* vLine3;
            QPushButton* loadButton;
            // QPushButton* validateButton;
            QPushButton* saveButton;
            QPushButton* candidatesButton;
            QPushButton* stepByStepButton;
            QPushButton* solveButton;
            QPushButton* clearButton;
            QPushButton* exitButton;
            std::string filename;

            Sudoku init(uint8_t* initVals) const;
            void clear();

            std::string loadButtonClicked();
            const void saveButtonClicked() const;
            const void candidatesButtonClicked() const;
            const void stepByStepButtonButtonClicked() const;
            const void solveButtonClicked() const;
            void clearButtonClicked();

            void closeEvent(QCloseEvent *bar);
    };
} // namespace sudoku
