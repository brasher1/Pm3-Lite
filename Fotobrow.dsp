# Microsoft Developer Studio Project File - Name="FOTOBROW" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=FOTOBROW - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Fotobrow.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Fotobrow.mak" CFG="FOTOBROW - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "FOTOBROW - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "FOTOBROW - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "FOTOBROW - Win32 BCPro Build" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "FOTOBROW - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\WinDebug"
# PROP BASE Intermediate_Dir ".\WinDebug"
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\windebug"
# PROP Intermediate_Dir ".\windebug"
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /Yu"stdafx.h" /c
# ADD CPP /nologo /Zp2 /MDd /W3 /Gm /GX /ZI /Od /I ".\inc" /I "." /I "d:\dev\maintsys" /I ".\photog" /I "c:\rdm32\winclude" /I "c:\pvapps\datatbl" /I "c:\rom32\ominc" /D "_DEBUG" /D "WIN32" /D "MSC" /D "OM_RDM" /D "_AFXDLL" /D "_MBCS" /D "FOTOBROW" /D "_WINDOWS" /Fr /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "c:\pgplus50\include" /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 c:\rdm32\lib\rdmwin32.lib c:\rom32\lib\msc\omrdmimp.lib glaux.lib glu32.lib opengl32.lib netapi32.lib mpr.lib /nologo /subsystem:windows /profile /debug /machine:I386 /out:".\winDebug\pm3lite.exe"
# SUBTRACT LINK32 /verbose /nodefaultlib

!ELSEIF  "$(CFG)" == "FOTOBROW - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\WinRel"
# PROP BASE Intermediate_Dir ".\WinRel"
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\winRel"
# PROP Intermediate_Dir ".\winRel"
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /Yu"stdafx.h" /c
# ADD CPP /nologo /Zp4 /MD /W3 /GX /I ".\inc" /I "." /I "d:\dev\maintsys" /I ".\photog" /I "c:\rdm32\winclude" /I "c:\pvapps\datatbl" /I "c:\rom32\ominc" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "MSC" /D "OM_RDM" /D "FOTOBROW" /D "_AFXDLL" /D "_MBCS" /Fr /FD /Gs /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "c:\pgplus50\include" /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 c:\rdm32\lib\rdmwin32.lib c:\rom32\lib\msc\omrdmimp.lib glaux.lib glu32.lib opengl32.lib netapi32.lib mpr.lib /nologo /subsystem:windows /profile /map /machine:I386 /out:".\winRel\pm3lite.exe"
# SUBTRACT LINK32 /verbose /nodefaultlib

!ELSEIF  "$(CFG)" == "FOTOBROW - Win32 BCPro Build"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\FOTOBROW"
# PROP BASE Intermediate_Dir ".\FOTOBROW"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\FOTOBROW"
# PROP Intermediate_Dir ".\FOTOBROW"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Zp4 /MDd /W3 /Gm /GX /Zi /Od /I ".\inc" /I "." /I "c:\dev\maintsys" /I ".\photog" /I "c:\rdm32\winclude" /I "c:\pgplus50\include" /I "c:\rom32\ominc" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "MSC" /D "OM_RDM" /D "_AFXDLL" /D "_MBCS" /D "FOTOBROW" /c
# SUBTRACT BASE CPP /Fr /Yc /Yu
# ADD CPP /nologo /Zp4 /MDd /W3 /Gm /GX /ZI /Od /I ".\inc" /I "." /I "d:\dev\maintsys" /I ".\photog" /I "c:\rdm32\winclude" /I "c:\pvapps\datatbl" /I "c:\rom32\ominc" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "MSC" /D "OM_RDM" /D "_AFXDLL" /D "_MBCS" /D "FOTOBROW" /FD /c
# SUBTRACT CPP /Fr /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /i "c:\pgplus50\include" /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "c:\pgplus50\include" /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 c:\rdm32\lib\rdmwin32.lib c:\rom32\lib\msc\omrdm.lib glaux.lib glu32.lib opengl32.lib netapi32.lib mpr.lib /nologo /subsystem:windows /profile /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"f:\pm3setup\wworks\data\program\pm3lite.exe"
# SUBTRACT BASE LINK32 /nodefaultlib
# ADD LINK32 c:\rdm32\lib\rdmwin32.lib c:\rom32\lib\msc\omrdmimp.lib glaux.lib glu32.lib opengl32.lib netapi32.lib mpr.lib /nologo /subsystem:windows /profile /debug /machine:I386 /out:"f:\pm3setup\wworks\data\program\pm3lite.exe"
# SUBTRACT LINK32 /verbose /nodefaultlib

