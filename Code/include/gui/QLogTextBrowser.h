#pragma once

#include <QtGui/QWheelEvent>
#include <QtWidgets/QTextBrowser>

namespace sudoku
{
    class QLogTextBrowser : public QTextBrowser
    {
            Q_OBJECT
        public:
            explicit QLogTextBrowser(QWidget* parent = nullptr);

            QLogTextBrowser(const QLogTextBrowser&) = delete;
            QLogTextBrowser(QLogTextBrowser&&) = delete;
            auto operator=(const QLogTextBrowser&) -> QLogTextBrowser& = delete;
            auto operator=(QLogTextBrowser&&) -> QLogTextBrowser& = delete;

            ~QLogTextBrowser() override = default;

        protected:
            void wheelEvent(QWheelEvent* event) override;
    };
} // namespace sudoku
