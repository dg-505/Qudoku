#include <QtWidgets/QApplication>
//#include <iostream>

#include "gui/MainGUI.h"
#include "qtresource.h"

auto main(int argc, char* argv[]) -> int
{
    Q_INIT_RESOURCE(resources);

    const QApplication app(argc, argv);

    sudoku::MainGUI mainGUI(nullptr);

    mainGUI.show();

    return QApplication::exec();
}