!ENDIF 

# Begin Target

# Name "FOTOBROW - Win32 Debug"
# Name "FOTOBROW - Win32 Release"
# Name "FOTOBROW - Win32 BCPro Build"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\src\assigneq.cpp
# End Source File
# Begin Source File

SOURCE=.\src\bundle.cpp
# End Source File
# Begin Source File

SOURCE=.\BundleCalcDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\bundleutils.cpp
# End Source File
# Begin Source File

SOURCE=.\src\calc3dp.cpp
# End Source File
# Begin Source File

SOURCE=.\CircleBy3DPoint.cpp
# End Source File
# Begin Source File

SOURCE=.\CircleByEdge.cpp
# End Source File
# Begin Source File

SOURCE=.\CircleDataDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\colormenu.cpp
# End Source File
# Begin Source File

SOURCE=.\ColumnEditor.cpp
# End Source File
# Begin Source File

SOURCE=.\src\columnfo.cpp
# End Source File
# Begin Source File

SOURCE=.\src\comision.cpp
# End Source File
# Begin Source File

SOURCE=.\src\construct.cpp
# End Source File
# Begin Source File

SOURCE=.\conversion.cpp
# End Source File
# Begin Source File

SOURCE=.\src\coordxfo.cpp
# End Source File
# Begin Source File

SOURCE=.\CreateCylinderByEdgeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CreateCylinderByEndpointsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CreateQuickCylinderDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CylinderData.cpp
# End Source File
# Begin Source File

SOURCE=.\datatbl.cpp
# End Source File
# Begin Source File

SOURCE=.\DDXFXNS.CPP
# End Source File
# Begin Source File

SOURCE=.\src\DIB.CPP
# End Source File
# Begin Source File

SOURCE=.\src\DIBPAL.CPP
# End Source File
# Begin Source File

SOURCE=.\src\drawlist.cpp
# End Source File
# Begin Source File

SOURCE=.\dtcell.cpp
# End Source File
# Begin Source File

SOURCE=.\dtcellset.cpp
# End Source File
# Begin Source File

SOURCE=.\dtcolumn.cpp
# End Source File
# Begin Source File

SOURCE=.\dtcolumnset.cpp
# End Source File
# Begin Source File

SOURCE=.\dtcurcell.cpp
# End Source File
# Begin Source File

SOURCE=.\dtrow.cpp
# End Source File
# Begin Source File

SOURCE=.\dtrowset.cpp
# End Source File
# Begin Source File

SOURCE=.\photog\ele_math.cpp
# End Source File
# Begin Source File

SOURCE=.\src\elem_help.cpp
# End Source File
# Begin Source File

SOURCE=.\ElementListView.cpp
# End Source File
# Begin Source File

SOURCE=.\src\engineer.cpp
# End Source File
# Begin Source File

SOURCE=.\exportToDGN.cpp
# End Source File
# Begin Source File

SOURCE=.\src\feetstat.cpp
# End Source File
# Begin Source File

SOURCE=.\font.cpp
# End Source File
# Begin Source File

SOURCE=.\src\fotobdoc.cpp
# End Source File
# Begin Source File

SOURCE=.\src\fotobrow.cpp
# End Source File
# Begin Source File

SOURCE=.\src\fotobrow.odl
# End Source File
# Begin Source File

SOURCE=.\fotobrow.rc

!IF  "$(CFG)" == "FOTOBROW - Win32 Debug"

# ADD BASE RSC /l 0x409 /i ".\windebug" /i "windebug"
# SUBTRACT BASE RSC /i "c:\pgplus50\include"
# ADD RSC /l 0x409 /i ".\windebug" /i "$(OUTDIR)"

!ELSEIF  "$(CFG)" == "FOTOBROW - Win32 Release"

!ELSEIF  "$(CFG)" == "FOTOBROW - Win32 BCPro Build"

