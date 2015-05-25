#!/bin/sh

set -e
set -x

#if OS is linux or is not set
if [ "$CXX" = "g++" ]; then
    cd QT
    qmake -qt=qt5 -v
    qmake -qt=qt5 -r HiveQt.pro
    make
elif [ "$CXX" = "clang++" ]; then
    cd QT
    qmake -qt=qt5 -v
    qmake -qt=qt5 -makefile HiveQt.pro -spec macx-llvm
    make
fi