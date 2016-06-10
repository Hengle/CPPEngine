#!/bin/bash
source ../../make.cnf

if [ -a ../../count.txt ]; then
	num=$(<../../count.txt)
	num2=$((num + 1)) 
	echo "$num2" > ../../count.txt
else
	echo "1" > ../../count.txt
fi
#echo "num:$num"

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

mkdir -p $BUILDPATH/jni/Libraries
ln -s $CURDIR/../../jpeg-8c $BUILDPATH/jni/Libraries
ln -s $CURDIR/../../otl $BUILDPATH/jni/Libraries
ln -s $CURDIR/../../hlsl2glslfork $BUILDPATH/jni/Libraries
ln -s $CURDIR/../../libpng-1.5.0 $BUILDPATH/jni/Libraries
ln -s $CURDIR/../../assimp-3.2 $BUILDPATH/jni/Libraries

ln -s $CURDIR/../../BoostWorkaround $BUILDPATH/jni/Libraries
ln -s $CURDIR/../../libzip $BUILDPATH/jni/Libraries
ln -s $CURDIR/../../spine-2.3 $BUILDPATH/jni/Libraries
ln -s $CURDIR/../../lua-5.3.2 $BUILDPATH/jni/Libraries
ln -s $CURDIR/../../STX $BUILDPATH/jni/Libraries
ln -s $CURDIR/../../Effects11 $BUILDPATH/jni/Libraries
ln -s $CURDIR/../../MeshRenderer2 $BUILDPATH/jni/Libraries
ln -s $CURDIR/../../Framework3 $BUILDPATH/jni/Libraries
#ln -s $CURDIR/../../MVC $BUILDPATH/jni/Libraries
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

CURDIR0=`pwd -P`
CURDIR="$CURDIR0/../../Libraries/MVC/NDK"

if [ -z "$1" ]; then
    echo "Usage: androidbuild.sh com.yourcompany.yourapp < sources.list"
    echo "Usage: androidbuild.sh com.yourcompany.yourapp source1.c source2.c ...sourceN.c"
    echo "To copy SDL source instead of symlinking: COPYSOURCE=1 androidbuild.sh ... "
    echo "You can pass additional arguments to ndk-build with the NDKARGS variable: NDKARGS=\"-s\" androidbuild.sh ..."
    exit 1
fi

#APPPATH="$( cd "$(dirname "$0")/.." ; pwd -P )"
APPPATH="$CURDIR0/../../Libraries/MVC/NDK"

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
BUILDPATH="$APPPATH"

export NDK_PROJECT_PATH="$BUILDPATH"

# Start Building

# APPPATH="$CURDIR0/../../Libraries/MVC/NDK"

CURDIR1=`pwd -P`
echo "CURDIR1=$CURDIR1"

# cp: tiedoston ”/home/tronty/IndieLibX/DesktopProjects/NDK_gl_code2/Android.mk” tilaa ei voi lukea: Tiedostoa tai hakemistoa ei ole

#rm -fr $APPPATH
mkdir -p $APPPATH/jni/MVC
# cp -fr ./hello-gl2/*.* $APPPATH

cp $CURDIR0/AndroidManifest.xml $APPPATH
cp $CURDIR0/Android.mk $APPPATH/jni
cp $CURDIR0/Application.mk $APPPATH/jni

cp $CURDIR0/android_fopen.h $APPPATH/jni/MVC

cp $CURDIR0/hello-gl2/default.properties $APPPATH
cp -fr $CURDIR0/hello-gl2/res $APPPATH  
cp -fr $CURDIR0/hello-gl2/src $APPPATH

cd "$CURDIR"

#if [ -z "$3" ]; then
#	NDK;
#else
#	NDK1;
#fi
NDK1;
assets;

# ??? sed -i.bu "s|org\.libsdl\.app|$APP|g" $BUILDPATH/AndroidManifest.xml

rm -fr $APPPATH/jni/SDL2

cat > "$APPPATH/jni/Android.mk" <<HERE1
include \$(call all-subdir-makefiles)
HERE1

#cat > "$APPPATH/res/values/strings.xml" <<HERE2
#<?xml version="1.0" encoding="utf-8"?>
#<resources>
#    <string name="gl2jni_activity">GL2JNI_${mainphoneproject}_$num</string>
#</resources>
#HERE2

sed -i.bu "s|>GL2JNI<|>GL2JNI_${mainphoneproject}_$num<|g" $APPPATH/res/values/strings.xml
rm -f $APPPATH/res/values/strings.xml.bu

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