# ADD BASE RSC /l 0x409 /i ".\FOTOBROW" /i "FOTOBROW"
# SUBTRACT BASE RSC /i "c:\pgplus50\include"
# ADD RSC /l 0x409 /i ".\FOTOBROW" /i "$(OUTDIR)"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\fotobvw.cpp
# End Source File
# Begin Source File

SOURCE=.\src\fotogdoc.cpp
# End Source File
# Begin Source File

SOURCE=.\src\fotoview.cpp
# End Source File
# Begin Source File

SOURCE=.\src\generalp.cpp
# End Source File
# Begin Source File

SOURCE=.\gettrans.cpp
# End Source File
# Begin Source File

SOURCE=.\gridbar.cpp
# End Source File
# Begin Source File

SOURCE=.\3DClass\HierElem.cpp
# End Source File
# Begin Source File

SOURCE=.\src\img_help.cpp
# End Source File
# Begin Source File

SOURCE=.\src\imgoverlay.cpp
# End Source File
# Begin Source File

SOURCE=.\3dclass\importreview.cpp
# End Source File
# Begin Source File

SOURCE=.\importSurveyData.cpp
# End Source File
# Begin Source File

SOURCE=.\LightAdjust.cpp
# End Source File
# Begin Source File

SOURCE=.\src\mainfrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MAINTCOM.H
# End Source File
# Begin Source File

SOURCE=.\src\maintena.cpp
# End Source File
# Begin Source File

SOURCE=.\photog\mathsubs.cpp
# End Source File
# Begin Source File

SOURCE=.\src\mfc_frames.cpp
# End Source File
# Begin Source File

SOURCE=.\src\mymdi.cpp
# End Source File
# Begin Source File

SOURCE=.\src\numcnvt.cpp
# End Source File
# Begin Source File

SOURCE=.\OpenGLSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\OPENPROJ.CPP
# End Source File
# Begin Source File

SOURCE=.\src\optionsd.cpp
# End Source File
# Begin Source File

SOURCE=.\photog\output.cpp
# End Source File
# Begin Source File

SOURCE=.\src\paletteOGL.cpp
# End Source File
# Begin Source File

SOURCE=.\photog\photog_math.cpp
# End Source File
# Begin Source File

SOURCE=.\res\PM3BACK.BMP
# End Source File
# Begin Source File

SOURCE=.\PointGridBar.cpp
# End Source File
# Begin Source File

SOURCE=.\src\projects.cpp
# End Source File
# Begin Source File

SOURCE=.\3DClass\Ptg3DBoxObj.cpp
# End Source File
# Begin Source File

SOURCE=.\3DClass\Ptg3DBoxObjOGL.cpp
# End Source File
# Begin Source File

SOURCE=.\3dclass\Ptg3DCalcPointObj.cpp
# End Source File
# Begin Source File

SOURCE=.\3DClass\Ptg3DConeObj.cpp
# End Source File
# Begin Source File

SOURCE=.\3DClass\Ptg3DConeObjOGL.cpp
# End Source File
# Begin Source File

SOURCE=.\3DClass\Ptg3DCtorObj.cpp
# End Source File
# Begin Source File

SOURCE=.\3DClass\Ptg3DCtorObjOGL.cpp
# End Source File
# Begin Source File

SOURCE=.\3DClass\Ptg3DCylinderObj.cpp
# End Source File
# Begin Source File

SOURCE=.\3DClass\Ptg3DCylinderObjOGL.cpp
# End Source File
# Begin Source File

SOURCE=.\3dclass\Ptg3DFacetObj.cpp
# End Source File
# Begin Source File

SOURCE=.\3dclass\Ptg3DFacetObjOGL.cpp
# End Source File
# Begin Source File

SOURCE=.\3DClass\Ptg3DLineObj.cpp
# End Source File
# Begin Source File

SOURCE=.\3DClass\Ptg3DLineObjOGL.cpp
# End Source File
# Begin Source File

SOURCE=.\3DClass\Ptg3DNode.cpp
# End Source File
# Begin Source File

SOURCE=.\3DClass\Ptg3DNodeOGL.cpp
# End Source File
# Begin Source File

SOURCE=.\3dclass\Ptg3DPointObj.cpp
# End Source File
# Begin Source File

SOURCE=.\3dclass\Ptg3DPointObjOGL.cpp
# End Source File
# Begin Source File

SOURCE=.\3DClass\Ptg3DPyramidObj.cpp
# End Source File
# Begin Source File

