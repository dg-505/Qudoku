convert Qudoku.svg -resize 16x16 Qudoku_16.png
convert Qudoku.svg -resize 24x24 Qudoku_24.png
convert Qudoku.svg -resize 32x32 Qudoku_32.png
convert Qudoku.svg -resize 40x40 Qudoku_40.png
convert Qudoku.svg -resize 48x48 Qudoku_48.png
convert Qudoku.svg -resize 64x64 Qudoku_64.png
convert Qudoku.svg -resize 128x128 Qudoku_128.png
convert Qudoku.svg -resize 256x256 Qudoku_256.png
convert Qudoku_16.png Qudoku_24.png Qudoku_32.png Qudoku_40.png Qudoku_48.png Qudoku_64.png Qudoku_128.png Qudoku_256.png -colors 256 Qudoku.ico

convert uninstall.svg -resize 16x16 uninstall_16.png
convert uninstall.svg -resize 24x24 uninstall_24.png
convert uninstall.svg -resize 32x32 uninstall_32.png
convert uninstall.svg -resize 40x40 uninstall_40.png
convert uninstall.svg -resize 48x48 uninstall_48.png
convert uninstall.svg -resize 64x64 uninstall_64.png
convert uninstall.svg -resize 128x128 uninstall_128.png
convert uninstall.svg -resize 256x256 uninstall_256.png
convert uninstall_16.png uninstall_24.png uninstall_32.png uninstall_40.png uninstall_48.png uninstall_64.png uninstall_128.png uninstall_256.png -colors 256 uninstall.ico

cp Qudoku.ico ../../Code/res
cp first.png ../../Code/res
cp prev.png ../../Code/res
cp next.png ../../Code/res
cp last.png ../../Code/res

cmd /k
