# Microsoft Developer Studio Project File - Name="wxgui" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=wxgui - Win32 Debug DLL
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "wxgui.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "wxgui.mak" CFG="wxgui - Win32 Debug DLL"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "wxgui - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "wxgui - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "wxgui - Win32 Debug DLL" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "wxgui - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O1 /Ob2 /I "wxnetwork/" /I "bitmap/" /I "vc/" /I "../../lib/include" /D "NDEBUG" /D "WIN32" /D "BYTE_ORDER_LITTLE_ENDIAN" /D "_WIN32" /D "_WINDOWS" /D "__WINDOWS__" /D "__WXMSW__" /D "__WIN95__" /D "__WIN32__" /D WINVER=0x0400 /D "STRICT" /FD /I /cvs/wxWindows" /I /cvs/wxWindows/include" " /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib rpcrt4.lib wsock32.lib winmm.lib wx.lib png.lib zlib.lib jpeg.lib tiff.lib Ws2_32.lib cryptlib.lib kimlib.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libcd.lib" /nodefaultlib:"libcid.lib" /nodefaultlib:"msvcrt.lib" /out:"Release/kim.exe" /libpath:"../../lib/Release" /libpath:"/cvs/cryptopp/src/release" /libpath:"/cvs/wxWindows/lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "wxgui - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "include/" /I "src/" /I "src/vc/" /I "../../lib/include" /I "../../wxWindows/lib/mswd" /I "../../wxWindows/include" /I "../../" /D "__WXDEBUG__" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "__WINDOWS__" /D "__WXMSW__" /D DEBUG=1 /D "__WIN95__" /D "__WIN32__" /D WINVER=0x0400 /D "STRICT" /D "BYTE_ORDER_LITTLE_ENDIAN" /FD /I /cvs/wxWindows/include" /I /cvs/kim/src/client/wx/" /I /cvs/wxWindows/include/" /I /cvs/wxWindows/include" /I /cvs/wxWindows/include" " " " " " /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /i "../../wxWindows/include" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib rpcrt4.lib wsock32.lib winmm.lib wxmsw233d.lib libgm.lib cryptlib.lib /nologo /subsystem:windows /incremental:no /pdb:"Debug\nnim.pdb" /debug /machine:I386 /nodefaultlib:"libcd.lib" /nodefaultlib:"libcid.lib" /nodefaultlib:"msvcrt.lib" /out:"Debug\nnim_debug.exe" /pdbtype:sept /libpath:"../../crypto/release" /libpath:"../../lib/Debug" /libpath:"../../wxWindows/lib" /fixed:no /libpath:"/wx2/lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "wxgui - Win32 Debug DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "wxgui___Win32_Debug_DLL"
# PROP BASE Intermediate_Dir "wxgui___Win32_Debug_DLL"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugDLL"
# PROP Intermediate_Dir "DebugDLL"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /I "include/" /I "src/gui/bitmaps/" /I "src/vc/" /I "../../lib/include" /I "../../wxWindows/lib/mswd" /I "../../wxWindows/include" /I "../../cryptopp/src" /D "__WXDEBUG__" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "__WINDOWS__" /D "__WXMSW__" /D DEBUG=1 /D "__WIN95__" /D "__WIN32__" /D WINVER=0x0400 /D "STRICT" /D "BYTE_ORDER_LITTLE_ENDIAN" /FD /I /cvs/wxWindows/include" /I /cvs/kim/src/client/wx/" /I /cvs/wxWindows/include/" /I /cvs/wxWindows/include" /I /cvs/wxWindows/include" " " " " " /c
# ADD CPP /nologo /MDd /W3 /GX /Zi /Od /Gf /I "..\..\wxWindows\include" /I "..\..\wxWindows\lib\mswdlld" /I "include" /I "..\..\lib\include" /I "src" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D WINVER=0x400 /D "_MT" /D wxUSE_GUI=1 /D "__WXDEBUG__" /D "WXUSINGDLL" /D "__WXMSW__" /D "__WIN32__" /Fp"DebugDll/nnim_debug.pch" /YX /FD /c
# ADD BASE RSC /l 0x409 /i "../../wxWindows/include" /d "_DEBUG"
# ADD RSC /l 0x409 /i "../../wxWindows/include" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib rpcrt4.lib wsock32.lib winmm.lib wxmswd.lib pngd.lib zlibd.lib jpegd.lib tiffd.lib cryptlib.lib libGM.lib /nologo /subsystem:windows /incremental:no /pdb:"Debug\nnim.pdb" /debug /machine:I386 /nodefaultlib:"libcd.lib" /nodefaultlib:"libcid.lib" /nodefaultlib:"msvcrt.lib" /out:"Debug\nnim_debug.exe" /pdbtype:sept /libpath:"../../crypto/debug" /libpath:"../../lib/Debug" /libpath:"../../wxWindows/lib" /fixed:no /libpath:"/wx2/lib"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib rpcrt4.lib wsock32.lib wxmsw233d.lib cryptlib.lib libGM.lib /nologo /version:0.4 /subsystem:windows /incremental:no /debug /machine:I386 /out:"DebugDll/nnim_debug.exe" /pdbtype:sept /libpath:"../../wxWindows/lib" /libpath:"../../crypto/release" /libpath:"../../lib/Debug"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "wxgui - Win32 Release"
# Name "wxgui - Win32 Debug"
# Name "wxgui - Win32 Debug DLL"
# Begin Group "Resources"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\src\vc\resource.rc

!IF  "$(CFG)" == "wxgui - Win32 Release"

# ADD BASE RSC /l 0x409 /i "src\vc" /i "vc"
# ADD RSC /l 0x409 /i "src\vc" /i "vc" /i /cvs/wxWindows/include" "

