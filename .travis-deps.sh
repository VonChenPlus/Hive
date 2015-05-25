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
    cwd=$(pwd)
    svn co --quiet http://llvm.org/svn/llvm-project/libcxx/trunk@201601 libcxx
    cd libcxx/lib && bash buildit
    sudo cp ./libc++.so.1.0 /usr/lib/
    sudo mkdir /usr/include/c++/v1
    cd .. && sudo cp -r include/* /usr/include/c++/v1/
    cd /usr/lib && sudo ln -sf libc++.so.1.0 libc++.so
    sudo ln -sf libc++.so.1.0 libc++.so.1 && cd $cwd
fi