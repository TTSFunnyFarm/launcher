; Toontown's Funny Farm Launcher (win64) installation script for the Nullsoft Installation System (NSIS).
; by John Cote (jwcotejr)
; created on 4-Dec-2019

; Helper defines
!define PRODUCT_NAME "Toontown's Funny Farm"
!define PRODUCT_VERSION "2.0.0"
!define PRODUCT_PUBLISHER "Toontown's Funny Farm"
!define PRODUCT_WEB_SITE "https://www.toontownsfunnyfarm.com/"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\FFLauncher.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"
!define PRODUCT_STARTMENU_REGVAL "NSIS:StartMenuDir"

; Require Administrator privileges
RequestExecutionLevel admin

; Use LZMA compression
SetCompressor lzma

; MUI 2.1 compatible ------
!include "MUI2.nsh"

; 64-bit support
!include "x64.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_WELCOMEFINISHPAGE_BITMAP "funny-farm-install.bmp"
!define MUI_UNWELCOMEFINISHPAGE_BITMAP "funny-farm-install.bmp"
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP "header.bmp"
!define MUI_HEADERIMAGE_RIGHT

; Welcome page
!insertmacro MUI_PAGE_WELCOME
; Directory page
!insertmacro MUI_PAGE_DIRECTORY
; Start menu page
var ICONS_GROUP
!define MUI_STARTMENUPAGE_NODISABLE
!define MUI_STARTMENUPAGE_DEFAULTFOLDER "Toontown's Funny Farm"
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "${PRODUCT_UNINST_ROOT_KEY}"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "${PRODUCT_UNINST_KEY}"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "${PRODUCT_STARTMENU_REGVAL}"
!insertmacro MUI_PAGE_STARTMENU Application $ICONS_GROUP
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
!define MUI_FINISHPAGE_RUN "$INSTDIR\FFLauncher.exe"
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES

; Language files
!insertmacro MUI_LANGUAGE "English"

; Reserve files
ReserveFile "${NSISDIR}\Plugins\x86-ansi\InstallOptions.dll"

; MUI end ------

Function .onInit
  ${If} ${RunningX64}
  SetRegView 64
  ${else}
  MessageBox MB_OK "This application can only run on 64-bit Windows."
  Abort
  ${EndIf}
FunctionEnd

Name "${PRODUCT_NAME}"
OutFile "launcher-win64-setup.exe"
InstallDir "$PROGRAMFILES64\Toontown's Funny Farm"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show

Section "MainSection" SEC01
  SetOutPath "$INSTDIR"
  SetOverwrite ifnewer
  File "launcher-win64\FFLauncher.exe"
  File "launcher-win64\Qt5Widgets.dll"
  File "launcher-win64\Qt5Network.dll"
  File "launcher-win64\Qt5Gui.dll"
  File "launcher-win64\Qt5Core.dll"
  File "launcher-win64\libcrypto-1_1-x64.dll"
  File "launcher-win64\libssl-1_1-x64.dll"
  SetOutPath "$INSTDIR\platforms"
  File "launcher-win64\platforms\qwindows.dll"
  SetOutPath "$INSTDIR\styles"
  File "launcher-win64\styles\qwindowsvistastyle.dll"

; Shortcuts
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
  CreateDirectory "$SMPROGRAMS\$ICONS_GROUP"
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\Toontown's Funny Farm.lnk" "$INSTDIR\FFLauncher.exe"
  CreateShortCut "$DESKTOP\Toontown's Funny Farm.lnk" "$INSTDIR\FFLauncher.exe"
  !insertmacro MUI_STARTMENU_WRITE_END
SectionEnd

Section -AdditionalIcons
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
  WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\Website.lnk" "$INSTDIR\${PRODUCT_NAME}.url"
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\Uninstall.lnk" "$INSTDIR\uninst.exe"
  !insertmacro MUI_STARTMENU_WRITE_END
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\FFLauncher.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\FFLauncher.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd


Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) was successfully removed from your computer."
FunctionEnd

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Are you sure you want to completely remove $(^Name) and all of its components?" IDYES +2
  Abort
FunctionEnd

Section Uninstall
  !insertmacro MUI_STARTMENU_GETFOLDER "Application" $ICONS_GROUP
  Delete "$INSTDIR\${PRODUCT_NAME}.url"
  Delete "$INSTDIR\uninst.exe"
  Delete "$INSTDIR\platforms\qwindows.dll"
  Delete "$INSTDIR\styles\qwindowsvistastyle.dll"
  Delete "$INSTDIR\Qt5Core.dll"
  Delete "$INSTDIR\Qt5Gui.dll"
  Delete "$INSTDIR\Qt5Network.dll"
  Delete "$INSTDIR\Qt5Widgets.dll"
  Delete "$INSTDIR\libcrypto-1_1-x64.dll"
  Delete "$INSTDIR\libssl-1_1-x64.dll"
  Delete "$INSTDIR\FFLauncher.exe"

  Delete "$SMPROGRAMS\$ICONS_GROUP\Uninstall.lnk"
  Delete "$SMPROGRAMS\$ICONS_GROUP\Website.lnk"
  Delete "$DESKTOP\Toontown's Funny Farm.lnk"
  Delete "$SMPROGRAMS\$ICONS_GROUP\Toontown's Funny Farm.lnk"

  RMDir "$SMPROGRAMS\$ICONS_GROUP"
  RMDir "$INSTDIR\platforms"
  RMDir "$INSTDIR\styles"
  RMDir "$INSTDIR"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"

  ;SetAutoClose true
SectionEnd
