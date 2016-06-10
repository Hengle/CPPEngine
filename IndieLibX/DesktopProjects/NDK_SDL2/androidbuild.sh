#!/bin/bash
source ../../make.cnf

assets()
{
	$ADB shell rm -fr /mnt/sdcard/IndieLib_resources
 	$ADB shell mkdir /mnt/sdcard/IndieLib_resources
 	mkdir -p $BUILDPATH/assets/IndieLib_resources
 	#cp -fr $CURDIR/../../../../IndieLib_resources $BUILDPATH/assets/.

	arr=("IndieLib-Animation-Collision" "IndieLib-Blend" "IndieLib-Fading" "IndieLib-FloatingText" "IndieLib-ParallaxScroll" "IndieLib-TankEnemies" "IndieLib-TankEnemiesParallax" "IndieLib-TankFollowsMouseFires" "IndieLib-Transitions" "RadeonTerrainDemo" "NatureScene" "testspine2" "ParticleVS")

	if [[ " ${arr[@]} " =~ " ${mainphoneproject} " ]]; then
		cp -fr $CURDIR/../../../../IndieLib_resources/$mainphoneproject $BUILDPATH/assets/IndieLib_resources/.
		$ADB push $CURDIR/../../../../IndieLib_resources/$mainphoneproject /mnt/sdcard/IndieLib_resources/.
	fi

	arr=("a_08_Collisions" "a_16_IND_Timer" "a_02_IND_Surface" "b_01_IND_Surface_Grids" "a_03_IND_Image" "b_02_Blitting_2d_Directly" "a_04_IND_Animation" "a_11_Animated_Tile_Scrolling" "a_05_IND_Font" "a_13_2d_Camera" "b_04_Several_ViewPorts" "a_06_Primitives" "c_01_Alien_BenchMark" "a_07_IND_Input" "a_15_Parallax_Scrolling" "Texture" "Blending")

	if [[ " ${arr[@]} " =~ " ${mainphoneproject} " ]]; then
		cp -fr $CURDIR/../../../../IndieLib_resources/$mainphoneproject $BUILDPATH/assets/IndieLib_resources/.
		$ADB push $CURDIR/../../../../IndieLib_resources/$mainphoneproject /mnt/sdcard/IndieLib_resources/.
	fi

	arr=("3DAnimation" "b_03_Blitting_3d_Directly" "a_09_IND_3dMesh")
	if [[ " ${arr[@]} " =~ " ${mainphoneproject} " ]]; then
		cp -fr $CURDIR/../../../../IndieLib_resources/skeleton $BUILDPATH/assets/IndieLib_resources/.
		$ADB push $CURDIR/../../../../IndieLib_resources/skeleton /mnt/sdcard/IndieLib_resources/skeleton
	fi

	arr=("a_10_IND_Light" "a_14_3d_Camera")
	if [[ " ${arr[@]} " =~ " ${mainphoneproject} " ]]; then
		cp -fr "$CURDIR/../../../../IndieLib_resources/trex dx" $BUILDPATH/assets/IndieLib_resources/.
		$ADB push "$CURDIR/../../../../IndieLib_resources/trex dx" "/mnt/sdcard/IndieLib_resources/trex dx"
	fi
	arr=("Test" "SimpleTexture")
	if [[ " ${arr[@]} " =~ " ${mainphoneproject} " ]]; then
		cp -f $CURDIR/../../../../IndieLib_resources/test.bmp $BUILDPATH/assets/IndieLib_resources/.
		$ADB push $CURDIR/../../../../IndieLib_resources/test.bmp /mnt/sdcard/IndieLib_resources/.
	fi
	if [ "$mainphoneproject" == "2DAnimation" ]; then
		cp -fr $CURDIR/../../../../IndieLib_resources/2DAnimation $BUILDPATH/assets/IndieLib_resources/.
		cp -fr $CURDIR/../../../../IndieLib_resources/IndieLib-Animation-Collision $BUILDPATH/assets/IndieLib_resources/.
		$ADB push $CURDIR/../../../../IndieLib_resources/2DAnimation /mnt/sdcard/IndieLib_resources/.
		$ADB push $CURDIR/../../../../IndieLib_resources/IndieLib-Animation-Collision /mnt/sdcard/IndieLib_resources/.
	fi

 	if [ "X" == "Y" ]; then
 	#chmod -R a+r $BUILDPATH/assets
	find $BUILDPATH/assets -type d -exec chmod a+x {} \;
	find $BUILDPATH/assets -type d -exec chmod a+r {} \;
	find $BUILDPATH/assets -type f -exec chmod a+r {} \;
	find $BUILDPATH/assets -type d -exec chmod a+w {} \;
	find $BUILDPATH/assets -type f -exec chmod a+w {} \;
	fi
}

