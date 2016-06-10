#
# Copyright (c) 2015 Tommi Roenty   http://www.tommironty.fi/
# Licensed under The GNU Lesser General Public License, version 2.1:
#     http://opensource.org/licenses/LGPL-2.1
#
source ../make.cnf
git clone https://bitbucket.org/James_Snook/simulatorfolders

curl -O https://github.com/kallewoof/plget/archive/master.zip
curl -O https://gist.github.com/kallewoof/de4899aabde564f62687/archive/2aa1d27b5180c21695c17db977ac69ec7705c6a2.zip

git clone https://github.com/CocoaPods/CocoaPods.git
git clone https://github.com/CocoaPods/Xcodeproj.git

curl -O https://gist.github.com/kizzx2/1534572/archive/8298cf9b5a9c5631f40c77003c21dd30097f06dc.zip -o 'Use premake4 to generate iOS .xcodeproj files.zip'

git clone https://github.com/ghughes/fruitstrap.git
git clone https://github.com/phonegap/ios-deploy.git
git clone git://github.com/phonegap/ios-sim.git

git clone https://github.com/James-Jones/HLSLCrossCompiler.git

curl -O https://github.com/Microsoft/DirectXMesh/archive/apr2016.zip
curl -O https://github.com/Microsoft/DirectXMesh/releases/download/apr2016/meshconvert.exe

git clone https://github.com/mawww/hlsl2glslfork.git


curl -O http://www.ijg.org/files/jpegsrc.v9b.tar.gz
git clone git://git.code.sf.net/p/libpng/code libpng-code
curl -O https://sourceforge.net/projects/tgalib/files/libtga/1.0.1/libtga-1.0.1.tar.gz/download
curl -O http://zlib.net/zlib-1.2.8.tar.gz


