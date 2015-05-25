#!/bin/sh

set -e
set -x

#if OS is linux or is not set
if [ "$CXX" = "g++" ]; then
    sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y
    sudo add-apt-repository ppa:ubuntu-sdk-team/ppa -y
    sudo apt-get -qq update
    sudo apt-get -qq install qt5-qmake qtbase5-dev qtdeclarative5-dev
    sudo apt-get -qq install g++-4.9 xorg-dev libglu1-mesa-dev libxcursor-dev
    sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.9 90
elif [ "$CXX" = "clang++" ]; then
    sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y
    sudo add-apt-repository ppa:ubuntu-sdk-team/ppa -y
    sudo apt-get -qq update
    sudo apt-get -qq install qt5-qmake qtbase5-dev qtdeclarative5-dev
    sudo apt-get install -y llvm-3.4 llvm-3.4-dev
fi