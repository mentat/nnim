# Microsoft Developer Studio Project File - Name="libGM" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libGM - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libGM.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libGM.mak" CFG="libGM - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libGM - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libGM - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libGM - Win32 Release"

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
# ADD CPP /nologo /MT /W3 /GX /O2 /I "common/" /I "common/expat" /I "protocol/" /I "protocol/msn/" /I "protocol/icq" /I "." /I "include/" /I "common/expat/include/" /I "win32/" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "HAVE_CRYPTO" /D "_WINDOWS" /D "USE_PRECOMPILED_HEADERS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "libGM - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /GX /Zi /I "common/" /I "common/expat" /I "protocol/" /I "protocol/msn/" /I "protocol/icq" /I "include/" /I "common/expat/include/" /I "." /I "win32/" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "HAVE_CRYPTO" /D "_WINDOWS" /D "USE_PRECOMPILED_HEADERS" /FAs /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "libGM - Win32 Release"
# Name "libGM - Win32 Debug"
# Begin Group "Headers"

# PROP Default_Filter ".h"
# Begin Source File

SOURCE=.\include\authload.h
# End Source File
# Begin Source File

SOURCE=.\include\basenetwork.h
# End Source File
# Begin Source File

SOURCE=.\include\contact.h
# End Source File
# Begin Source File

SOURCE=.\include\cryptography.h
# End Source File
# Begin Source File

SOURCE=.\certificates\dstring.h
# End Source File
# Begin Source File

SOURCE=.\include\dstring.h
# End Source File
# Begin Source File

SOURCE=.\include\exception.h
# End Source File
# Begin Source File

SOURCE=.\include\expat.h
# End Source File
# Begin Source File

SOURCE=.\include\globals.h
# End Source File
# Begin Source File

SOURCE=.\certificates\gmstorage.h
# End Source File
# Begin Source File

SOURCE=.\include\icqprotocol.h
# End Source File
# Begin Source File

SOURCE=.\include\kitprotocol.h
# End Source File
# Begin Source File

SOURCE=.\include\log.h
# End Source File
# Begin Source File

SOURCE=.\include\manager.h
# End Source File
# Begin Source File

SOURCE=.\include\msnprotocol.h
# End Source File
# Begin Source File

SOURCE=.\include\peerprotocol.h
# End Source File
# Begin Source File

SOURCE=.\certificates\platform.h
# End Source File
# Begin Source File

SOURCE=.\include\platform.h
# End Source File
# Begin Source File

SOURCE=.\include\protocol.h
# End Source File
# Begin Source File

SOURCE=.\include\securecomm.h
# End Source File
# Begin Source File

SOURCE=.\include\sign.h
# End Source File
# Begin Source File

SOURCE=.\include\ssh.h
# End Source File
# Begin Source File

SOURCE=.\include\tocprotocol.h
# End Source File
# Begin Source File

SOURCE=.\include\xml.h
# End Source File
# Begin Source File

SOURCE=.\include\xmlnode.h
# End Source File
# Begin Source File

SOURCE=.\include\yahooprotocol.h
# End Source File
# End Group
# Begin Group "Common"

# PROP Default_Filter ""
# Begin Group "expat"

# PROP Default_Filter ""
# Begin Group "include"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\common\expat\include\asciitab.h
# End Source File
# Begin Source File

SOURCE=.\common\expat\include\hashtable.h
# End Source File
# Begin Source File

SOURCE=.\common\expat\include\iasciitab.h
# End Source File
# Begin Source File

SOURCE=.\common\expat\include\latin1tab.h
# End Source File
# Begin Source File

SOURCE=.\common\expat\include\nametab.h
# End Source File
# Begin Source File

SOURCE=.\common\expat\include\utf8tab.h
# End Source File
# Begin Source File

SOURCE=.\common\expat\include\xmldef.h
# End Source File
# Begin Source File

SOURCE=.\common\expat\include\xmlparse.h
# End Source File
# Begin Source File

SOURCE=.\common\expat\include\xmlrole.h
# End Source File
# Begin Source File

SOURCE=.\common\expat\include\xmltok.h
# End Source File
# Begin Source File

SOURCE=.\common\expat\include\xmltok_impl.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\common\expat\gennmtab.c
# End Source File
# Begin Source File

SOURCE=.\common\expat\hashtable.c
# End Source File
# Begin Source File

SOURCE=.\common\expat\xmlparse.c
# End Source File
# Begin Source File

SOURCE=.\common\expat\xmlrole.c
# End Source File
# Begin Source File

SOURCE=.\common\expat\xmltok.c
# End Source File
# Begin Source File

SOURCE=.\common\expat\xmltok_impl.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\common\expat\xmltok_ns.c
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "authload"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\common\auth_contact.cpp
# End Source File
# Begin Source File

SOURCE=.\common\auth_global.cpp
# End Source File
# Begin Source File

SOURCE=.\common\auth_user.cpp
# End Source File
# Begin Source File

SOURCE=.\common\authload.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\common\contact.cpp
# End Source File
# Begin Source File

SOURCE=.\common\cryptography.cpp
# End Source File
# Begin Source File

SOURCE=.\common\expat.cpp
# End Source File
# Begin Source File

SOURCE=.\common\log.cpp
# End Source File
# Begin Source File