git clone git://source.winehq.org/git/wine.git
curl -O https://developer.apple.com/library/mac/samplecode/GLEssentials/GLEssentials.zip
git clone https://github.com/soywiz/sdl-static.git
git clone https://github.com/MicBosi/visualizationlibrary.git
git clone https://github.com/corporateshark/Android-NDK-Game-Development-Cookbook.git
# does not work: curl -O http://nehe.googlecode.com/files/Lesson01.zip
curl -O http://insanitydesign.com/wp/wp-content/uploads/lesson08.zip
curl -O http://download.nvidia.com/developer/Third_Party/Nature/NatureScene.zip
curl -O http://download-developer.amd.com/GPU/executables/AshliViewerSetup-1.7.0.msi
curl -O http://software.intel.com/file/15625 -o GrassISN__drop2.zip
curl -O http://software.intel.com/file/26305/ -o VolumetricCloud_Code.zip
curl -O http://www2.ati.com/misc/sdk/ati_sdk(mar2006).exe
curl -O http://developer.download.nvidia.com/SDK/9.5/NVIDIA_SDK_9.52.0324.0405.exe
curl -O http://www.microsoft.com/en-us/download/details.aspx?id=6812 -o DXSDK_June_2010.exe
git clone https://github.com/Piepenguin1995/citybuilder.git
curl -O http://http.download.nvidia.com/developer/GPU_Gems_2/CD/Content/16.zip
curl -O http://http.download.nvidia.com/developer/GPU_Gems_2/CD/Content/18.zip
curl -O http://kcat.strangesoft.net/d3dgl-src.tar.bz2
curl -O https://code.msdn.microsoft.com/vstudio/Port-of-Github-Pacman-e5a82615/file/136036/2/Android%20OpenGL%20ES%202.0%20Pac-Man%20Game%20Sample.zip -o Android\ OpenGL\ ES\ 2.0\ Pac-Man\ Game\ Sample.zip
git clone git://github.com/gametutorials/tutorials.git
curl -O http://www.gametutorial.net/games-showcase/Moon-lander/Moon-lander_source-code.zip
curl -O http://www.gametutorial.net/games-showcase/Shoot-the-duck/Shoot-the-duck_source-code.zip
curl -O http://www.gametutorial.net/games-showcase/Helicopter-battle/Helicopter-battle_source-code.zip
curl -O http://psellos.com/pub/ocamlxsim/runsim;mv ./runsim ./runsim.sh;chmod a+x ./runsim.sh
git clone https://github.com/randomdrake/nasa-apod-desktop.git
curl -O http://downloads.sourceforge.net/project/libusb/libusb-1.0/libusb-1.0.20/libusb-1.0.20.tar.bz2
curl -O https://www.kernel.org/pub/linux/utils/usb/usbutils/usbutils-008.tar.gz
git clone https://chromium.googlesource.com/angle/angle
git clone git://github.com/aras-p/hlsl2glslfork
svn co http://assimp.svn.sourceforge.net/svnroot/assimp/trunk/include assimp/include
svn co http://assimp.svn.sourceforge.net/svnroot/assimp/trunk/code assimp/code 
svn co http://assimp.svn.sourceforge.net/svnroot/assimp/trunk/tools/assimp_view/ assimp/tools/assimp_view
curl -O http://www.humus.name/3D/Framework3.zip
git clone git://giflib.git.sourceforge.net/gitroot/gif lib/giflib
curl -O http://www.indielib.com/sdk/MudgeFont_LatestVersion.rar
curl -O http://www.indielib.com/sdk/old_IndieLib_LatestVersion_SDK.rar
git clone https://github.com/DarthMike/indielib-crossplatform.git
svn co http://indielib-crossplatform.googlecode.com/svn/trunk/IndieLib/common/src/ IndieLib/common/src
svn co http://indielib-crossplatform.googlecode.com/svn/trunk/IndieLib/common/include/ IndieLib/common/include
svn co https://indielib-crossplatform.googlecode.com/svn/trunk/IndieLib/tutorials/ IndieLib/tutorials/
svn co https://jpeg.svn.sourceforge.net/svnroot/jpeg/ljpeg-6b/ jpeg
git clone git://libpng.git.sourceforge.net/gitroot/libpng/libpng
curl -O ftp://ftp.remotesensing.org/pub/libtiff/tiff-4.0.3.zip
curl -O http://sourceforge.net/projects/libpng/files/zlib/1.2.6/zlib126.zip
curl -O http://www.lua.org/ftp/lua-5.3.2.tar.gz
git clone git://repo.or.cz/openal-soft.git
git clone https://github.com/WARP-LAB/FreeImage-LuxRender.git
curl -O http://downloads.sourceforge.net/freeimage/FreeImage3170.zip
svn co https://openil.svn.sourceforge.net/svnroot/openil/trunk/DevIL/include/IL/ openil/include/IL/
svn co https://openil.svn.sourceforge.net/svnroot/openil/trunk/DevIL/src-IL/src/ openil/src-IL/src/
svn co https://openil.svn.sourceforge.net/svnroot/openil/trunk/DevIL/src-ILU/src/ openil/src-ILU/src/
cvs -d:pserver:anonymous@opentreelib.cvs.sourceforge.net:/cvsroot/opentreelib login
cvs -z3 -d:pserver:anonymous@opentreelib.cvs.sourceforge.net:/cvsroot/opentreelib co otl/include
cvs -z3 -d:pserver:anonymous@opentreelib.cvs.sourceforge.net:/cvsroot/opentreelib co otl/src
cvs -z3 -d:pserver:anonymous@opentreelib.cvs.sourceforge.net:/cvsroot/opentreelib co otl/examples
hg clone -u SDL-1.2 http://hg.libsdl.org/SDL SDL-1.2
curl -O $sdlurl/release/SDL2-$sdl2version.tar.gz
curl -O $sdlurl/release/SDL2_image-$sdlimage2version.tar.gz
git clone https://github.com/grimfang4/sfxr.git
git clone https://github.com/EsotericSoftware/spine-runtimes.git
svn checkout http://tinypy.googlecode.com/svn/trunk/ tinypy-read-only
git clone git://git.code.sf.net/p/tinyxml/git tinyxml-git
mkdir whirlgif
cd whirlgif
curl -O ftp://sohoftp.nascom.nasa.gov/solarsoft/offline/sources/whirlgif/CHANGES
curl -O ftp://sohoftp.nascom.nasa.gov/solarsoft/offline/sources/whirlgif/Makefile
curl -O ftp://sohoftp.nascom.nasa.gov/solarsoft/offline/sources/whirlgif/anim.gif
curl -O ftp://sohoftp.nascom.nasa.gov/solarsoft/offline/sources/whirlgif/anim.html
curl -O ftp://sohoftp.nascom.nasa.gov/solarsoft/offline/sources/whirlgif/face0.gif
curl -O ftp://sohoftp.nascom.nasa.gov/solarsoft/offline/sources/whirlgif/face1.gif
curl -O ftp://sohoftp.nascom.nasa.gov/solarsoft/offline/sources/whirlgif/face2.gif
curl -O ftp://sohoftp.nascom.nasa.gov/solarsoft/offline/sources/whirlgif/face3.gif
curl -O ftp://sohoftp.nascom.nasa.gov/solarsoft/offline/sources/whirlgif/face4.gif
curl -O ftp://sohoftp.nascom.nasa.gov/solarsoft/offline/sources/whirlgif/whirlgif
curl -O ftp://sohoftp.nascom.nasa.gov/solarsoft/offline/sources/whirlgif/whirlgif.1
curl -O ftp://sohoftp.nascom.nasa.gov/solarsoft/offline/sources/whirlgif/whirlgif.c
curl -O ftp://sohoftp.nascom.nasa.gov/solarsoft/offline/sources/whirlgif/whirlgif.doc
curl -O ftp://sohoftp.nascom.nasa.gov/solarsoft/offline/sources/whirlgif/whirlgif.h
cd ..
#curl -O $androidurl/ndk/$NDK-windows.zip
#unzip $NDK-windows.zip
#curl -O $androidurl/ndk/$NDK-darwin-x86.tar.bz2
curl -O $androidurl/ndk/$NDK-linux-x86.tar.bz2
bunzip2 $NDK-linux-x86.tar.bz2
tar -xvf $NDK-linux-x86.tar
#curl -O $androidurl/adt/adt-bundle-windows-x86.zip
#unzip adt-bundle-windows-x86.zip
#curl -O $androidurl/adt/adt-bundle-windows-x86_64.zip
#unzip adt-bundle-windows-x86_64.zip
#curl -O $androidurl/adt/adt-bundle-mac-x86_64.zip
#unzip adt-bundle-mac-x86_64.zip
curl -O $androidurl/adt/adt-bundle-linux-x86.zip
unzip adt-bundle-linux-x86.zip
rm -f hlsl2glslfork/*
rm -f giflib/*
rm -f SDL/*
rm -f SDL-1.2/*
rm -f hlsl2glslfork/*.*
rm -f giflib/*.*
rm -f SDL/*.*
rm -f SDL-1.2/*.*
rm -fr hlsl2glslfork/hlslang.xcodeproj
rm -fr hlsl2glslfork/tests
rm -fr hlsl2glslfork/tools
rm -fr giflib/pic
rm -fr giflib/doc
rm -fr giflib/tests
rm -fr giflib/windows
rm -fr jpeg/*.jpg
rm -fr jpeg/*.bmp
rm -fr jpeg/*.ppm
rm -fr jpeg/*.doc
rm -fr whirlgif/*.doc
rm -fr whirlgif/*.gif
rm -fr whirlgif/whirlgif
rm -fr libpng/*.jpg
rm -fr libpng/*.png
rm -fr libpng/arm
rm -fr libpng/scripts
rm -fr libpng/contrib
rm -fr libpng/projects
rm -fr SDL-1.2/VisualC
rm -fr SDL-1.2/VisualCE
rm -fr SDL-1.2/docs
rm -fr SDL-1.2/build-scripts
rm -fr SDL-1.2/acinclude
rm -fr SDL-1.2/Xcode
rm -fr SDL-1.2/*.zip
rm -fr SDL/VisualC
rm -fr SDL/build-scripts
rm -fr SDL/acinclude
rm -fr SDL/Xcode
rm -fr SDL/Xcode-iOS
rm -fr SDL/android-project
rm -fr SDL/*.zip
rm -fr SDL/test
rm -fr SDL-1.2/test
