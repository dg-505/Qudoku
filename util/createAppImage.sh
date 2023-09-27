#!/bin/bash

if [ -t 0 ]; then
	rm ../release/Qudoku-x86_64.AppImage
	cd ../release/AppImage
	find . ! -name 'Qudoku.desktop' -exec rm -r {} \;
	cd ../../util


	FILE=`zenity --file-selection --title="Select a File"`
	case $? in
         0)
                echo "\"$FILE\" selected.";;
         1)
                echo "No file selected.";;
        -1)
                echo "An unexpected error has occurred.";;
esac
	cp $FILE ../release/AppImage
	cp ./icons/Qudoku_256.png ../release/AppImage
	export PATH=/home/joni/Qt/6.5.2/gcc_64/bin/:$PATH
	./linuxdeployqt-continuous-x86_64.AppImage ../release/AppImage/Qudoku -appimage
	mv Qudoku*.AppImage ../release
else 
	gnome-terminal -e "bash -c './createAppImage.sh; read -p \"Press Enter to exit...\"'"
fi

exit
