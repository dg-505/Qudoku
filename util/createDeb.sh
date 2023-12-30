#!/bin/bash

set -e

if [ -t 0 ]; then
    # clear output
    echo -n "Clearing output..."
    rm -f ../release/*.deb
    rm -rf ../release/deb
    echo "done"

    # get version number from InnoSetup script
    version=$(grep "#define Version" ./createSetup.iss)
    version=$(echo "${version}" | sed 's/^.\{17\}\(.*\)..$/\1/')
    echo "Building Qudoku version ${version}"

    ### generate DEBIAN directory wit control, postinst, and postrm files
    echo -n "Generating maintainer scripts in 'DEBIAN' directory..."
    mkdir -p ../release/deb/qudoku/DEBIAN
    control=$(cat <<EOF
Package: Qudoku
Version: $version
Architecture: amd64
Maintainer: dg-505 <dg-505@github.com>
Homepage: https://github.com/dg-505/Qudoku
Description: Qt based cross platform sudoku solver
EOF
)
    echo "${control}" > ../release/deb/qudoku/DEBIAN/control

    postinst=$(cat <<EOF
#!/bin/bash

set -e

chmod -R 766 /usr/share/qudoku/*

sudo update-desktop-database
EOF
)
    echo "${postinst}" > ../release/deb/qudoku/DEBIAN/postinst
    chmod +x ../release/deb/qudoku/DEBIAN/postinst

    echo "done"

    ### generate "/usr/bin" directory with executable binary and "platforms/libqxcb.so"
    mkdir -p ../release/deb/qudoku/usr/bin/platforms
    # choose executable binary
    EXEC_BINARY=`zenity --file-selection --title="Select executable binary" --filename=../build/Focal/Release/Qudoku`
    echo "Executable binary: \"${EXEC_BINARY}\""

    # coose Qt directory
    QT_DIR=`zenity --file-selection --directory --title="Select Qt directory" --filename=/home/$USER/Qt/5.15.16/gcc_64/`
    echo "Qt directory: \"${QT_DIR}\""

    echo -n "Copying files..."

    cp ${EXEC_BINARY} ../release/deb/qudoku/usr/bin/

    # create ini file with path to data directory
    ini="[DIRS]\nDataDir=/usr/share/qudoku/data"
    echo -e "$ini" > ../release/deb/qudoku/usr/bin/Qudoku.ini

    ### Copy required libaries
    cp ${QT_DIR}/plugins/platforms/libqxcb.so ../release/deb/qudoku/usr/bin/platforms/

    mkdir -p ../release/deb/qudoku/usr/lib
    libs=("libicudata.so.56" "libicui18n.so.56" "libicuuc.so.56" "libQt5Core.so.5" "libQt5DBus.so.5" "libQt5Gui.so.5" "libQt5Widgets.so.5" "libQt5XcbQpa.so.5")
    for lib in "${libs[@]}"; do
        cp ${QT_DIR}/lib/${lib} ../release/deb/qudoku/usr/lib/
    done

    # libxcb-xinerama.so.0 libxcb-xinerama.so.0.0.0
    cp /usr/lib/x86_64-linux-gnu/libxcb-xinerama.so.0 ../release/deb/qudoku/usr/lib/

    ### generate "/usr/share" directory with .desktop file, icons and  example.txt
    mkdir -p ../release/deb/qudoku/usr/share/applications
    desktop=$(cat <<EOF
[Desktop Entry]
Type        = Application
Name        = Qudoku
Version     = ${version}
Exec        = Qudoku
GenericName = Sudoku solver
Icon        = qudoku
Categories  = Utility;Game;
Comment     = Qt based cross platform sudoku solver
EOF
)
    echo "${desktop}" > ../release/deb/qudoku/usr/share/applications/qudoku.desktop

    mkdir -p ../release/deb/qudoku/usr/share/qudoku/data/
    cp ../Code/data/example.txt ../release/deb/qudoku/usr/share/qudoku/data/

    sizes=(16 24 32 40 48 64 128 256)
    for size in "${sizes[@]}";
    do
        mkdir -p ../release/deb/qudoku/usr/share/icons/hicolor/${size}x${size}/apps/
        cp ./icons/Qudoku_${size}.png ../release/deb/qudoku/usr/share/icons/hicolor/${size}x${size}/apps/qudoku.png
    done
    mkdir -p ../release/deb/qudoku/usr/share/icons/hicolor/scalable/apps/
    cp ./icons/Qudoku.svg ../release/deb/qudoku/usr/share/icons/hicolor/scalable/apps/qudoku.svg

    echo "done"

    ### build deb package
    dpkg-deb --build ../release/deb/qudoku

    mv ../release/deb/qudoku*.deb ../release/qudoku-${version}.deb

else
    gnome-terminal -e "bash -c './createDeb.sh; read -p \"Press Enter to exit...\"'"
fi

exit
