#pragma once

#include <QTextBrowser>
#include <QWheelEvent>

namespace sudoku
{
    class QLogTextBrowser : public QTextBrowser
    {
            Q_OBJECT // NOLINT
        public:
            explicit QLogTextBrowser(QWidget* parent);
            QLogTextBrowser(const QLogTextBrowser&) = delete;
            QLogTextBrowser(QLogTextBrowser&&) = delete;
            auto operator=(const QLogTextBrowser&) -> QLogTextBrowser& = delete;
            auto operator=(QLogTextBrowser&&) -> QLogTextBrowser& = delete;
            ~QLogTextBrowser() override = default;

        protected:
            void wheelEvent(QWheelEvent* event) override;
    };
} // namespace sudoku
