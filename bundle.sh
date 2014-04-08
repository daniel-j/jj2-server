#!/bin/bash

cd ${0%/*}

mkdir build-jj2-server-console-release
cd build-jj2-server-console-release
qmake ../src/jj2-server-console.pro
make

cd ..

mkdir build-jj2-server-gui-release
cd build-jj2-server-gui-release
qmake ../src/jj2-server-gui.pro
make

cd ..

linuxdir=builds/linux-$(uname -m)/
rm -rf "$linuxdir"
mkdir -p "$linuxdir"

echo "copying binaries.."
cp build-jj2-server-console-release/jj2-server-console "$linuxdir"
cp build-jj2-server-gui-release/jj2-server-gui "$linuxdir"
cp -r lib64/* "$linuxdir"
cd "$linuxdir"

echo "removing rpath from binaries..."
chrpath -d jj2-server-console
chrpath -d jj2-server-gui

echo "console:"
LD_LIBRARY_PATH=. ldd ./jj2-server-console

echo "gui:"
LD_LIBRARY_PATH=. ldd ./jj2-server-gui

echo "making zip.."
rm ../linux-$(uname -m).zip
zip -r ../linux-$(uname -m).zip .

cd ../../