NDK1()
{
#echo "mainphoneproject2:$mainphoneproject"
#exit 0

ln -s $CURDIR/../../../../IndieLibX_Libraries/AllLibs_NDK $BUILDPATH/jni

ln -s $CURDIR/../../../Samples $BUILDPATH/jni
ln -s $CURDIR/../../../Dependencies $BUILDPATH/jni
ln -s $CURDIR/../../../../$NDK $BUILDPATH/jni

mkdir -p $BUILDPATH/jni/Libraries/SDL2-$sdl2version
ln -s $CURDIR/../../SDL2-$sdl2version/include $BUILDPATH/jni/Libraries/SDL2-$sdl2version
ln -s $CURDIR/../../STX $BUILDPATH/jni/Libraries
}

NDK()
{
ln -s $CURDIR/../../../../IndieLibX_Libraries/AllLibs_NDK $BUILDPATH/jni

ln -s $CURDIR/../../../Samples $BUILDPATH/jni
ln -s $CURDIR/../../../Dependencies $BUILDPATH/jni
ln -s $CURDIR/../../../../$NDK $BUILDPATH/jni

#ln -s $CURDIR/../.. $BUILDPATH/jni
mkdir -p $BUILDPATH/jni/Libraries
ln -s $CURDIR/../../jpeg-8c $BUILDPATH/jni/Libraries
ln -s $CURDIR/../../otl $BUILDPATH/jni/Libraries
ln -s $CURDIR/../../hlsl2glslfork $BUILDPATH/jni/Libraries
ln -s $CURDIR/../../libpng-1.5.0 $BUILDPATH/jni/Libraries
ln -s $CURDIR/../../assimp-3.2 $BUILDPATH/jni/Libraries

mkdir -p $BUILDPATH/jni/Libraries/SDL2-$sdl2version
ln -s $CURDIR/../../SDL2-$sdl2version/include $BUILDPATH/jni/Libraries/SDL2-$sdl2version

# rm -fr $BUILDPATH/jni/SDL
# /home/tronty/IndieLibX/Libraries/SDL2-2.0.4/build/com.Test.SimpleTexture/jni/Libraries/SDL2-2.0.4
# /home/tronty/IndieLibX/Libraries/SDL2-2.0.4/build/com.Test.SimpleTexture/jni/SDL

#ln -s $CURDIR/../../SDL2-$sdl2version/src $BUILDPATH/jni/Libraries/SDL2-$sdl2version
#find $BUILDPATH/jni/Libraries/SDL2-$sdl2version/src -name "*.mk" -type f -delete

ln -s $CURDIR/../../BoostWorkaround $BUILDPATH/jni/Libraries
ln -s $CURDIR/../../libzip $BUILDPATH/jni/Libraries
ln -s $CURDIR/../../spine-2.3 $BUILDPATH/jni/Libraries
ln -s $CURDIR/../../lua-5.3.2 $BUILDPATH/jni/Libraries
ln -s $CURDIR/../../STX $BUILDPATH/jni/Libraries
ln -s $CURDIR/../../Effects11 $BUILDPATH/jni/Libraries
ln -s $CURDIR/../../MeshRenderer2 $BUILDPATH/jni/Libraries
ln -s $CURDIR/../../Framework3 $BUILDPATH/jni/Libraries
ln -s $CURDIR/../../MVC $BUILDPATH/jni/Libraries
ln -s $CURDIR/../../tinyxml $BUILDPATH/jni/Libraries
ln -s $CURDIR/../../FW3 $BUILDPATH/jni/Libraries
ln -s $CURDIR/../../NSGUI $BUILDPATH/jni/Libraries
ln -s $CURDIR/../../openal-soft-1.15.1 $BUILDPATH/jni/Libraries
ln -s $CURDIR/../../zlib-1.2.5 $BUILDPATH/jni/Libraries
ln -s $CURDIR/../../IndieLib $BUILDPATH/jni/Libraries
ln -s $CURDIR/../../openil-1.7.8 $BUILDPATH/jni/Libraries
ln -s $CURDIR/../../FreeImage3170 $BUILDPATH/jni/Libraries
ln -s $CURDIR/../../Image3 $BUILDPATH/jni/Libraries
}

#echo "\$0=$0"
#echo "\$1=$1"
#echo "\$2=$2"
#echo "\$3=$3"
#exit 0

SOURCES=()
MKSOURCES=""
CURDIR0=`pwd -P`
CURDIR="$CURDIR0/../../Libraries/SDL2-$sdl2version/build-scripts"
cd "$CURDIR"

