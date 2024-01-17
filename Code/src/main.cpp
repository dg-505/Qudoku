#include <QApplication>
#include <QDir>
#include <QTextStream>

#include "gui/MainGUI.h"

auto main(int argc, char* argv[]) -> int
{
    Q_INIT_RESOURCE(resources);

#ifndef _WIN32
    // if not existent, create config file and example in /home/<USER>/.qudoku
    auto cfgDir = QDir::homePath() + QStringLiteral("/.qudoku");
    if (!QDir(cfgDir).exists())
    {
        QDir(QString()).mkdir(cfgDir);
        QFile iniFile(cfgDir + QStringLiteral("/Qudoku.ini"));
        if (iniFile.open(QFile::WriteOnly | QFile::Truncate))
        {
            QTextStream ini(&iniFile);
            ini << "[DIRS]\nDataDir=" << cfgDir << "/data\n\n[I18N]\nlang=en_US\n";
        }
        auto dataDir = cfgDir + QStringLiteral("/data");
        QDir(QString()).mkdir(dataDir);
        QFile exampleFile(dataDir + QStringLiteral("/example.txt"));
        if (exampleFile.open(QFile::WriteOnly | QFile::Truncate))
        {
            QTextStream example(&exampleFile);
            example << ".3..........195.....8....6.8...6....4..8....1....2.....6....28....419..5.......7.";
        }
    }
#endif

    std::string version;
    version = "2.0.0";
    const QApplication app(argc, argv, QT_VERSION);
    sudoku::MainGUI mainGUI(version, nullptr);
    mainGUI.show();
    return QApplication::exec();
}
