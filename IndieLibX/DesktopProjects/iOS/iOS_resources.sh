#!/bin/bash
source ../../../make.cnf

TOPDIR_="./Documents/IndieLib_resources"
if [ -n "$1" ]
then
	TOPDIR_="$1/Documents/IndieLib_resources"
fi

DIR=../../../..

mkdir -p "$TOPDIR_"

CURDIR=`pwd -P` 

arr=("IndieLib-Animation-Collision" "IndieLib-Blend" "IndieLib-Fading" "IndieLib-FloatingText" "IndieLib-ParallaxScroll" "IndieLib-TankEnemies" "IndieLib-TankEnemiesParallax" "IndieLib-TankFollowsMouseFires" "IndieLib-Transitions" "RadeonTerrainDemo" "NatureScene" "testspine2" "ParticleVS")

if [[ " ${arr[@]} " =~ " ${mainphoneproject} " ]]; then
	cp -fr "$CURDIR/$DIR/IndieLib_resources/$mainphoneproject" "$TOPDIR_/."
fi

arr=("a_08_Collisions" "a_16_IND_Timer" "a_02_IND_Surface" "b_01_IND_Surface_Grids" "a_03_IND_Image" "b_02_Blitting_2d_Directly" "a_04_IND_Animation" "a_11_Animated_Tile_Scrolling" "a_05_IND_Font" "a_13_2d_Camera" "b_04_Several_ViewPorts" "a_06_Primitives" "c_01_Alien_BenchMark" "a_07_IND_Input" "a_15_Parallax_Scrolling" "Texture" "Blending")

if [[ " ${arr[@]} " =~ " ${mainphoneproject} " ]]; then
	cp -fr "$CURDIR/$DIR/IndieLib_resources/$mainphoneproject" "$TOPDIR_/."
fi

arr=("3DAnimation" "b_03_Blitting_3d_Directly" "a_09_IND_3dMesh")
if [[ " ${arr[@]} " =~ " ${mainphoneproject} " ]]; then
	cp -fr "$CURDIR/$DIR/IndieLib_resources/skeleton" "$TOPDIR_/."
fi

arr=("a_10_IND_Light" "a_14_3d_Camera")
if [[ " ${arr[@]} " =~ " ${mainphoneproject} " ]]; then
	cp -fr "$CURDIR/$DIR/IndieLib_resources/trex dx" "$TOPDIR_/."
fi
arr=("Test" "SimpleTexture" "Lesson01")
if [[ " ${arr[@]} " =~ " ${mainphoneproject} " ]]; then
	cp -f "$CURDIR/$DIR/IndieLib_resources/test.bmp" "$TOPDIR_/."
fi

arr=("GLEssentialsiOS" "GLEssentialsOSX")
if [[ " ${arr[@]} " =~ " ${mainphoneproject} " ]]; then
	cp -f "$CURDIR/$DIR/IndieLib_resources/GLEssentials" "$1/Documents/Data"
fi

if [ "$mainphoneproject" == "2DAnimation" ]; then
	cp -fr "$CURDIR/$DIR/IndieLib_resources/2DAnimation" "$TOPDIR_/."
	cp -fr "$CURDIR/$DIR/IndieLib_resources/IndieLib-Animation-Collision" "$TOPDIR_/."
fi

if [ "X" == "Y" ]; then
 	#chmod -R a+r $TOPDIR_
	find $TOPDIR_ -type d -exec chmod a+x {} \;
	find $TOPDIR_ -type d -exec chmod a+r {} \;
	find $TOPDIR_ -type d -exec chmod a+w {} \;
	find $TOPDIR_ -type f -exec chmod a+r {} \;
	find $TOPDIR_ -type f -exec chmod a+w {} \;
fi

