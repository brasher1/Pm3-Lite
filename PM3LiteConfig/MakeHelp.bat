@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by PM3LITECONFIG.HPJ. >"hlp\PM3LiteConfig.hm"
echo. >>"hlp\PM3LiteConfig.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\PM3LiteConfig.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\PM3LiteConfig.hm"
echo. >>"hlp\PM3LiteConfig.hm"
echo // Prompts (IDP_*) >>"hlp\PM3LiteConfig.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\PM3LiteConfig.hm"
echo. >>"hlp\PM3LiteConfig.hm"
echo // Resources (IDR_*) >>"hlp\PM3LiteConfig.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\PM3LiteConfig.hm"
echo. >>"hlp\PM3LiteConfig.hm"
echo // Dialogs (IDD_*) >>"hlp\PM3LiteConfig.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\PM3LiteConfig.hm"
echo. >>"hlp\PM3LiteConfig.hm"
echo // Frame Controls (IDW_*) >>"hlp\PM3LiteConfig.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\PM3LiteConfig.hm"
REM -- Make help for Project PM3LITECONFIG


echo Building Win32 Help files
start /wait hcrtf -x "hlp\PM3LiteConfig.hpj"
echo.
if exist Debug\nul copy "hlp\PM3LiteConfig.hlp" Debug
if exist Debug\nul copy "hlp\PM3LiteConfig.cnt" Debug
if exist Release\nul copy "hlp\PM3LiteConfig.hlp" Release
if exist Release\nul copy "hlp\PM3LiteConfig.cnt" Release
echo.


