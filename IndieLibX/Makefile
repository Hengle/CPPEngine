#
# Copyright (c) 2015 Tommi Roenty   http://www.tommironty.fi/
# Licensed under The GNU Lesser General Public License, version 2.1:
#     http://opensource.org/licenses/LGPL-2.1
#
#export shell = /bin/sh
export shell = /bin/bash
export python=python

include make.cnf

OS="Windows"
STR = $(shell uname -a)
ifneq (, $(findstring Ubuntu, $(STR)))
	OS="Ubuntu"
else ifneq (, $(findstring Darwin, $(STR)))
	OS="Darwin"
else ifneq (, $(findstring Debian, $(STR)))
	OS="Debian"
endif

export OS

Bit32p64=$(shell uname -m)
export Bit32p64
	
python_version_full := $(wordlist 2,4,$(subst ., ,$(shell python --version 2>&1)))
python_version_major := $(word 1,${python_version_full})
python_version_minor := $(word 2,${python_version_full})
python_version_patch := $(word 3,${python_version_full})

my_cmd.python.2 := python2 some_script.py2
my_cmd.python.3 := python3 some_script.py3
my_cmd := ${my_cmd.python.${python_version_major}}

# sdl2-config --static-libs

ifeq ("$(OS)", "Darwin")
	MACOSX_VERSION=$(xcodebuild -showsdks | grep macosx | sort | tail -n 1 | awk '{print substr($NF, 7)}')
	iOS_VERSION=$(xcodebuild -showsdks | grep iphoneos | sort | tail -n 1 | awk '{print substr($NF, 9)}')
	XCODE=$(xcodebuild -version | grep Xcode | cut -d " " -f2)
	export MACOSX_VERSION
	export iOS_VERSION
	export XCODE
endif

help:
	@echo ""
	@echo "Instead of this Makefile use ./make.sh to build solutions defined in ./DesktopProjects/Projects.xml"
	@echo ""
	@echo "Usage:	./make.sh		->	Removes temporary files."
	@echo "Usage:	./make.sh [< "${solutions}" >] [< "${platforms}" >]	->	Builds an solution."
	@echo ""
	@echo "Usage:	make RadeonTerrainDemo		->	Runs original RadeonTerrainDemo.exe with wine"
	@echo "Usage:	make NatureScene		->	Runs original NatureScene.exe with wine"
	@echo "Usage:	make Particles			->	Runs original Particles.exe with wine"
	@echo "Usage:	make ParticleVS			->	Runs original ParticleVS.exe with wine"

	@echo ""

rmdirs:
	rm -fr ./Libraries/MVC/NDK/build
	mkdir ./Libraries/MVC/NDK/build
	rm -fr ./Libraries/SDL2-${sdl2version}/build
	mkdir  ./Libraries/SDL2-${sdl2version}/build
	rm -fr ./DesktopProjects/projectsX
	mkdir ./DesktopProjects/projectsX
	rm -fr ./IndieLibX.xcworkspace

print:
	./python.sh
	#echo "3 Building to iOS${iOS_VERSION}"
	#echo "3 Building to MacOS${MACOSX_VERSION}"

copy: rmdirs
	#mkdir ./DesktopProjects/projectsX/Resources
	#cp -fr ./DesktopProjects/GLEssentials/Resources/iOS ./DesktopProjects/projectsX/Resources/.
	#cp -fr ./DesktopProjects/GLEssentials/Resources/OSX ./DesktopProjects/projectsX/Resources/.
	#cp -f ./DesktopProjects/GLEssentials/Default-568h@2x.png  ./DesktopProjects/projectsX/.      
	#cp -f ./DesktopProjects/GLEssentials/OSXGLEssentials-Info.plist ./DesktopProjects/projectsX/.
	#cp -f ./DesktopProjects/GLEssentials/iOSGLEssentials-Info.plist ./DesktopProjects/projectsX/.
	#cp -f ./DesktopProjects/GLEssentials/OSXGLEssentials_Prefix.pch ./DesktopProjects/projectsX/.
	#cp -f ./DesktopProjects/GLEssentials/iOSGLEssentials_Prefix.pch ./DesktopProjects/projectsX/.
	cd ./DesktopProjects;python Projects.py --solution $(esolution) --buildtool $(emakesystem)

