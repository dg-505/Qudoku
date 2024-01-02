#include <QApplication>

#include "gui/MainGUI.h"

auto main(int argc, char* argv[]) -> int
{
    Q_INIT_RESOURCE(resources);

    std::string version;
    version = "2.0.0";
    const QApplication app(argc, argv, QT_VERSION);
    sudoku::MainGUI mainGUI(version, nullptr);
    mainGUI.show();
    return QApplication::exec();
}