!ELSEIF  "$(CFG)" == "wxgui - Win32 Debug"

# ADD BASE RSC /l 0x409 /i "src\vc" /i "vc"
# SUBTRACT BASE RSC /i "../../wxWindows/include"
# ADD RSC /l 0x409 /i "src\vc" /i /cvs/wxWindows/include" "
# SUBTRACT RSC /i "../../wxWindows/include"

!ELSEIF  "$(CFG)" == "wxgui - Win32 Debug DLL"

# ADD BASE RSC /l 0x409 /i "src\vc" /i /cvs/wxWindows/include" "
# SUBTRACT BASE RSC /i "../../wxWindows/include"
# ADD RSC /l 0x409 /i "src\vc" /i "..\..\wxWindows\include" /i /cvs/wxWindows/include" "
# SUBTRACT RSC /i "../../wxWindows/include"

!ENDIF 

# End Source File
# End Group
# Begin Group "Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\include\NAbout.h
# End Source File
# Begin Source File

SOURCE=.\include\NChat.h
# End Source File
# Begin Source File

SOURCE=.\include\NContact.h
# End Source File
# Begin Source File

SOURCE=.\include\NContactEdit.h
# End Source File
# Begin Source File

SOURCE=.\include\NContactTree.h
# End Source File
# Begin Source File

SOURCE=.\include\NContactTreeXML.h
# End Source File
# Begin Source File

SOURCE=.\include\NContactWiz.h
# End Source File
# Begin Source File

SOURCE=.\include\NDerivedClasses.h
# End Source File
# Begin Source File

SOURCE=.\include\NEvent.h
# End Source File
# Begin Source File

SOURCE=.\include\NInclude.h
# End Source File
# Begin Source File

SOURCE=.\include\NLog.h
# End Source File
# Begin Source File

SOURCE=.\include\NLogin.h
# End Source File
# Begin Source File

SOURCE=.\include\NMain.h
# End Source File
# Begin Source File

SOURCE=.\include\NNets.h
# End Source File
# Begin Source File

SOURCE=.\include\NPrefGen.h
# End Source File
# Begin Source File

SOURCE=.\include\NPrefNet.h
# End Source File
# Begin Source File

SOURCE=.\include\NPrefs.h
# End Source File
# Begin Source File

SOURCE=.\include\NPrefSec.h
# End Source File
# Begin Source File

SOURCE=.\include\NProtocol_wdr.h
# End Source File
# Begin Source File

SOURCE=.\include\NProtocolManager.h
# End Source File
# Begin Source File

SOURCE=.\include\NResources_wdr.h
# End Source File
# Begin Source File

SOURCE=.\include\NSocket.h
# End Source File
# Begin Source File

SOURCE=.\include\NSocketServer.h
# End Source File
# Begin Source File

SOURCE=.\include\NTaskbar.h
# End Source File
# Begin Source File

SOURCE=.\include\NUserWiz.h
# End Source File
# Begin Source File

SOURCE=.\include\NWizards_wdr.h
# End Source File
# Begin Source File

SOURCE=.\include\NXpms.h
# End Source File
# End Group
# Begin Group "Windows"

# PROP Default_Filter ""
# Begin Group "Trees"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\windows\NContactTree.cpp
# End Source File
# Begin Source File

SOURCE=.\src\windows\NContactTreeXML.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\src\windows\NAbout.cpp
# End Source File
# Begin Source File

SOURCE=.\src\windows\NChat.cpp
# End Source File
# Begin Source File

SOURCE=.\src\windows\NContact.cpp
# End Source File
# Begin Source File

SOURCE=.\src\windows\NContactEdit.cpp

!IF  "$(CFG)" == "wxgui - Win32 Release"

!ELSEIF  "$(CFG)" == "wxgui - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxgui - Win32 Debug DLL"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\windows\NLog.cpp
# End Source File
# Begin Source File

SOURCE=.\src\windows\NLogin.cpp
# End Source File
# Begin Source File

SOURCE=.\src\windows\NNets.cpp

!IF  "$(CFG)" == "wxgui - Win32 Release"

!ELSEIF  "$(CFG)" == "wxgui - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxgui - Win32 Debug DLL"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\windows\NPrefGen.cpp
# End Source File
# Begin Source File

SOURCE=.\src\windows\NPrefNet.cpp
# End Source File
# Begin Source File

SOURCE=.\src\windows\NPrefs.cpp
# End Source File
# Begin Source File

SOURCE=.\src\windows\NPrefSec.cpp
# End Source File
# Begin Source File

SOURCE=.\src\windows\NProtocol_wdr.cpp
# End Source File
# Begin Source File

SOURCE=.\src\windows\NResources_wdr.cpp
# End Source File
# End Group
# Begin Group "Wizards"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\wizards\NContactWiz.cpp
# End Source File
# Begin Source File

SOURCE=.\src\wizards\NUserWiz.cpp
# End Source File
# Begin Source File

SOURCE=.\src\wizards\NWizards_wdr.cpp
# End Source File
# End Group
# Begin Group "Main"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\NEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\src\NMain.cpp
# End Source File
# Begin Source File

SOURCE=.\src\NProtocolManager.cpp
# End Source File
# Begin Source File

SOURCE=.\src\NSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\src\NSocketServer.cpp

!IF  "$(CFG)" == "wxgui - Win32 Release"

!ELSEIF  "$(CFG)" == "wxgui - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxgui - Win32 Debug DLL"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=.\src\windows\NTaskbar.cpp
# End Source File
# Begin Source File

SOURCE=.\README
# End Source File
# Begin Source File

SOURCE=.\standard.txt
# End Source File
# Begin Source File

SOURCE=.\TODO.wx
# End Source File
# End Target
# End Project