iOScopy:
	#cp -f ./DesktopProjects/GLEssentials/Resources/iOS/MainWindow.xib ./bin/.

OSXcopy:
	#cp -f ./DesktopProjects/GLEssentials/Resources/OSX/MainMenu.xib ./bin/.

rmtmps1:
	if [ -e "./DesktopProjects/projectsX/Makefile" ]; then \
	make -C ./DesktopProjects/projectsX clean; \
	fi; \
	if [ -e "./DesktopProjects/projectsX/Makefile.iOS" ]; then \
	make -f Makefile.iOS -C ./DesktopProjects/projectsX clean; \
	fi;  \
	if [ -e "./DesktopProjects/projectsX/Makefile.mw" ]; then \
	make -f Makefile.mw -C ./DesktopProjects/projectsX clean; \
	fi; 

rmtmps: rmtmps1
	rm -fr ./DesktopProjects/projectsX
	mkdir ./DesktopProjects/projectsX
	rm -fr ./Libraries/MVC/NDK/build
	mkdir -p ./Libraries/MVC/NDK/build
	rm -fr ./Libraries/SDL2-${sdl2version}/build
	mkdir  ./Libraries/SDL2-${sdl2version}/build
	#rm -fr ./DesktopProjects/projectsX
	#mkdir ./DesktopProjects/projectsX
	#rm -fr ./IndieLibX.xcworkspace
	rm -f ./scripting/*.pyc ./scripting/*.o ./scripting/IndieLib.so ./scripting/*.*~ ./scripting/*~ ./scripting/IndieLib_wrap.cpp ./scripting/IndieLib_wrap.c ./scripting/IndieLib_wrap.cxx ./NDK.log ./NDKbuild.log ./scripting/IndieLib.py ./iOSbuild.log ./build.log
	find . -name "*.obj" -type f -delete
	find . -name "*.o" -type f -delete
	find . -name "*.*~" -type f -delete
	find . -name "*~" -type f -delete
	find . -name "*.log" -type f -delete
	find ./DesktopProjects -name "*.pyc" -type f -delete
	find ./scripting -name "*.pyc" -type f -delete
	find ./mk -name "*.pyc" -type f -delete

rmlibs:
	rm -fr ./lib/*.*

rmapps:
	rm -fr ./bin/*.*

cleanNDK:
	rm -fr ./Libraries/MVC/NDK

clean: cleanNDK rmtmps rmlibs rmapps

#adb shell df
emulator:
	#$ANDROID_SDK/tools/android list avds
	#$ANDROID_SDK/tools/android list targets
	#$ANDROID_SDK/tools/android create avd -n avd19 -t 1
	#rm -f NDK.log
	#$(ANDROID_SDK)/platform-tools/adb logcat -c
	#$(ANDROID_SDK)/platform-tools/adb logcat -v threadtime >NDK.log &
	#
	#cat > "$(HOME)/adt-bundle/sdk/platforms/android-19/skins/WVGA800/hardware.ini" <<HERE \
	## skin-specific hardware values \n\
	#hw.lcd.density=240 \n\
	##vm.heapSize=48 \n\
	##hw.ramSize=512 \n\
	#vm.heapSize=480 \n\
	#hw.ramSize=5120 \n\
	#HERE
	$(ANDROID_SDK)/tools/emulator -force-32bit -memory 128 -cache-size 256 -partition-size 256 -avd avd19 -gpu on &

solution: print copy
ifneq ("$(emakesystem)", "NDK")
	##cd ./DesktopProjects/build-scripts;./androidbuild0.sh $(package) $(sources)    
	#cd ./DesktopProjects/build-scripts;./androidbuild.sh -m $(mainphoneproject) -b $(package) -a True -o True -f $(NDKplatform)
	#else
	cd ./DesktopProjects;python Projects.py --buildtool $(emakesystem) --solution None --mainphoneproject $(mainphoneproject) --package $(package) --apps True;cd ..; \
	if [ -e "./DesktopProjects/projectsX/Makefile" ]; then \
	make -C ./DesktopProjects/projectsX all 2>&1 | tee build.log; \
	fi; \
	if [ -e "./DesktopProjects/projectsX/Makefile.iOS" ]; then \
	make -C ./DesktopProjects/projectsX -f Makefile.iOS all 2>&1 | tee iOSbuild.log; \
	fi; \
	if [ -e "./DesktopProjects/projectsX/Makefile.mw" ]; then \
	make -C ./DesktopProjects/projectsX -f Makefile.mw all 2>&1 | tee build.log; \
	fi; 
endif

ndkupdate:
	$(ANDROID_SDK)/tools/android update project --subprojects --name "avd19" --path . --target "android-$(NDKplatform)"

#$ANDROID_SDK/platform-tools/adb uninstall $(package)
update:
	$(ANDROID_SDK)/tools/android update project --subprojects --name "avd19" --path . --target "android-$(NDKplatform)"

create:
	$(ANDROID_SDK)/tools/android create project -k $(package) --path . --target "android-$(NDKplatform)" -a NativeActivity

android:
	#lsusb
	#cat 18d1 >$HOME/.android/adb_usb.ini
	#adb kill-server
	#adb start-server
	##ls 	/etc/udev/rules.d/51-android-rules
	##/etc/init.d/udev restart

test:
	#make test arch=i386
	echo "$(arch)"

installlibs:
	sudo cp $HOME/IndieLibX/lib/*.a /usr/local/lib/.

NDKInstalllibs:
	if [ "$NDKarch" == "arm" ]; then
		cp $HOME/IndieLibX/Libraries/MVC/NDK/build/$(mainphoneproject)/obj/local/armeabi/*.a $HOME/$NDK/platforms/android-$(NDKplatform)/arch-$(NDKarch)/usr/lib/.
	else
		cp $HOME/IndieLibX/Libraries/MVC/NDK/build/$(mainphoneproject)/obj/local/$(NDKarch)/*.a $HOME/$NDK/platforms/android-$(NDKplatform)/arch-$(NDKarch)/usr/lib/.
	fi

iOSInstalllibs:
	if [ "$iOSplatform" == "iPhoneSimulator" ]; then
		mkdir -p $HOME/lib/iPhoneSimulator
		cp -fr $HOME/IndieLibX/lib/*.framework $HOME/lib/iPhoneSimulator/.
	else
		mkdir -p $HOME/lib/iPhoneOS
		cp -fr $HOME/IndieLibX/lib/*.framework $HOME/lib/iPhoneOS/.
	fi

#find . -type f -name "*.h" -exec grep -l "ifndef SWIG" {} \;

#find . -type d -exec chmod a+x {} \;
#find . -type d -exec chmod a+r {} \;
#find . -type f -exec chmod a+r {} \;
#find . -name build.gradle
#grep -R "buildToolsVersion"
#find . -name build.gradle -exec grep "buildToolsVersion" '{}' \; -print

ndksrc:
	cd ~/IndieLibX/Libraries/SDL2-$(sdl2version) \
	echo $(notdir $(filter-out %/playmus.c %/playwave.c, $(wildcard ./*.c))) \
	cd ~
	#cd ~/IndieLibX/Libraries/SDL2_mixer-2.0.0 \
	#echo $(notdir $(filter-out %/playmus.c %/playwave.c, $(wildcard ./*.c))) \
	#cd ~

#Projects.py ProjectsNDK.py Projects.py pbxproj.py Projects.py ProjectsNMAKE.py ProjectsVC.py --apps True
testVC: print copy
	cd ./DesktopProjects;python Projects.py --buildtool $(emakesystem) --solution Test --mainphoneproject $(mainphoneproject) --package $(package) --apps True;python Projects.py --buildtool VC --solution None --mainphoneproject Test --package com.test.Test;cd ..
	
testNMAKE: print copy
	cd ./DesktopProjects;python Projects.py --buildtool $(emakesystem) --solution Test --mainphoneproject $(mainphoneproject) --package $(package) --apps True;python Projects.py --buildtool NMAKE --solution None --mainphoneproject Test --package com.test.Test;cd ..

all: help

Particles:
	if [ -e "/usr/bin/wine" ]; then \
	cd ./Samples/ParticleVS; \
	unzip -x ./Chap12.zip -d ./Particle; \
	cd ./Particle/Chap12/Particles; \
	wine ./Particles.exe; \
	cd ../../..; \
	rm -fr ./Particle; \
	fi;

ParticleVS:
	if [ -e "/usr/bin/wine" ]; then \
	cd ./Samples/ParticleVS; \
	unzip -x ./Chap12.zip -d ./ParticleVS; \
	cd ./ParticleVS/Chap12/ParticleVS; \
	wine ./ParticlesVS.exe; \
	cd ../../..; \
	rm -fr ./ParticleVS; \
	fi;

RadeonTerrainDemo:
	if [ -e "/usr/bin/wine" ]; then \
	cd ./Samples/RadeonTerrainDemo; \
	unzip -x ./RadeonTerrainDemo.zip -d ./RadeonTerrainDemo; \
	cd ./RadeonTerrainDemo/RadeonTerrainDemo;  \
	wine ./RadeonTerrainDemo.exe; \
	cd ../..; \
	rm -fr ./RadeonTerrainDemo; \
	fi;

NatureScene:
	if [ -e "/usr/bin/wine" ]; then \
	cd ./Samples/NatureScene; \
	unzip -x ./NatureScene.zip -d ./NatureScene; \
	cd ./NatureScene/NatureScene/Application\ Nature; \
	wine ./SceneGraph.exe; \
	cd ../../..; \
	rm -fr ./NatureScene; \
	fi;

otl_SDL:
	cd ./Libraries; \
	unzip -x ./otl_SDL.zip -d ./otl_SDL; \
	cd ..; \
	./make.sh; \
	./make.sh otl_SDL; \
	./run.sh; \
	./make.sh; \
	rm -fr ./Libraries/otl_SDL

files:
	grep -R '\"[a-z0-9_/\\]*\.[a-z0-9]*\"' ./Samples ./Libraries/IndieLib/IndieLib_SDK-1.03/tutorials/source/ ./Libraries/IndieLib/IndieLibSamples >files.txt

X:
	find ../IndieLib_resources -type d -exec chmod a+x {} \;
	find ../IndieLib_resources -type d -exec chmod a+r {} \;
	find ../IndieLib_resources -type f -exec chmod a+r {} \;
	find ../IndieLib_resources -type d -exec chmod a+w {} \;
	find ../IndieLib_resources -type f -exec chmod a+w {} \;

SDL2files:
	touch -t 201602261231.43 ./wotsit
	find ./Libraries/SDL2-2.0.4 -newer ./wotsit -print
	rm -f ./wotsit

NDK2LOG:
	rm -f NDK2.log
	$(ANDROID_SDK)/platform-tools/adb logcat -c
	$(ANDROID_SDK)/platform-tools/adb logcat -v threadtime >NDK2.log

LOG:
	if [ -e "/var/log/system.log" ]; then \
		cat /var/log/system.log  | grep SimpleTexture; \
	fi; \
	if [ -e "./NDK.log" ]; then \
		cat ./NDK.log  | grep SimpleTexture; \
	fi; \
	if [ -e "./NDK2.log" ]; then \
		cat ./NDK2.log  | grep SimpleTexture; \
	fi;

GLEssentials-Clean:
	rm -fr ./Samples/iOS/GLEssentials/build

GLEssentials-OSX:
	xcodebuild -project ./Samples/iOS/GLEssentials/GLEssentials.xcodeproj -target GLEssentials-OSX -configuration Debug
	./Samples/iOS/GLEssentials/build/Debug-iphonesimulator/GLEssentials.app/Contents/MacOS/GLEssentials

GLEssentials-iOS:
	xcodebuild -sdk iphonesimulator -project ./Samples/iOS/GLEssentials/GLEssentials.xcodeproj -target GLEssentials-iOS -configuration Debug
	ios-sim launch ./Samples/iOS/GLEssentials/build/Debug-iphonesimulator/GLEssentials.app --devicetypeid com.apple.CoreSimulator.SimDeviceType.iPad-2

