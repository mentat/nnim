# Microsoft Developer Studio Project File - Name="protocol" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=protocol - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "protocol.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "protocol.mak" CFG="protocol - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "protocol - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "protocol - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "protocol - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x41d /d "NDEBUG"
# ADD RSC /l 0x41d /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "protocol - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ  /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "." /I "../crypto" /I "./icq" /I "./msn" /I "../common" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ  /c
# ADD BASE RSC /l 0x41d /d "_DEBUG"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "protocol - Win32 Release"
# Name "protocol - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\icq\fnacbospackets.cpp
# End Source File
# Begin Source File

SOURCE=.\icq\fnacbudpackets.cpp
# End Source File
# Begin Source File

SOURCE=.\icq\fnacgenpackets.cpp
# End Source File
# Begin Source File

SOURCE=.\icq\fnaclocpackets.cpp
# End Source File
# Begin Source File

SOURCE=.\icq\fnacmsgpackets.cpp
# End Source File
# Begin Source File

SOURCE=.\icq\fnacpacket.cpp
# End Source File
# Begin Source File

SOURCE=.\icq\icqprotocol.cpp
# End Source File
# Begin Source File

SOURCE=.\kitprotocol.cpp
# End Source File
# Begin Source File

SOURCE=.\manager.cpp
# End Source File
# Begin Source File

SOURCE=.\msn\msnprotocol.cpp
# End Source File
# Begin Source File

SOURCE=.\icq\packet.cpp
# End Source File
# Begin Source File

SOURCE=.\icq\packets.cpp
# End Source File
# Begin Source File

SOURCE=.\icq\parsefnac.cpp
# End Source File
# Begin Source File

SOURCE=.\icq\parsefnacbos.cpp
# End Source File
# Begin Source File

SOURCE=.\icq\parsefnacbud.cpp
# End Source File
# Begin Source File

SOURCE=.\icq\parsefnacgen.cpp
# End Source File
# Begin Source File

SOURCE=.\icq\parsefnacloc.cpp
# End Source File
# Begin Source File

SOURCE=.\icq\parsefnacmsg.cpp
# End Source File
# Begin Source File

SOURCE=.\icq\parsepacket.cpp
# End Source File
# Begin Source File

SOURCE=.\peerprotocol.cpp
# End Source File
# Begin Source File

SOURCE=.\protocol.cpp
# End Source File
# Begin Source File

SOURCE=.\icq\stdpackets.cpp
# End Source File
# Begin Source File

SOURCE=.\tocprotocol.cpp
# End Source File
# Begin Source File

SOURCE=.\icq\util.cpp
# End Source File
# Begin Source File

SOURCE=.\yahooprotocol.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\icq\fnacbudpackets.h
# End Source File
# Begin Source File

SOURCE=.\icq\fnacgenpackets.h
# End Source File
# Begin Source File

SOURCE=.\icq\fnaclocpackets.h
# End Source File
# Begin Source File

SOURCE=.\icq\fnacmsgpackets.h
# End Source File
# Begin Source File

SOURCE=.\icq\fnacpacket.h
# End Source File
# Begin Source File

SOURCE=.\icq\iceq.h
# End Source File
# Begin Source File

SOURCE=.\icq\icqprotocol.h
# End Source File
# Begin Source File

SOURCE=.\kitprotocol.h
# End Source File
# Begin Source File

SOURCE=.\manager.h
# End Source File
# Begin Source File

SOURCE=.\msn\msnprotocol.h
# End Source File
# Begin Source File

SOURCE=.\icq\packet.h
# End Source File
# Begin Source File

SOURCE=.\icq\packets.h
# End Source File
# Begin Source File

SOURCE=.\peerprotocol.h
# End Source File
# Begin Source File

SOURCE=.\protocol.h
# End Source File
# Begin Source File

SOURCE=.\icq\stdpackets.h
# End Source File
# Begin Source File

SOURCE=.\tocprotocol.h
# End Source File
# Begin Source File

SOURCE=.\icq\util.h
# End Source File
# Begin Source File

SOURCE=.\icq\v7protocol.h
# End Source File
# Begin Source File

SOURCE=.\yahooprotocol.h
# End Source File
# End Group
# End Target
# End Project