SOURCE=.\3DClass\Ptg3DPyramidObjOGL.cpp
# End Source File
# Begin Source File

SOURCE=.\3DClass\Ptg3DRtorObj.cpp
# End Source File
# Begin Source File

SOURCE=.\3DClass\Ptg3DRtorObjOGL.cpp
# End Source File
# Begin Source File

SOURCE=.\3DClass\PtgCamera.cpp
# End Source File
# Begin Source File

SOURCE=.\3DClass\PtgCameraOGL.cpp
# End Source File
# Begin Source File

SOURCE=.\3DClass\PtgTransMatrix.cpp
# End Source File
# Begin Source File

SOURCE=.\3DClass\PtgTransMatrixOGL.cpp
# End Source File
# Begin Source File

SOURCE=.\3dclass\PtgWorld.cpp
# End Source File
# Begin Source File

SOURCE=.\RadiographyData.cpp
# End Source File
# Begin Source File

SOURCE=.\rdm_utils.cpp
# End Source File
# Begin Source File

SOURCE=.\src\regfuncs.cpp
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\src\resultsh.cpp
# End Source File
# Begin Source File

SOURCE=.\rvqlite\rvqConnect.cpp
# End Source File
# Begin Source File

SOURCE=.\rvqlite\rvqDelete.cpp
# End Source File
# Begin Source File

SOURCE=.\rvqlite\rvqLite.cpp
# End Source File
# Begin Source File

SOURCE=.\src\selecteq.cpp
# End Source File
# Begin Source File

SOURCE=.\src\selectim.cpp
# End Source File
# Begin Source File

SOURCE=.\Splash.cpp
# End Source File
# Begin Source File

SOURCE=.\src\stdafx.cpp
# End Source File
# Begin Source File

SOURCE=.\strutils.cpp
# End Source File
# Begin Source File

SOURCE=.\transfor.cpp
# End Source File
# Begin Source File

SOURCE=.\treemenu.cpp
# End Source File
# Begin Source File

SOURCE=.\triton\triton.cpp
# End Source File
# Begin Source File

SOURCE=.\triton\tritondb.cpp
# End Source File
# Begin Source File

SOURCE=.\triton\tritondraw.cpp
# End Source File
# Begin Source File

SOURCE=.\ZoneEditor.cpp
# End Source File
# Begin Source File

SOURCE=.\src\zoneform.cpp
# End Source File
# Begin Source File

SOURCE=.\src\zonesele.cpp
# End Source File
# Begin Source File

SOURCE=C:\rdm32\LIB\RDMWIN32.LIB
# End Source File
# Begin Source File

SOURCE=.\ObjDGNX\pm3dgnInterface.lib
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\inc\2DEdge.h
# End Source File
# Begin Source File

SOURCE=.\inc\3dpoints.h
# End Source File
# Begin Source File

SOURCE=.\inc\assigneq.h
# End Source File
# Begin Source File

SOURCE=.\bundlecalcdlg.h
# End Source File
# Begin Source File

SOURCE=.\inc\bundleutils.h
# End Source File
# Begin Source File

SOURCE=.\circleby3dpoint.h
# End Source File
# Begin Source File

SOURCE=.\circlebyedge.h
# End Source File
# Begin Source File

SOURCE=.\CircleDataDlg.h
# End Source File
# Begin Source File

SOURCE=.\colormenu.h
# End Source File
# Begin Source File

SOURCE=.\columneditor.h
# End Source File
# Begin Source File

SOURCE=.\inc\columnfo.h
# End Source File
# Begin Source File

SOURCE=.\inc\comision.h
# End Source File
# Begin Source File

SOURCE=.\inc\common.h
# End Source File
# Begin Source File

SOURCE=.\inc\construct.h
# End Source File
# Begin Source File

SOURCE=.\conversion.h
# End Source File
# Begin Source File

SOURCE=.\inc\coordxfo.h
# End Source File
# Begin Source File

SOURCE=.\createcylinderbyedgedlg.h
# End Source File
# Begin Source File

SOURCE=.\CreateCylinderByEndpointsDlg.h
# End Source File
# Begin Source File

SOURCE=.\CreateQuickCylinderDlg.h
# End Source File
# Begin Source File

SOURCE=.\cylinderdata.h
# End Source File
# Begin Source File