SOURCE=.\common\xml.cpp
# End Source File
# Begin Source File

SOURCE=.\common\xmlnode.cpp
# End Source File
# End Group
# Begin Group "Protocol"

# PROP Default_Filter ""
# Begin Group "icq"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\protocol\icq\fnacbospackets.cpp
# End Source File
# Begin Source File

SOURCE=.\protocol\icq\fnacbudpackets.cpp
# End Source File
# Begin Source File

SOURCE=.\protocol\icq\fnacbudpackets.h
# End Source File
# Begin Source File

SOURCE=.\protocol\icq\fnacgenpackets.cpp
# End Source File
# Begin Source File

SOURCE=.\protocol\icq\fnacgenpackets.h
# End Source File
# Begin Source File

SOURCE=.\protocol\icq\fnacicqpackets.cpp
# End Source File
# Begin Source File

SOURCE=.\protocol\icq\fnacicqpackets.h
# End Source File
# Begin Source File

SOURCE=.\protocol\icq\fnaclocpackets.cpp
# End Source File
# Begin Source File

SOURCE=.\protocol\icq\fnaclocpackets.h
# End Source File
# Begin Source File

SOURCE=.\protocol\icq\fnacmsgpackets.cpp
# End Source File
# Begin Source File

SOURCE=.\protocol\icq\fnacmsgpackets.h
# End Source File
# Begin Source File

SOURCE=.\protocol\icq\fnacpacket.cpp
# End Source File
# Begin Source File

SOURCE=.\protocol\icq\fnacpacket.h
# End Source File
# Begin Source File

SOURCE=.\protocol\icq\iceq.h
# End Source File
# Begin Source File

SOURCE=.\protocol\icq\icqprotocol.cpp
# End Source File
# Begin Source File

SOURCE=.\protocol\icq\packet.cpp
# End Source File
# Begin Source File

SOURCE=.\protocol\icq\packet.h
# End Source File
# Begin Source File

SOURCE=.\protocol\icq\packets.cpp
# End Source File
# Begin Source File

SOURCE=.\protocol\icq\packets.h
# End Source File
# Begin Source File

SOURCE=.\protocol\icq\parsefnac.cpp
# End Source File
# Begin Source File

SOURCE=.\protocol\icq\parsefnacbos.cpp
# End Source File
# Begin Source File

SOURCE=.\protocol\icq\parsefnacbud.cpp
# End Source File
# Begin Source File

SOURCE=.\protocol\icq\parsefnacgen.cpp
# End Source File
# Begin Source File

SOURCE=.\protocol\icq\parsefnacicq.cpp
# End Source File
# Begin Source File

SOURCE=.\protocol\icq\parsefnacloc.cpp
# End Source File
# Begin Source File

SOURCE=.\protocol\icq\parsefnacmsg.cpp
# End Source File
# Begin Source File

SOURCE=.\protocol\icq\parsepacket.cpp
# End Source File
# Begin Source File

SOURCE=.\protocol\icq\stdpackets.cpp
# End Source File
# Begin Source File

SOURCE=.\protocol\icq\stdpackets.h
# End Source File
# Begin Source File

SOURCE=.\protocol\icq\util.cpp
# End Source File
# Begin Source File

SOURCE=.\protocol\icq\util.h
# End Source File
# Begin Source File

SOURCE=.\protocol\icq\v7protocol.h
# End Source File
# End Group
# Begin Group "msn"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\protocol\msn\command.h
# End Source File
# Begin Source File

SOURCE=.\protocol\msn\errors.h
# End Source File
# Begin Source File

SOURCE=.\protocol\msn\msnprotocol.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\protocol\emptyprotocol.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\protocol\emptyprotocol.h
# End Source File
# Begin Source File

SOURCE=.\protocol\kitprotocol.cpp
# End Source File
# Begin Source File

SOURCE=.\protocol\manager.cpp
# End Source File
# Begin Source File

SOURCE=.\protocol\peerprotocol.cpp

!IF  "$(CFG)" == "libGM - Win32 Release"

!ELSEIF  "$(CFG)" == "libGM - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\protocol\protocol.cpp
# End Source File
# Begin Source File

SOURCE=.\protocol\tocprotocol.cpp
# End Source File
# Begin Source File

SOURCE=.\protocol\yahooprotocol.cpp
# End Source File
# End Group
# Begin Group "Certificates"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\certificates\gmstorage.cpp
# End Source File
# Begin Source File

SOURCE=.\certificates\securecomm.cpp
# End Source File
# Begin Source File

SOURCE=.\certificates\sign.cpp
# End Source File
# Begin Source File

SOURCE=.\certificates\test.cpp
# End Source File
# End Group
# Begin Group "Crypto"

# PROP Default_Filter ""
# End Group
# Begin Source File

SOURCE=.\AUTHORS
# End Source File
# Begin Source File

SOURCE=.\ChangeLog
# End Source File
# Begin Source File

SOURCE=.\COPYING
# End Source File
# Begin Source File

SOURCE=.\INSTALL
# End Source File
# Begin Source File

SOURCE=.\NEWS
# End Source File
# Begin Source File

SOURCE=.\README
# End Source File
# Begin Source File

SOURCE=.\THANKS
# End Source File
# Begin Source File

SOURCE=.\XMLSpec.html
# End Source File
# End Target
# End Project
