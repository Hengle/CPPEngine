#!/bin/bash
source ../../../make.cnf

CURDIR=`pwd -P`
echo "$CURDIR"

assets()
{

   if [ -n "$1" ]
   then
	DIR=$1
   else
	DIR=$CURDIR/../../../..
   fi

	TARGET_DIR=../../../bin/$mainphoneproject.app/Documents/IndieLib_resources

	mkdir -p $TARGET_DIR
 	#cp -fr $DIR/IndieLib_resources $BUILDPATH/assets/.

	arr=("IndieLib-Animation-Collision" "IndieLib-Blend" "IndieLib-Fading" "IndieLib-FloatingText" "IndieLib-ParallaxScroll" "IndieLib-TankEnemies" "IndieLib-TankEnemiesParallax" "IndieLib-TankFollowsMouseFires" "IndieLib-Transitions" "RadeonTerrainDemo" "NatureScene" "testspine2" "ParticleVS")

	if [[ " ${arr[@]} " =~ " ${mainphoneproject} " ]]; then
		echo "cp -fr $DIR/IndieLib_resources/$mainphoneproject $TARGET_DIR/."
		cp -fr $DIR/IndieLib_resources/$mainphoneproject $TARGET_DIR/.
	fi

	arr=("a_08_Collisions" "a_16_IND_Timer" "a_02_IND_Surface" "b_01_IND_Surface_Grids" "a_03_IND_Image" "b_02_Blitting_2d_Directly" "a_04_IND_Animation" "a_11_Animated_Tile_Scrolling" "a_05_IND_Font" "a_13_2d_Camera" "b_04_Several_ViewPorts" "a_06_Primitives" "c_01_Alien_BenchMark" "a_07_IND_Input" "a_15_Parallax_Scrolling" "Texture" "Blending")

	if [[ " ${arr[@]} " =~ " ${mainphoneproject} " ]]; then
		echo "cp -fr $DIR/IndieLib_resources/$mainphoneproject $TARGET_DIR/."
		cp -fr $DIR/IndieLib_resources/$mainphoneproject $TARGET_DIR/.
	fi

	arr=("3DAnimation" "b_03_Blitting_3d_Directly" "a_09_IND_3dMesh")
	if [[ " ${arr[@]} " =~ " ${mainphoneproject} " ]]; then
		echo "cp -fr $DIR/IndieLib_resources/skeleton $TARGET_DIR/."
		cp -fr $DIR/IndieLib_resources/skeleton $TARGET_DIR/.
	fi

	arr=("a_10_IND_Light" "a_14_3d_Camera")
	if [[ " ${arr[@]} " =~ " ${mainphoneproject} " ]]; then
		echo "cp -fr "$DIR/IndieLib_resources/trex dx" $TARGET_DIR/."
		cp -fr "$DIR/IndieLib_resources/trex dx" $TARGET_DIR/.
	fi
	arr=("Test" "SimpleTexture")
	if [[ " ${arr[@]} " =~ " ${mainphoneproject} " ]]; then
		echo "cp -f $DIR/IndieLib_resources/test.bmp $TARGET_DIR/."
		cp -f $DIR/IndieLib_resources/test.bmp $TARGET_DIR/.
	fi
	if [ "$mainphoneproject" == "2DAnimation" ]; then
		echo "cp -fr $DIR/IndieLib_resources/2DAnimation $TARGET_DIR/."
		cp -fr $DIR/IndieLib_resources/2DAnimation $TARGET_DIR/.
		echo "cp -fr $DIR/IndieLib_resources/IndieLib-Animation-Collision $TARGET_DIR/."
		cp -fr $DIR/IndieLib_resources/IndieLib-Animation-Collision $TARGET_DIR/.
	fi
}

assets


