#include <QApplication>
#include <QDir>
#include <QFileDialog>
#include <QTextStream>

#include "gui/MainGUI.h"

auto main(int argc, char* argv[]) -> int
{
    Q_INIT_RESOURCE(resources);

    std::string version;
    version = "2.0.0";
    const QApplication app(argc, argv, QT_VERSION);

#ifndef _WIN32
    // if not existent, create config file in /home/<USER>/.qudoku
    auto homeDir = QDir::homePath() + QStringLiteral("/.qudoku");
    QFile iniFile(homeDir + QStringLiteral("/Qudoku.ini"));
    if (!iniFile.exists())
    {
        QDir(QString()).mkdir(homeDir);
        auto dataDir = QFileDialog::getExistingDirectory(nullptr, "Specify data directory", homeDir, QFileDialog::ShowDirsOnly);
        if (dataDir.isEmpty())
        {
            dataDir = homeDir;
        }
        QDir(QString()).mkdir(dataDir + QStringLiteral("/data"));
        if (iniFile.open(QFile::WriteOnly | QFile::Truncate))
        {
            QTextStream ini(&iniFile);
            ini << "[DIRS]\nDataDir=" << dataDir << "\nHomeDir=" << homeDir << "\n\n[I18N]\nlang=en_US\n";
        }
        iniFile.close();
        QFile exampleFile(dataDir + QStringLiteral("/data/example.txt"));
        if (exampleFile.open(QFile::WriteOnly | QFile::Truncate))
        {
            QTextStream example(&exampleFile);
            example << ".3..........195.....8....6.8...6....4..8....1....2.....6....28....419..5.......7.";
        }
        QDir(QString()).mkdir(dataDir + QStringLiteral("/export"));
    }
#endif

    sudoku::MainGUI mainGUI(version, nullptr);
    mainGUI.show();
    return QApplication::exec();
}