SOURCE=.\datatbl.h
# End Source File
# Begin Source File

SOURCE=.\inc\DbaseToTildeDelim.h
# End Source File
# Begin Source File

SOURCE=.\inc\ddxfxns.h
# End Source File
# Begin Source File

SOURCE=.\inc\dib.h
# End Source File
# Begin Source File

SOURCE=.\inc\dibpal.h
# End Source File
# Begin Source File

SOURCE=.\inc\drawlist.h
# End Source File
# Begin Source File

SOURCE=.\inc\drawutils.h
# End Source File
# Begin Source File

SOURCE=.\dtcell.h
# End Source File
# Begin Source File

SOURCE=.\dtcellset.h
# End Source File
# Begin Source File

SOURCE=.\dtcolumn.h
# End Source File
# Begin Source File

SOURCE=.\dtcolumnset.h
# End Source File
# Begin Source File

SOURCE=.\dtcurcell.h
# End Source File
# Begin Source File

SOURCE=.\dtrow.h
# End Source File
# Begin Source File

SOURCE=.\dtrowset.h
# End Source File
# Begin Source File

SOURCE=.\inc\EDITBAR.H
# End Source File
# Begin Source File

SOURCE=.\photog\ele_math.h
# End Source File
# Begin Source File

SOURCE=.\inc\elem_help.h
# End Source File
# Begin Source File

SOURCE=.\elementListview.h
# End Source File
# Begin Source File

SOURCE=.\inc\engineer.h
# End Source File
# Begin Source File

SOURCE=.\font.h
# End Source File
# Begin Source File

SOURCE=.\inc\fotobdoc.h
# End Source File
# Begin Source File

SOURCE=.\inc\fotobrow.h
# End Source File
# Begin Source File

SOURCE=.\inc\fotobvw.h
# End Source File
# Begin Source File

SOURCE=.\inc\fotogdoc.h
# End Source File
# Begin Source File

SOURCE=.\inc\fotoview.h
# End Source File
# Begin Source File

SOURCE=.\inc\generalp.h
# End Source File
# Begin Source File

SOURCE=.\gridbar.h
# End Source File
# Begin Source File

SOURCE=.\3dclass\HierElem.h
# End Source File
# Begin Source File

SOURCE=.\lightadjust.h
# End Source File
# Begin Source File

SOURCE=.\inc\listerbx.h
# End Source File
# Begin Source File

SOURCE=.\inc\mainfrm.h
# End Source File
# Begin Source File

SOURCE=.\inc\maintena.h
# End Source File
# Begin Source File

SOURCE=.\photog\mathsubs.h
# End Source File
# Begin Source File

SOURCE=.\inc\mfc_frames.h
# End Source File
# Begin Source File

SOURCE=.\inc\mymdi.h
# End Source File
# Begin Source File

SOURCE=.\openglsettings.h
# End Source File
# Begin Source File

SOURCE=.\openproj.h
# End Source File
# Begin Source File

SOURCE=.\inc\optionsd.h
# End Source File
# Begin Source File

SOURCE=.\photog\output.h
# End Source File
# Begin Source File

SOURCE=.\inc\paletteogl.h
# End Source File
# Begin Source File

SOURCE=.\inc\photog.h
# End Source File
# Begin Source File

SOURCE=.\photog\photog_math.h
# End Source File
# Begin Source File

SOURCE=.\inc\photog_msg.h
# End Source File
# Begin Source File

SOURCE=.\PointGridBar.h
# End Source File
# Begin Source File

SOURCE=.\inc\projects.h
# End Source File
# Begin Source File

SOURCE=.\3dclass\Ptg3DBoxObj.h
# End Source File
# Begin Source File

SOURCE=.\3dclass\Ptg3DCalcPointObj.h
# End Source File
# Begin Source File

SOURCE=.\3dclass\Ptg3DConeObj.h
# End Source File
# Begin Source File

SOURCE=.\3dclass\Ptg3DCtorObj.h
# End Source File
# Begin Source File

SOURCE=.\3dclass\Ptg3DCylinderObj.h
# End Source File
# Begin Source File

SOURCE=.\3dclass\Ptg3DFacetObj.h
# End Source File
# Begin Source File

SOURCE=.\3dclass\Ptg3DLineObj.h
# End Source File
# Begin Source File

