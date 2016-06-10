rem
rem Copyright (c) 2015 Tommi Roenty   http://www.tommironty.fi/
rem Licensed under The GNU Lesser General Public License, version 2.1:
rem     http://opensource.org/licenses/LGPL-2.1
rem

IF not EXIST "C:\\Python27" GOTO :PY27_INSTALL

SET x86=""
SET mscver=9
SET ok=0
:SymLoop
IF EXIST "C:\\Program Files\\Microsoft Visual Studio %mscver%.0\\VC\\bin\\vcvars32.bat" (
	call "C:\\Program Files\\Microsoft Visual Studio %mscver%.0\\VC\\bin\\vcvars32.bat"
	SET ok=1
)
IF EXIST "C:\\Program Files (x86)\\Microsoft Visual Studio %mscver%.0\\VC\\bin\\vcvars32.bat" (
	call "C:\\Program Files (x86)\\Microsoft Visual Studio %mscver%.0\\VC\\bin\\vcvars32.bat"
	SET ok=1
	SET x86=" (x86)"
)
IF "%ok%"=="1" GOTO EndSymLoop
SET mscver+=1
IF %mscver%>12 GOTO EndSymLoop
GOTO SymLoop
:EndSymLoop

IF %ok%==0 (
        ECHO "vcvars32.bat not found."
	GOTO end
)

CALL cnf.bat

IF "%1"=="RadeonTerrainDemo" GOTO RadeonTerrainDemo
IF "%1"=="NatureScene" GOTO NatureScene
IF "%1"=="ParticleVS" GOTO ParticleVS
IF "%1"=="Particles" GOTO Particles
IF "%1"=="otl-nightly" GOTO otl-nightly

SET USERPROFILE=%USERPROFILE%

SET solution="Test"
SET makesystem="NMAKE"
SET Projects_py_return_value="0"

IF "%1"=="help" GOTO help
IF "%1"=="clean" GOTO clean

IF not "%1"=="" SET solution="%1"
IF not "%2"=="" SET makesystem="%2"
rem SET platforms3=NDK NMAKE VC MW PRO CMake
rem IF not %makesystem% in (%platforms3%) GOTO usagent
GOTO all

:PY27_INSTALL
	ECHO "Could not find the Python, install C:\\Python27 and add it to the path"
	GOTO end

:help
	nmake /F ".\Makefile.msc" help
	GOTO end

:all
	cd .\\DesktopProjects
	CALL C:\\Python27\\python.exe Projects.py --solution %solution% --buildtool %makesystem%
	cd ..
	IF "%Projects_py_return_value%"=="3" GOTO usagent
	nmake /F ".\Makefile.msc" solution
	GOTO end

:clean
	nmake /F ".\Makefile.msc" clean
	GOTO end

:Particles
	cscript j_unzip.vbs Samples\ParticleVS\Chap12.zip Samples\ParticleVS\Chap12
	cd Samples\ParticleVS\Chap12\Chap12\Particles
	CALL Particles.exe
	cd ..\\..\\..
	del /S /Q .\\Chap12
	rmdir /S /Q .\\Chap12
	cd ..\\..
	GOTO end

:ParticleVS
	cscript j_unzip.vbs Samples\ParticleVS\Chap12.zip Samples\ParticleVS\Chap12
	cd Samples\ParticleVS\Chap12\Chap12\ParticleVS
	CALL ParticlesVS.exe
	cd ..\\..\\..
	del /S /Q .\\Chap12
	rmdir /S /Q .\\Chap12
	cd ..\\..
	GOTO end

:RadeonTerrainDemo
	cscript j_unzip.vbs Samples\RadeonTerrainDemo\RadeonTerrainDemo.zip Samples\RadeonTerrainDemo
	cd Samples\RadeonTerrainDemo\RadeonTerrainDemo
	CALL RadeonTerrainDemo.exe
	cd ..
	del /S /Q .\\RadeonTerrainDemo
	rmdir /S /Q .\\RadeonTerrainDemo
	cd ..\\..
	GOTO end

:NatureScene
	cscript j_unzip.vbs Samples\NatureScene\NatureScene.zip Samples\NatureScene
	cd "Samples\NatureScene\NatureScene\Application Nature"
	CALL SceneGraph.exe
	cd ..\\..
	del /S /Q .\\NatureScene
	rmdir /S /Q .\\NatureScene
	cd ..\\..
	GOTO end

:otl-nightly
	cscript j_unzip.vbs Libraries\otl_SDL.zip Libraries\otl_SDL
	CALL Make.bat otl_SDL
	CALL run.bat
	del /S /Q .\\Libraries\\otl_SDL
	rmdir /S /Q .\\Libraries\\otl_SDL
	GOTO end

:usagent
	nmake /F ".\Makefile.msc" help
	GOTO end

:end

