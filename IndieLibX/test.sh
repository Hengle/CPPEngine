#!/bin/bash
#
# Copyright (c) 2015 Tommi Roenty   http://www.tommironty.fi/
# Licensed under The GNU Lesser General Public License, version 2.1:
#     http://opensource.org/licenses/LGPL-2.1
#
source ./make.cnf
case "$OSTYPE" in
    darwin*)
        platform='OSX'
        ;; 
    linux*)
        platform='LNX'  
        ;;
  *);;
esac
#./make.sh
#./make.sh AllLibs $platform
#sudo mv lib/*.* /usr/local/lib
./make.sh
./make.sh Test $platform
./run.sh
./make.sh
./make.sh SampleApps $platform
./run.sh
./make.sh
#./make.sh TerrainApps $platform
#./run.sh
#./make.sh
./make.sh IndieLibApps $platform
./run.sh
./make.sh
exit 0
if [[ $OSTYPE == linux* ]]; then
cd ./scripting
make all
make clean
cd ..
fi
case "$OSTYPE" in
    darwin*)
        ./make.sh Test iOS
        ;; 
    linux*)
        ./make.sh Test NDK
        ;;
  *);;
esac
./make.sh

