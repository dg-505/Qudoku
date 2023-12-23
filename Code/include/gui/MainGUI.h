#pragma once

// #include <QtCore/QThread>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>

#include "globals.h"
#include "gui/QInputField.h"
#include "gui/QLogTextBrowser.h"
#include "sudoku/Sudoku.h"

namespace sudoku
{

    //    class WorkerThread : public QThread
    //    {
    //            Q_OBJECT
    //        public:
    //            WorkerThread(std::array<QInputField*, static_cast<uint8_t>(global::order* global::order)>* fields, std::array<uint8_t, static_cast<uint8_t>(global::order* global::order)>* vals, QLogTextBrowser* logTextArea, bool nakedSinglesEnabled, bool hiddenSinglesEnabled, bool nakedPairsEnabled, bool hiddenPairsEnabled, bool nakedTriplesEnabled, bool hiddenTriplesEnabled, bool blockLineChecksEnabled, bool lineBlockChecksEnabled, bool backtrackingEnabled)
    //                : _fields(fields),
    //                  _vals(vals),
    //                  _logTextArea(logTextArea),
    //                  _nakedSinglesEnabled(nakedSinglesEnabled),
    //                  _hiddenSinglesEnabled(hiddenSinglesEnabled),
    //                  _nakedPairsEnabled(nakedPairsEnabled),
    //                  _hiddenPairsEnabled(hiddenPairsEnabled),
    //                  _nakedTriplesEnabled(nakedTriplesEnabled),
    //                  _hiddenTriplesEnabled(hiddenTriplesEnabled),
    //                  _blockLineChecksEnabled(blockLineChecksEnabled),
    //                  _lineBlockChecksEnabled(lineBlockChecksEnabled),
    //                  _backtrackingEnabled(backtrackingEnabled) {}

    //            void run() override
    //            {
    //                QString result;
    //                /* ... here is the expensive or blocking operation ... */
    //                for (uint8_t i = 1; i <= global::order * global::order; i++)
    //                {
    //                    try
    //                    {
    //                        _vals->at(i - 1) = static_cast<uint8_t>(std::stoi(this->_fields->at(i - 1)->text().toStdString(), nullptr, global::base));
    //                    }
    //                    catch (const std::exception& e)
    //                    {
    //                        _vals->at(i - 1) = 0;
    //                    }
    //                }
    //                auto* sudoku = new Sudoku(this->_vals, this->_logTextArea, this->_nakedSinglesEnabled, this->_hiddenSinglesEnabled, this->_nakedPairsEnabled, this->_hiddenPairsEnabled, this->_nakedTriplesEnabled, this->_hiddenTriplesEnabled, this->_blockLineChecksEnabled, this->_lineBlockChecksEnabled, this->_backtrackingEnabled);
    //                sudoku->solve("");
    //                emit resultReady(result);
    //            }
    //        signals:
    //            void resultReady(const QString& s);

    //        private:
    //            std::array<QInputField*, static_cast<uint8_t>(global::order* global::order)>* _fields;
    //            std::array<uint8_t, static_cast<uint8_t>(global::order* global::order)>* _vals;
    //            QLogTextBrowser* _logTextArea;
    //            bool _nakedSinglesEnabled;
    //            bool _hiddenSinglesEnabled;
    //            bool _nakedPairsEnabled;
    //            bool _hiddenPairsEnabled;
    //            bool _nakedTriplesEnabled;
    //            bool _hiddenTriplesEnabled;
    //            bool _blockLineChecksEnabled;
    //            bool _lineBlockChecksEnabled;
    //            bool _backtrackingEnabled;
    //    };

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

            // void handleResults();

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

            auto init(std::array<uint8_t, static_cast<uint8_t>(global::order* global::order)>* initVals) const -> Sudoku*;
            void clear();

            void techniquesButtonClicked();
            auto loadButtonClicked() -> std::string;
            void saveButtonClicked() const;
            void candidatesButtonClicked();
            void stepByStepButtonClicked();
            void solveButtonClicked();
            void clearButtonClicked();

            void closeEvent(QCloseEvent* /*event*/) override;

            //void startWorkInAThread();

        protected:
            void keyPressEvent(QKeyEvent* event) override;
    };
} // namespace sudoku
