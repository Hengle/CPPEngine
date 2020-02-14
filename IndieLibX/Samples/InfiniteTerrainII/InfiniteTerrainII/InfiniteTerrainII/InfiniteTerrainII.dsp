# Microsoft Developer Studio Project File - Name="InfiniteTerrainII" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=InfiniteTerrainII - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "InfiniteTerrainII.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "InfiniteTerrainII.mak" CFG="InfiniteTerrainII - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "InfiniteTerrainII - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "InfiniteTerrainII - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "InfiniteTerrainII - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /G6 /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "NO_PNG" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x41d /d "NDEBUG"
# ADD RSC /l 0x41d /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /pdb:none /machine:I386 /nodefaultlib:"msvcrt" /out:"InfiniteTerrainII.exe"

!ELSEIF  "$(CFG)" == "InfiniteTerrainII - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x41d /d "_DEBUG"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libc" /nodefaultlib:"msvcrt" /out:"InfiniteTerrainII_d.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "InfiniteTerrainII - Win32 Release"
# Name "InfiniteTerrainII - Win32 Debug"
# Begin Group "Framework3"

# PROP Default_Filter ""
# Begin Group "Direct3D"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Framework3\Direct3D\D3DApp.cpp
# End Source File
# Begin Source File

SOURCE=..\Framework3\Direct3D\D3DApp.h
# End Source File
# Begin Source File

SOURCE=..\Framework3\Direct3D\Direct3DRenderer.cpp
# End Source File
# Begin Source File

SOURCE=..\Framework3\Direct3D\Direct3DRenderer.h
# End Source File
# End Group
# Begin Group "GUI"

# PROP Default_Filter "*.cpp;*.h"
# Begin Source File

SOURCE=..\Framework3\GUI\Button.cpp
# End Source File
# Begin Source File

SOURCE=..\Framework3\GUI\Button.h
# End Source File
# Begin Source File

SOURCE=..\Framework3\GUI\CheckBox.cpp
# End Source File
# Begin Source File

SOURCE=..\Framework3\GUI\CheckBox.h
# End Source File
# Begin Source File

SOURCE=..\Framework3\GUI\Dialog.cpp
# End Source File
# Begin Source File

SOURCE=..\Framework3\GUI\Dialog.h
# End Source File
# Begin Source File

SOURCE=..\Framework3\GUI\DropDownList.cpp
# End Source File
# Begin Source File

SOURCE=..\Framework3\GUI\DropDownList.h
# End Source File
# Begin Source File

SOURCE=..\Framework3\GUI\Label.cpp
# End Source File
# Begin Source File

SOURCE=..\Framework3\GUI\Label.h
# End Source File
# Begin Source File

SOURCE=..\Framework3\GUI\Slider.cpp
# End Source File
# Begin Source File

SOURCE=..\Framework3\GUI\Slider.h
# End Source File
# Begin Source File

SOURCE=..\Framework3\GUI\Widget.cpp
# End Source File
# Begin Source File

SOURCE=..\Framework3\GUI\Widget.h
# End Source File
# End Group
# Begin Group "Imaging"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Framework3\Imaging\Image.cpp
# End Source File
# Begin Source File

SOURCE=..\Framework3\Imaging\Image.h
# End Source File
# End Group
# Begin Group "Math"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Framework3\Math\Noise.cpp
# End Source File
# Begin Source File

SOURCE=..\Framework3\Math\Noise.h
# End Source File
# Begin Source File

SOURCE=..\Framework3\Math\Vector.cpp
# End Source File
# Begin Source File

SOURCE=..\Framework3\Math\Vector.h
# End Source File
# End Group
# Begin Group "Util"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Framework3\Util\String.cpp
# End Source File
# Begin Source File

SOURCE=..\Framework3\Util\String.h
# End Source File
# Begin Source File

SOURCE=..\Framework3\Util\Tokenizer.cpp
# End Source File
# Begin Source File

SOURCE=..\Framework3\Util\Tokenizer.h
# End Source File
# End Group
# Begin Group "Windows"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Framework3\Windows\Resources.rc
# End Source File
# Begin Source File

SOURCE=..\Framework3\Windows\WindowsBase.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\Framework3\BaseApp.cpp
# End Source File
# Begin Source File

SOURCE=..\Framework3\BaseApp.h
# End Source File
# Begin Source File

SOURCE=..\Framework3\Config.cpp
# End Source File
# Begin Source File

SOURCE=..\Framework3\Config.h
# End Source File
# Begin Source File

SOURCE=..\Framework3\CPU.cpp
# End Source File
# Begin Source File

SOURCE=..\Framework3\CPU.h
# End Source File
# Begin Source File

SOURCE=..\Framework3\Platform.cpp
# End Source File
# Begin Source File

SOURCE=..\Framework3\Platform.h
# End Source File
# Begin Source File

SOURCE=..\Framework3\Renderer.cpp
# End Source File
# Begin Source File

SOURCE=..\Framework3\Renderer.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\App.cpp
# End Source File
# Begin Source File

SOURCE=.\App.h
# End Source File
# Begin Source File

SOURCE=.\generateTerrain.shd
# End Source File
# Begin Source File

SOURCE=.\skybox.shd
# End Source File
# Begin Source File

SOURCE=.\tangentSpace.shd
# End Source File
# Begin Source File

SOURCE=.\terrain.shd
# End Source File
# End Target
# End Project