# Fetch sources
if [[ $# -ge 2 ]]; then
    for src in ${@:2}
    do
        SOURCES+=($src)
        MKSOURCES="$MKSOURCES $(basename $src)"
    done
else
    if [ -n "$1" ]; then
        while read src
        do
            SOURCES+=($src)
            MKSOURCES="$MKSOURCES $(basename $src)"
        done
    fi
fi

if [ -z "$1" ] || [ -z "$SOURCES" ]; then
    echo "Usage: androidbuild.sh com.yourcompany.yourapp < sources.list"
    echo "Usage: androidbuild.sh com.yourcompany.yourapp source1.c source2.c ...sourceN.c"
    echo "To copy SDL source instead of symlinking: COPYSOURCE=1 androidbuild.sh ... "
    echo "You can pass additional arguments to ndk-build with the NDKARGS variable: NDKARGS=\"-s\" androidbuild.sh ..."
    exit 1
fi

#SDLPATH="$( cd "$(dirname "$0")/.." ; pwd -P )"
SDLPATH="$CURDIR0/../../Libraries/SDL2-$sdl2version"

cp $CURDIR0/android_fopen.h $SDLPATH/include
cp $CURDIR0/android_fopen.c $SDLPATH/src/core/android
cp $CURDIR0/Android.mk $SDLPATH
cp $CURDIR0/Application.mk $SDLPATH/android-project/jni
cp $CURDIR0/SDLActivity.java $SDLPATH/android-project/src/org/libsdl/app
#cp $CURDIR0/SDLmainService.java $SDLPATH/android-project/src/org/libsdl/app
cp $CURDIR0/SDL_android.c $SDLPATH/src/core/android
cp $CURDIR0/SDL_android_main.c $SDLPATH/src/main/android
cp $CURDIR0/AndroidManifest.xml $SDLPATH/android-project
#cp $CURDIR0/ForegroundService.java $SDLPATH/android-project/src/org/libsdl/app

NDKBUILD=`which ndk-build`
if [ -z "$NDKBUILD" ];then
    echo "Could not find the ndk-build utility, install Android's NDK and add it to the path"
    exit 1
fi

ANDROID=`which android`
if [ -z "$ANDROID" ];then
    echo "Could not find the android utility, install Android's SDK and add it to the path"
    exit 1
fi

ANT=`which ant`
if [ -z "$ANT" ];then
    echo "Could not find the ant utility, install ant and add it to the path"
    exit 1
fi

ADB=`which adb`
if [ -z "$ADB" ];then
    echo "Could not find the adb utility, install Android's SDK and add it to the path"
    exit 1
fi

NCPUS="1"
case "$OSTYPE" in
    darwin*)
        NCPU=`sysctl -n hw.ncpu`
        ;; 
    linux*)
        if [ -n `which nproc` ]; then
            NCPUS=`nproc`
        fi  
        ;;
  *);;
esac

APP="$1"
APPARR=(${APP//./ })
BUILDPATH="$SDLPATH/build/$APP"

export NDK_PROJECT_PATH="$BUILDPATH"

# Start Building

#rm -rf $BUILDPATH
#mkdir -p $BUILDPATH

cp -r $SDLPATH/android-project/* $BUILDPATH

# Copy SDL sources
mkdir -p $BUILDPATH/jni/SDL
if [ -z "$COPYSOURCE" ]; then
    ln -s $SDLPATH/src $BUILDPATH/jni/SDL
    ln -s $SDLPATH/include $BUILDPATH/jni/SDL
else
    cp -r $SDLPATH/src $BUILDPATH/jni/SDL
    cp -r $SDLPATH/include $BUILDPATH/jni/SDL
fi

cp -r $SDLPATH/Android.mk $BUILDPATH/jni/SDL
sed -i.bu "s|YourSourceHere.c|$MKSOURCES|g" $BUILDPATH/jni/src/Android.mk
sed -i.bu "s|org\.libsdl\.app|$APP|g" $BUILDPATH/AndroidManifest.xml

# Copy user sources
for src in "${SOURCES[@]}"
do
    cp $src $BUILDPATH/jni/src
done

# Create an inherited Activity
cd $BUILDPATH/src
for folder in "${APPARR[@]}"
do
    mkdir -p $folder
    cd $folder
done

ACTIVITY="${folder}Activity"
sed -i.bu "s|SDLActivity|$ACTIVITY|g" $BUILDPATH/AndroidManifest.xml
sed -i.bu "s|SDLActivity|$APP|g" $BUILDPATH/build.xml

# Fill in a default Activity
echo "package $APP;" >  "$ACTIVITY.java"
echo "import org.libsdl.app.SDLActivity;" >> "$ACTIVITY.java"
echo "public class $ACTIVITY extends SDLActivity {}" >> "$ACTIVITY.java"

if [ -z "$3" ]; then
	NDK;
else
	NDK1;
fi
assets;

# Update project and build
cd $BUILDPATH
$ANDROID update project --path . --target "android-19" --subprojects

exit 0

$NDKBUILD V=1 -j $NCPUS $NDKARGS
$ANT debug

cd $CURDIR

APK="$BUILDPATH/bin/$APP-debug.apk"

if [ -f "$APK" ]; then
    echo "Your APK is ready at $APK"
    echo "To install to your device: "
    echo "cd  $BUILDPATH"
    echo "ant debug install"
    exit 0
fi

echo "There was an error building the APK"
exit 1
