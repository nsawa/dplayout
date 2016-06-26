# Microsoft Developer Studio Project File - Name="dplayout" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=dplayout - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "dplayout.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "dplayout.mak" CFG="dplayout - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "dplayout - Win32 Release" ("Win32 (x86) Console Application" 用)
!MESSAGE "dplayout - Win32 Debug" ("Win32 (x86) Console Application" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "dplayout - Win32 Release"

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
# ADD CPP /nologo /MT /W3 /GX /O2 /I "." /I "..\.." /I "..\..\libclip" /I "..\..\tool\gc" /I "..\..\tool\pthreads" /I "..\..\tool\sqlite" /I "..\..\tool\pdcurses" /I "..\..\tool\SofdecHelper" /I "..\..\tool\ssbpLib" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "EXTCLIPH" /D YYDEBUG=1 /D "YYERROR_VERBOSE" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib SETARGV.OBJ /nologo /subsystem:console /machine:I386 /nodefaultlib:"LIBC" /nodefaultlib:"LIBCD" /nodefaultlib:"LIBCPMT" /nodefaultlib:"LIBCPMTD" /nodefaultlib:"LIBCMTD" /libpath:"." /libpath:"..\.." /libpath:"..\..\libclip\Release" /libpath:"..\..\tool\gc" /libpath:"..\..\tool\pthreads" /libpath:"..\..\tool\sqlite" /libpath:"..\..\tool\pdcurses" /libpath:"..\..\tool\SofdecHelper" /libpath:"..\..\tool\ssbpLib" /libpath:"..\..\parson\Release" /libpath:"..\..\yaml\Release" /libpath:"..\..\liblua\win\Release"

!ELSEIF  "$(CFG)" == "dplayout - Win32 Debug"

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
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /I "." /I "..\.." /I "..\..\libclip" /I "..\..\tool\gc" /I "..\..\tool\pthreads" /I "..\..\tool\sqlite" /I "..\..\tool\pdcurses" /I "..\..\tool\SofdecHelper" /I "..\..\tool\ssbpLib" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "EXTCLIPH" /D YYDEBUG=1 /D "YYERROR_VERBOSE" /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib SETARGV.OBJ /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"LIBC" /nodefaultlib:"LIBCD" /nodefaultlib:"LIBCPMT" /nodefaultlib:"LIBCPMTD" /nodefaultlib:"LIBCMT" /pdbtype:sept /libpath:"." /libpath:"..\.." /libpath:"..\..\libclip\Debug" /libpath:"..\..\tool\gc" /libpath:"..\..\tool\pthreads" /libpath:"..\..\tool\sqlite" /libpath:"..\..\tool\pdcurses" /libpath:"..\..\tool\SofdecHelper" /libpath:"..\..\tool\ssbpLib" /libpath:"..\..\parson\Debug" /libpath:"..\..\yaml\Debug" /libpath:"..\..\liblua\win\Debug"

!ENDIF 

# Begin Target

# Name "dplayout - Win32 Release"
# Name "dplayout - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\app.cpp
# End Source File
# Begin Source File

SOURCE=.\work.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\app.h
# End Source File
# Begin Source File

SOURCE=.\work.h
# End Source File
# End Group
# End Target
# End Project