SOURCE=.\3DClass\Ptg3DNode.h
# End Source File
# Begin Source File

SOURCE=.\3dclass\Ptg3DPointObj.h
# End Source File
# Begin Source File

SOURCE=.\3dclass\Ptg3DPyramidObj.h
# End Source File
# Begin Source File

SOURCE=.\3dclass\Ptg3DRtorObj.h
# End Source File
# Begin Source File

SOURCE=.\3dclass\PtgCamera.h
# End Source File
# Begin Source File

SOURCE=.\3dclass\PtgTransMatrix.h
# End Source File
# Begin Source File

SOURCE=.\3dclass\ptgworld.h
# End Source File
# Begin Source File

SOURCE=.\radiographydata.h
# End Source File
# Begin Source File

SOURCE=.\rdm_utils.h
# End Source File
# Begin Source File

SOURCE=.\inc\regfuncs.h
# End Source File
# Begin Source File

SOURCE=.\inc\resultsh.h
# End Source File
# Begin Source File

SOURCE=.\rvqlite\rvqLite.h
# End Source File
# Begin Source File

SOURCE=.\inc\SEARCHBX.H
# End Source File
# Begin Source File

SOURCE=.\inc\selecteq.h
# End Source File
# Begin Source File

SOURCE=.\inc\selectim.h
# End Source File
# Begin Source File

SOURCE=.\splash.h
# End Source File
# Begin Source File

SOURCE=.\inc\stdafx.h
# End Source File
# Begin Source File

SOURCE=.\strutils.h
# End Source File
# Begin Source File

SOURCE=.\transfor.h
# End Source File
# Begin Source File

SOURCE=.\treemenu.h
# End Source File
# Begin Source File

SOURCE=.\triton\triton.h
# End Source File
# Begin Source File

SOURCE=.\triton\triton.hpp
# End Source File
# Begin Source File

SOURCE=.\inc\tritondb.h
# End Source File
# Begin Source File

SOURCE=.\zoneeditor.h
# End Source File
# Begin Source File

SOURCE=.\inc\zoneform.h
# End Source File
# Begin Source File

SOURCE=.\inc\zonesele.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\ABD1.BMP
# End Source File
# Begin Source File

SOURCE=.\res\ABD2.BMP
# End Source File
# Begin Source File

SOURCE=.\res\ABD3.BMP
# End Source File
# Begin Source File

SOURCE=.\res\annoy_ba.bmp
# End Source File
# Begin Source File

SOURCE=.\res\backgrou.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00005.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00006.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00007.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00008.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00009.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00010.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00011.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cedthorz.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cedtvert.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cone.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cur00001.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur00002.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\equipmen.bmp
# End Source File
# Begin Source File

SOURCE=.\res\fotobdoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\fotobrow.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_elem.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_imag.bmp
# End Source File
# Begin Source File

SOURCE=.\res\image.bmp
# End Source File
# Begin Source File

SOURCE=.\res\medthorz.bmp
# End Source File
# Begin Source File

SOURCE=.\res\medtvert.bmp
# End Source File
# Begin Source File

SOURCE=.\res\node.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ogl_blac.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ogl_cyan.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ogl_gree.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ogl_grey.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ogl_lt_b.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ogl_lt_c.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ogl_lt_g.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ogl_lt_r.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ogl_lt_w.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ogl_purp.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ogl_red.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PM3LLOGO.BMP
# End Source File
# Begin Source File

SOURCE=.\res\pms_symb.bmp
# End Source File
# Begin Source File

SOURCE=.\res\small_ma.ico
# End Source File
# Begin Source File

