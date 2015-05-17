#!/bin/sh

set -e
set -x

#if OS is linux or is not set
if [ "$TRAVIS_OS_NAME" = "linux" -o -z "$TRAVIS_OS_NAME" ]; then
    cd QT
    qmake -qt=qt5 -v
    qmake -qt=qt5 -r HiveQt.pro
    make
elif [ "$TRAVIS_OS_NAME" = "osx" ]; then
fi