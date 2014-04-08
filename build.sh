#!/bin/bash

cd ${0%/*}


echo "copying binaries.."
cp build-jj2-server-console-Desktop-Release/jj2-server-console builds/linux64/
cp build-jj2-server-gui-Desktop-Release/jj2-server-gui builds/linux64/
cd builds/linux64/

echo "removing rpath from binaries"
chrpath -d jj2-server-console
chrpath -d jj2-server-gui

echo "console:"
LD_LIBRARY_PATH=. ldd ./jj2-server-console

echo "gui:"
LD_LIBRARY_PATH=. ldd ./jj2-server-gui

echo "making zip.."
rm ../linux64.zip
zip -r ../linux64.zip .

cd ../../


