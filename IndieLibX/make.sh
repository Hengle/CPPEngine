#!/bin/bash
#
# Copyright (c) 2015 Tommi Roenty   http://www.tommironty.fi/
# Licensed under The GNU Lesser General Public License, version 2.1:
#     http://opensource.org/licenses/LGPL-2.1
#
source ./make.cnf

PYTHON=`which python`
if [ -z "$PYTHON" ]; then
	echo "Could not find python."
	exit 1
fi

OS='Windows'
platformsArray=( NDK NMAKE VC MW PRO CMake )
makesystem="NMAKE"
if [ -f "/etc/lsb-release" ]; then
	DINFO=$(cat /etc/lsb-release | grep DISTRIB_ID | sed s/.*=//)
	if [ $DINFO == "Ubuntu" ]; then
		OS='Ubuntu'
		platformsArray=( NDK LNX MW PRO CMake )
		makesystem="LNX"
	fi
elif [ -f "/etc/debian_version" ]; then
	OS='Debian'
	platformsArray=( NDK LNX MW PRO CMake )
	makesystem="LNX"
elif [ -d "/Applications/Xcode.app" ]; then
	OS='Darwin'
	platformsArray=( NDK LNX iOS OSX MW PRO CMake )
	makesystem='OSX'
elif [ -d "/Applications/Xcode-Beta.app" ]; then
	OS='Darwin'
	platformsArray=( NDK LNX iOS OSX MW PRO CMake )
	makesystem='OSX'
fi

Bit32p64=''
if [ $(uname -m) == "i686" ]; then
	Bit32p64='i686'
fi

PTHREAD=''
if [ -f "/usr/include/pthread.h" ];
then
	PTHREAD='-pthread'
fi
export PTHREAD

export OS
export Bit32p64

#RAKE=`which rake`
if [ "$OS" == "Darwin" ]; then
	#if [ -z "$RAKE" ]; then
	#	echo "Could not find rake."
	#	exit 1
	#fi

	# Global vars
	if [ -z ${SDK_NAME} ]; then
		UFW_GREP_RESULT=$(xcodebuild -showsdks | grep -o "iphoneos.*$")
		while read -r line; do
		iOS_VERSION="${line}"
		done <<< "${UFW_GREP_RESULT}"
		UFW_GREP_RESULT2=$(xcodebuild -showsdks | grep -o "macosx.*$")
		while read -r line; do
		MACOSX_VERSION="${line}"
		done <<< "${UFW_GREP_RESULT2}"

	fi

	iOS_VERSION=$(echo "${iOS_VERSION}" | grep -o "[0-9].*$")
	MACOSX_VERSION=$(echo "${MACOSX_VERSION}" | grep -o "[0-9].*$")


	# Extract the latest SDK version from the final field of the form: iphoneosX.Y
	readonly iOS_SDK=$(xcodebuild -showsdks | grep iphoneos | sort | tail -n 1 | awk '{print substr($NF, 9)}')
	readonly OSX_SDK=$(xcodebuild -showsdks | grep macosx | sort | tail -n 1 | awk '{print substr($NF, 7)}')
	# Extract Xcode version.
	readonly XCODE=$(xcodebuild -version | grep Xcode | cut -d " " -f2)

	echo "iOS_VERSION: $iOS_VERSION"
	echo "MACOSX_VERSION: $MACOSX_VERSION"
	echo "iOS_SDK: $iOS_SDK"
	echo "OSX_SDK: $OSX_SDK"
	echo "XCODE: $XCODE"

	export iOS_VERSION
	export MACOSX_VERSION
	export iOS_SDK
	export OSX_SDK
	export XCODE
fi

solution="Test"

s=$solutions
p=$platforms

usage()
{
echo "Usage: $0 -> Removes Temporary Files."
echo "Usage: $0 [< ${solutions} >] [< LNX|NDK|OSX|iOS|NMAKE >] -> Builds an solution.";
#echo "Usage: $0 buildlibrary ProjectName [< ${projects} >] -> Builds an single project.";
echo "Usage: $0 buildprj [< ${projects} >] [< LNX|NDK|OSX|iOS|NMAKE >] -> Builds an single project.";
echo "Usage: $0 buildsln [< ${solutions} >] [< LNX|NDK|OSX|iOS|NMAKE >] -> Builds an solution.";
}

# ./make.sh buildsln AllLibs LNX 
if [ "$1" == "buildsln" ]; then
	MINPARAMS=2
	if [ $# -lt "$MINPARAMS" ]
	then
		usage
		exit 0
	fi
	bt=LNX
	if [ -n "$3" ]; then
		bt=$3
	fi
	cd ./DesktopProjects
	echo "#4:$PYTHON build.py --buildsln $2 --buildtool $bt"
	$PYTHON build.py --buildsln $2 --buildtool $bt 2>&1 | tee ../build.log
	cd ..
	exit 0
fi

# ./make.sh buildprj STX LNX 
if [ "$1" == "buildprj" ]; then
	MINPARAMS=2
	if [ $# -lt "$MINPARAMS" ]
	then
		usage
		exit 0
	fi
	bt=LNX
	if [ -n "$3" ]; then
		bt=$3
	fi
	cd ./DesktopProjects
	echo "#4:$PYTHON build.py --buildprj $2 --buildtool $bt"
	$PYTHON build.py --buildprj $2 --buildtool $bt 2>&1 | tee ../build.log
	cd ..
	exit 0
fi

# ./make.sh buildlibrary STX LNX 
if [ "$1" == "buildlibrary" ]; then

	MINPARAMS=2
	if [ $# -lt "$MINPARAMS" ]
	then
		usage
		exit 0
	fi

	bt=LNX
	if [ -n "$3" ]; then
		bt=$3
	fi
	export esolution=AllLibs
	export emakesystem=$bt
	cd ./DesktopProjects
	echo "#4a:$PYTHON Projects.py --solution AllLibs --buildtool $bt --buildproject $2"
	$PYTHON Projects.py --solution AllLibs --buildtool $bt --buildproject $2
	echo "#4b:$PYTHON Projects.py --buildtool $bt --solution None --mainphoneproject a_02_IND_Surface --package com.IndieLibApps.a_02_IND_Surface --apps True --buildproject $2"
	$PYTHON Projects.py --buildtool $bt --solution None --mainphoneproject a_02_IND_Surface --package com.IndieLibApps.a_02_IND_Surface --apps True --buildproject $2
	cd ..
	if [ -e "./DesktopProjects/projectsX/Makefile" ]; then 
	make -C ./DesktopProjects/projectsX all 2>&1 | tee build.log
	fi
	exit 0
fi

solution1=$1

NDK_gl_code()
{
	JNI="./Libraries/MVC/NDK/build/$package/jni"
	mkdir -p $JNI
	cd ./DesktopProjects
	echo "#1a:$PYTHON Projects.py --solution $solution1 --buildtool NDK"
	$PYTHON Projects.py --solution $solution1 --buildtool NDK
	cd ..
	cd ./DesktopProjects
	echo "#1b:$PYTHON Projects.py --buildtool NDK --solution None --mainphoneproject $mainphoneproject --package $package --TargetDirectory ../Libraries/MVC/NDK/jni --OpenAL True --NDKPlatform 19"
	$PYTHON Projects.py --buildtool NDK --solution None --mainphoneproject $mainphoneproject --package $package --TargetDirectory ../Libraries/MVC/NDK/jni --OpenAL True --NDKPlatform 19
	cd ..
	cd ./DesktopProjects/NDK_gl_code
	./androidbuild.sh $package ../test/testgles2.c
	cd ../..
	rm -fr $JNI/src
	cd ./DesktopProjects/NDK_gl_code
	./androidbuild3.sh $package
	cd ../..
}

NDK_gl_code2()
{
	JNI="./Libraries/MVC/NDK/build/$package/jni"
	mkdir -p $JNI
	cd ./DesktopProjects
	echo "#2a:$PYTHON Projects.py --solution $solution1 --buildtool NDK"
	$PYTHON Projects.py --solution $solution1 --buildtool NDK
	cd ..
	cd ./DesktopProjects
	echo "#2b:$PYTHON Projects.py --buildtool NDK --solution None --mainphoneproject $mainphoneproject --package $package --TargetDirectory ../Libraries/MVC/NDK/jni --OpenAL True --NDKPlatform 19"
	$PYTHON Projects.py --buildtool NDK --solution None --mainphoneproject $mainphoneproject --package $package --TargetDirectory ../Libraries/MVC/NDK/jni --OpenAL True --NDKPlatform 19
	cd ..
	cd ./DesktopProjects/NDK_gl_code2
	./androidbuild.sh $package ../test/testgles2.c
	cd ../..
	rm -fr $JNI/src
	cd ./DesktopProjects/NDK_gl_code2
	./androidbuild3.sh $package
	cd ../..
}

NDK_SDL2()
{
	JNI="./Libraries/SDL2-$sdl2version/build/$package/jni"
	mkdir -p $JNI
	cd ./DesktopProjects
	echo "#3a:$PYTHON Projects.py --solution $solution1 --buildtool NDK"
	$PYTHON Projects.py --solution $solution1 --buildtool NDK
	cd ..
	cd ./DesktopProjects
	echo "#3b:$PYTHON Projects.py --buildtool NDK --solution None --mainphoneproject $mainphoneproject --package $package --TargetDirectory ../Libraries/SDL2-$sdl2version/build/$package/jni --OpenAL True --NDKPlatform 19"
	$PYTHON Projects.py --buildtool NDK --solution None --mainphoneproject $mainphoneproject --package $package --TargetDirectory ../Libraries/SDL2-$sdl2version/build/$package/jni --OpenAL True --NDKPlatform 19
	cd ..
	rm -fr $JNI/SDL2
	cd ./DesktopProjects/NDK_SDL2
	./androidbuild.sh $package ../test/testgles2.c
	cd ../..
	rm -fr $JNI/src
	cd ./DesktopProjects/NDK_SDL2
	./androidbuild3.sh $package
	cd ../..
}

if [ "$2" == "OSX" ]; then
	echo "2 Building to MacOS$MACOSX_VERSION"
fi
if [ "$2" == "iOS" ]; then
	echo "2 Building to iOS$iOS_VERSION"
	#cd ./DesktopProjects
	#echo "$PYTHON Projects.py --solution $solution --buildtool iOS"
	#$PYTHON Projects.py --solution $solution --buildtool iOS
	#echo "$PYTHON Projects.py --buildtool iOS --solution None --mainphoneproject $mainphoneproject --package $package --apps True"
	#$PYTHON Projects.py --buildtool iOS --solution None --mainphoneproject $mainphoneproject --package $package --apps True
	#cd ..
	#if [ -n "$RAKE" ]; then
	#	cd ./DesktopProjects/projectsX
	#	rake xcode && rake xcodebuild
	#	cd ../..
	#fi
	#make solution
	#exit 0
fi

if [ "$2" == "NDK" ]; then
	#NDK_SDL2
	NDK_gl_code2
	exit 0
fi

if [ "$1" == "help" ]; then
	usage
	exit 0
fi

if [[ ${platformsArray[*]} =~ "$makesystem" ]]; then
	echo ""
else
	usage
	exit 0
fi

if [ -z "$1" ] && [ -z "$2" ]; then
	#usage
	make clean
	exit 0
else
	if [ -n "$1" ]; then
		solution=$1
	fi
	if [ -n "$2" ]; then
		makesystem=$2
	fi
fi

clean()
{
	make clean
}

run()
{
export esolution=$solution
export emakesystem=$makesystem
cd ./DesktopProjects
echo "#4:$PYTHON Projects.py --solution $solution --buildtool $makesystem"
$PYTHON Projects.py --solution $solution --buildtool $makesystem

ret=$?
if [ $ret -eq 3 ]; then
	#Handle failure
	cd ..
else
	cd ..
	make solution
fi
}

run

