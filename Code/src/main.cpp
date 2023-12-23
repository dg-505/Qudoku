#include <QtWidgets/QApplication>

#include "gui/MainGUI.h"
#include "qtresource.h"

auto main(int argc, char* argv[]) -> int
{
    Q_INIT_RESOURCE(resources);

    std::string version;
    version = "1.3.0";
    const QApplication app(argc, argv, QT_VERSION);
    sudoku::MainGUI mainGUI(version, nullptr);
    mainGUI.show();
    return QApplication::exec();
}
