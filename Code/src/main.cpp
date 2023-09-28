#include <QtWidgets/QApplication>
#include <algorithm>
#include <fstream>
#include <string>
//#include <iostream>

#include "gui/MainGUI.h"
#include "qtresource.h"

int main(int argc, char* argv[])
{
    /*
    // clang-format off
    // Program version number is timestamp of compilation in "YYYY-MM-DD_hh-mm-ss" format
    constexpr char version[20] = {
        __DATE__[7],
        __DATE__[8],
        __DATE__[9],
        __DATE__[10],
        '-',
        // Month: J F M A M J J A S O N D
        // First month letter, Oct Nov Dec = '1' otherwise '0'
        (__DATE__[0] == 'O' || __DATE__[0] == 'N' || __DATE__[0] == 'D') ? '1' : '0',
        // Second month letter
        (__DATE__[0] == 'J') ?
            (
                (__DATE__[1] == 'a') ? '1' : // Jan, Jun or Jul
                    (
                        (__DATE__[2] == 'n') ? '6' : '7'
                        )
                ) :
            (__DATE__[0] == 'F') ? '2' : // Feb 
                (__DATE__[0] == 'M') ? (__DATE__[2] == 'r') ? '3' : '5' : // Mar or May
                (__DATE__[0] == 'A') ? (__DATE__[1] == 'p') ? '4' : '8' : // Apr or Aug
                (__DATE__[0] == 'S') ? '9' : // Sep
                (__DATE__[0] == 'O') ? '0' : // Oct
                (__DATE__[0] == 'N') ? '1' : // Nov
                (__DATE__[0] == 'D') ? '2' : // Dec
                0,
        '-',
        __DATE__[4],
        __DATE__[5],
        '_',
        __TIME__[0],
        __TIME__[1],
        ':',
        __TIME__[3],
        __TIME__[4],
        ':',
        __TIME__[6],
        __TIME__[7],
    };
    // clang-format on

    // write version to file
    std::ofstream versionFile("version.txt");
    versionFile << version << std::endl;
    versionFile.close();
*/
    Q_INIT_RESOURCE(resources);

    const QApplication app(argc, argv);

    sudoku::MainGUI mainGUI(nullptr);

    mainGUI.show();

    return QApplication::exec();
}