SOURCE=.\Splsh16.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar.bmp
# End Source File
# End Group
# End Target
# End Project
# Section FOTOBROW : {2CBBFC20-1DE4-11CF-8E0A-484F4DF914E6}
# 	2:5:Class:CDTCell
# 	2:10:HeaderFile:dtcell.h
# 	2:8:ImplFile:dtcell.cpp
# End Section
# Section FOTOBROW : {BEF6E003-A874-101A-8BBA-00AA00300CAB}
# 	2:5:Class:COleFont
# 	2:10:HeaderFile:font.h
# 	2:8:ImplFile:font.cpp
# End Section
# Section FOTOBROW : {AA352000-1F65-11CF-8E0A-484F4DF914E6}
# 	2:5:Class:CDTRow
# 	2:10:HeaderFile:dtrow.h
# 	2:8:ImplFile:dtrow.cpp
# End Section
# Section FOTOBROW : {4C1DF001-D3FF-11CE-84BA-484F4DF914E6}
# 	2:5:Class:CDatatbl
# 	2:10:HeaderFile:datatbl.h
# 	2:8:ImplFile:datatbl.cpp
# End Section
# Section FOTOBROW : {FC816DA0-2320-11CF-8E0A-484F4DF914E6}
# 	2:5:Class:CDTCellSet
# 	2:10:HeaderFile:dtcellset.h
# 	2:8:ImplFile:dtcellset.cpp
# End Section
# Section FOTOBROW : {D75EB420-231F-11CF-8E0A-484F4DF914E6}
# 	2:5:Class:CDTCurCell
# 	2:10:HeaderFile:dtcurcell.h
# 	2:8:ImplFile:dtcurcell.cpp
# End Section
# Section FOTOBROW : {4C1DF000-D3FF-11CE-84BA-484F4DF914E6}
# 	0:8:Font.cpp:C:\busbyj\dev\Pm3 Lite\Font.cpp
# 	0:10:DTCell.cpp:C:\busbyj\dev\Pm3 Lite\DTCell.cpp
# 	0:9:DTRow.cpp:C:\busbyj\dev\Pm3 Lite\DTRow.cpp
# 	0:13:DTCurCell.cpp:C:\busbyj\dev\Pm3 Lite\DTCurCell.cpp
# 	0:8:DTCell.h:C:\busbyj\dev\Pm3 Lite\DTCell.h
# 	0:6:Font.h:C:\busbyj\dev\Pm3 Lite\Font.h
# 	0:11:DTCellSet.h:C:\busbyj\dev\Pm3 Lite\DTCellSet.h
# 	0:10:DTColumn.h:C:\busbyj\dev\Pm3 Lite\DTColumn.h
# 	0:13:DTColumnSet.h:C:\busbyj\dev\Pm3 Lite\DTColumnSet.h
# 	0:12:DTRowSet.cpp:C:\busbyj\dev\Pm3 Lite\DTRowSet.cpp
# 	0:13:DTCellSet.cpp:C:\busbyj\dev\Pm3 Lite\DTCellSet.cpp
# 	0:7:DTRow.h:C:\busbyj\dev\Pm3 Lite\DTRow.h
# 	0:12:DTColumn.cpp:C:\busbyj\dev\Pm3 Lite\DTColumn.cpp
# 	0:9:Datatbl.h:C:\busbyj\dev\Pm3 Lite\Datatbl.h
# 	0:11:DTCurCell.h:C:\busbyj\dev\Pm3 Lite\DTCurCell.h
# 	0:15:DTColumnSet.cpp:C:\busbyj\dev\Pm3 Lite\DTColumnSet.cpp
# 	0:10:DTRowSet.h:C:\busbyj\dev\Pm3 Lite\DTRowSet.h
# 	0:11:Datatbl.cpp:C:\busbyj\dev\Pm3 Lite\Datatbl.cpp
# 	2:21:DefaultSinkHeaderFile:datatbl.h
# 	2:16:DefaultSinkClass:CDatatbl
# End Section
# Section FOTOBROW : {2CBBFC22-1DE4-11CF-8E0A-484F4DF914E6}
# 	2:5:Class:CDTColumn
# 	2:10:HeaderFile:dtcolumn.h
# 	2:8:ImplFile:dtcolumn.cpp
# End Section
# Section OLE Controls
# 	{4C1DF000-D3FF-11CE-84BA-484F4DF914E6}
# End Section
# Section FOTOBROW : {0DD3D1C0-2028-11CF-8E0A-484F4DF914E6}
# 	2:5:Class:CDTColumnSet
# 	2:10:HeaderFile:dtcolumnset.h
# 	2:8:ImplFile:dtcolumnset.cpp
# End Section
# Section FOTOBROW : {007B9DC0-2044-11CF-8E0A-484F4DF914E6}
# 	2:5:Class:CDTRowSet
# 	2:10:HeaderFile:dtrowset.h
# 	2:8:ImplFile:dtrowset.cpp
# End Section
