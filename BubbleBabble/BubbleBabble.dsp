# Microsoft Developer Studio Project File - Name="BubbleBabble" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=BubbleBabble - Win32 DebugST
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BubbleBabble.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BubbleBabble.mak" CFG="BubbleBabble - Win32 DebugST"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BubbleBabble - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "BubbleBabble - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "BubbleBabble - Win32 DebugU" (based on "Win32 (x86) Static Library")
!MESSAGE "BubbleBabble - Win32 ReleaseU" (based on "Win32 (x86) Static Library")
!MESSAGE "BubbleBabble - Win32 DebugST" (based on "Win32 (x86) Static Library")
!MESSAGE "BubbleBabble - Win32 ReleaseST" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "BubbleBabble - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "$(CRYPTOPPDIR)" /D "WIN32" /D "NDEBUG" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x424 /d "NDEBUG"
# ADD RSC /l 0x424 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "BubbleBabble - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /I "$(CRYPTOPPDIR)" /D "WIN32" /D "_DEBUG" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x424 /d "_DEBUG"
# ADD RSC /l 0x424 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "BubbleBabble - Win32 DebugU"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "BubbleBabble___Win32_DebugU"
# PROP BASE Intermediate_Dir "BubbleBabble___Win32_DebugU"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugU"
# PROP Intermediate_Dir "DebugU"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /I "$(CRYPTOPPDIR)" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /I "$(CRYPTOPPDIR)" /D "WIN32" /D "_DEBUG" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x424 /d "_DEBUG"
# ADD RSC /l 0x424 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "BubbleBabble - Win32 ReleaseU"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "BubbleBabble___Win32_ReleaseU"
# PROP BASE Intermediate_Dir "BubbleBabble___Win32_ReleaseU"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseU"
# PROP Intermediate_Dir "ReleaseU"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "$(CRYPTOPPDIR)" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "$(CRYPTOPPDIR)" /D "WIN32" /D "NDEBUG" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x424 /d "NDEBUG"
# ADD RSC /l 0x424 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "BubbleBabble - Win32 DebugST"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "BubbleBabble___Win32_DebugST"
# PROP BASE Intermediate_Dir "BubbleBabble___Win32_DebugST"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugST"
# PROP Intermediate_Dir "DebugST"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /I "$(CRYPTOPPDIR)" /D "WIN32" /D "_DEBUG" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /I "$(CRYPTOPPDIR)" /D "WIN32" /D "_DEBUG" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x424 /d "_DEBUG"
# ADD RSC /l 0x424 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "BubbleBabble - Win32 ReleaseST"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "BubbleBabble___Win32_ReleaseST"
# PROP BASE Intermediate_Dir "BubbleBabble___Win32_ReleaseST"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseST"
# PROP Intermediate_Dir "ReleaseST"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "$(CRYPTOPPDIR)" /D "WIN32" /D "NDEBUG" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /I "$(CRYPTOPPDIR)" /D "WIN32" /D "NDEBUG" /D "_LIB" /YX /FD /c
# SUBTRACT CPP /O<none>
# ADD BASE RSC /l 0x424 /d "NDEBUG"
# ADD RSC /l 0x424 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "BubbleBabble - Win32 Release"
# Name "BubbleBabble - Win32 Debug"
# Name "BubbleBabble - Win32 DebugU"
# Name "BubbleBabble - Win32 ReleaseU"
# Name "BubbleBabble - Win32 DebugST"
# Name "BubbleBabble - Win32 ReleaseST"
# Begin Source File

SOURCE=.\babble.cpp
# End Source File
# Begin Source File

SOURCE=.\babble.h
# End Source File
# End Target
# End Project
