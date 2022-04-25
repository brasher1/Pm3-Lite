@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by FOTOBROW.HPJ. >hlp\fotobrow.hm
echo. >>hlp\fotobrow.hm
echo // Commands (ID_* and IDM_*) >>hlp\fotobrow.hm
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>hlp\fotobrow.hm
echo. >>hlp\fotobrow.hm
echo // Prompts (IDP_*) >>hlp\fotobrow.hm
makehm IDP_,HIDP_,0x30000 resource.h >>hlp\fotobrow.hm
echo // Prompts (IDP_*) >>hlp\fotobrow.hm
makehm ID_,HIDP_,0x30000 resource.h >>hlp\fotobrow.hm
echo. >>hlp\fotobrow.hm
echo // Resources (IDR_*) >>hlp\fotobrow.hm
makehm IDR_,HIDR_,0x20000 resource.h >>hlp\fotobrow.hm
echo. >>hlp\fotobrow.hm
echo // Dialogs (IDD_*) >>hlp\fotobrow.hm
makehm IDD_,HIDD_,0x20000 resource.h >>hlp\fotobrow.hm
echo. >>hlp\fotobrow.hm
echo // Frame Controls (IDW_*) >>hlp\fotobrow.hm
makehm IDW_,HIDW_,0x50000 resource.h >>hlp\fotobrow.hm
echo. >>hlp\fotobrow.hm
echo // Frame Controls (IDW_*) >>hlp\fotobrow.hm
makehm IDC_,HIDC_,0x10000 resource.h >>hlp\fotobrow.hm

REM -- Make help for Project FOTOBROW

echo Building Win32 Help files
call hc31 fotobrow.hpj
if exist windebug copy fotobrow.hlp windebug
if exist winrel copy fotobrow.hlp winrel
EndLocal
