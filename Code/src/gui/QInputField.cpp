#include <QMessageBox>

#include "gui/QInputField.h"

namespace sudoku
{
    QInputField::QInputField(QWidget* parent)
        : QLineEdit(parent) {}

    void QInputField::keyPressEvent(QKeyEvent* event)
    {
        if (event->key() == Qt::Key_1 || event->key() == Qt::Key_2 || event->key() == Qt::Key_3 || event->key() == Qt::Key_4 || event->key() == Qt::Key_5 || event->key() == Qt::Key_6 || event->key() == Qt::Key_7 || event->key() == Qt::Key_8 || event->key() == Qt::Key_9 ||
            event->key() == Qt::Key_Escape || event->key() == Qt::Key_Insert || event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace || event->key() == Qt::Key_Home || event->key() == Qt::Key_End || event->key() == Qt::Key_PageUp || event->key() == Qt::Key_PageDown || event->key() == Qt::Key_Left || event->key() == Qt::Key_Right || event->key() == Qt::Key_Up || event->key() == Qt::Key_Down ||
            event->key() == Qt::Key_Tab || event->key() == Qt::Key_Backtab || event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter || event->key() == Qt::Key_NumLock || event->key() == Qt::Key_Pause || event->key() == Qt::Key_Print || event->key() == Qt::Key_SysReq ||
            event->key() == Qt::Key_Shift || event->key() == Qt::Key_Control || event->key() == Qt::Key_Meta || event->key() == Qt::Key_Alt || event->key() == Qt::Key_AltGr || event->key() == Qt::Key_NumLock || event->key() == Qt::Key_CapsLock || event->key() == Qt::Key_ScrollLock || (event->key() >= Qt::Key_F1 && event->key() <= Qt::Key_F35))
        {
            QLineEdit::keyPressEvent(event);
        }
        else
        {
            event->ignore();
            QMessageBox messageBox(QMessageBox::Critical, QStringLiteral("Illegal input"), QStringLiteral("<html><center>Input '") + event->text() + QStringLiteral("' not allowed!<br>Allowed inputs are<br>numbers from 1 to 9.</center></html>"), QMessageBox::Ok, this, Qt::WindowFlags());
            messageBox.setStyleSheet(QStringLiteral("color: black; background: rgb(239, 239, 239)"));
            messageBox.adjustSize();
            QRect const fieldRect = QRect(mapToGlobal(rect().topLeft()), mapToGlobal(rect().bottomRight()));
            messageBox.setGeometry(fieldRect.x() + fieldRect.width() / 2 - messageBox.width() / 2,
                                   fieldRect.y() + fieldRect.height() / 2 - messageBox.height() / 2,
                                   messageBox.width(),
                                   messageBox.height());
            messageBox.exec();
        }
    }

    void QInputField::contextMenuEvent(QContextMenuEvent* /*event*/)
    {}
} // namespace sudoku
