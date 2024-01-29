#pragma once

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QString>

namespace sudoku
{
    class DataDirDialog : public QDialog
    {
            Q_OBJECT
        public:
            DataDirDialog(const DataDirDialog&) = delete;
            DataDirDialog(DataDirDialog&&) = delete;
            auto operator=(const DataDirDialog&) -> DataDirDialog& = delete;
            auto operator=(DataDirDialog&&) -> DataDirDialog& = delete;
            explicit DataDirDialog(QWidget* parent);
            ~DataDirDialog() override = default;

            void browse();
            void useDefault();
            void proceed();

            [[nodiscard]] auto getDataDir() const -> QString;

        Q_SIGNALS:
            void directorySelected(const QString &dir);

        private:
            QLabel* _descLabel;
            QLineEdit* _lineEdit;
            QPushButton* _browseButton;
            QPushButton* _defaultButton;
            QPushButton* _cancelButton;
            QPushButton* _proceedButton;
            QString _defaultDir;
            QString _dataDir;
    };
} // namespace sudoku
