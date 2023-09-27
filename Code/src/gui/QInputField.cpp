#include <QtWidgets/QMessageBox>
//#include <iostream>

#include "gui/QInputField.h"

//using std::cout;
//using std::endl;

namespace sudoku
{
    QInputField::QInputField(QWidget* parent)
        : QLineEdit(parent) {}

    void QInputField::keyPressEvent(QKeyEvent* event)
    {
        // QChar key = event->text()[0];
        if (event->key() == Qt::Key_1 || event->key() == Qt::Key_2 || event->key() == Qt::Key_3 || event->key() == Qt::Key_4 || event->key() == Qt::Key_5 || event->key() == Qt::Key_6 || event->key() == Qt::Key_7 || event->key() == Qt::Key_8 || event->key() == Qt::Key_9)
        {
            // event->accept();
            QLineEdit::keyPressEvent(event);
        }
        else if (event->key() == Qt::Key_Escape || event->key() == Qt::Key_Insert || event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace || event->key() == Qt::Key_Home || event->key() == Qt::Key_End || event->key() == Qt::Key_PageUp || event->key() == Qt::Key_PageDown || event->key() == Qt::Key_Left || event->key() == Qt::Key_Right || event->key() == Qt::Key_Up || event->key() == Qt::Key_Down || event->key() == Qt::Key_Tab || event->key() == Qt::Key_Backtab || event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter || event->key() == Qt::Key_NumLock || event->key() == Qt::Key_Pause || event->key() == Qt::Key_Print || event->key() == Qt::Key_SysReq || event->key() == Qt::Key_Shift || event->key() == Qt::Key_Control || event->key() == Qt::Key_Meta || event->key() == Qt::Key_Alt || event->key() == Qt::Key_AltGr || event->key() == Qt::Key_NumLock || event->key() == Qt::Key_CapsLock || event->key() == Qt::Key_ScrollLock || (event->key() >= Qt::Key_F1 && event->key() <= Qt::Key_F35))
        {
            // event->accept();
            QLineEdit::keyPressEvent(event);
        }
        else
        {
            event->ignore();
            QMessageBox messageBox;
            messageBox.setWindowTitle("Illegal input");
            messageBox.setText(QString("<html><center>Input '%1' not allowed!<br>Allowed inputs are<br>numbers from 1 to 9.</center></html>").arg(event->text()));
            messageBox.setIcon(QMessageBox::Critical);
            messageBox.addButton(QMessageBox::Ok);
            messageBox.adjustSize();
            // cout << mapToGlobal(rect().topLeft()).x() << "," << mapToGlobal(rect().topLeft()).y() << "  ,  " << mapToGlobal(rect().bottomRight()).x() << "," << mapToGlobal(rect().bottomRight()).y() << endl;
            QRect const fieldRect = QRect(mapToGlobal(rect().topLeft()), mapToGlobal(rect().bottomRight()));
            messageBox.setGeometry(fieldRect.x() + fieldRect.width() / 2 - messageBox.width() / 2,
                                   fieldRect.y() + fieldRect.height() / 2 - messageBox.height() / 2,
                                   messageBox.width(),
                                   messageBox.height());
            messageBox.exec();
        }
    }
    void QInputField::contextMenuEvent(QContextMenuEvent* event)
    {
        Q_UNUSED(event); // Unused parameter to avoid a compiler warning
        // Do nothing to disable the context menu
    }
} // namespace sudoku
