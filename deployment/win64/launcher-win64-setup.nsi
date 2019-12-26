; Toontown's Funny Farm Launcher (win64) installation script for the Nullsoft Installation System (NSIS).
; by John Cote (jwcotejr)
; created on 4-Dec-2019

; Helper defines
!define PRODUCT_NAME "Toontown's Funny Farm"
!define PRODUCT_VERSION "2.0.0"
!define PRODUCT_PUBLISHER "The Toontown's Funny Farm Team"
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

; For checking Windows version
!include WinVer.nsh

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
; Components page
!insertmacro MUI_PAGE_COMPONENTS
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
  ${IfNot} ${AtLeastWin7}
    MessageBox MB_OK "This application requires Windows 7 or newer."
    Abort
  ${EndIf}

  ${If} ${RunningX64}
    ${DisableX64FSRedirection}
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

Section "Toontown's Funny Farm" SEC01
  SectionIn RO
  SetOutPath "$INSTDIR"
  SetOverwrite ifnewer
  File "launcher-win64\FFLauncher.exe"
  File "launcher-win64\Qt5Widgets.dll"
  File "launcher-win64\Qt5Network.dll"
  File "launcher-win64\Qt5Gui.dll"
  File "launcher-win64\Qt5Core.dll"
  File "launcher-win64\libcrypto-1_1-x64.dll"
  File "launcher-win64\libssl-1_1-x64.dll"
  File "launcher-win32\VC_redist.x64.exe"
  File "launcher-win32\VC_redist.x86.exe"
  SetOutPath "$INSTDIR\platforms"
  File "launcher-win64\platforms\qwindows.dll"
  SetOutPath "$INSTDIR\styles"
  File "launcher-win64\styles\qwindowsvistastyle.dll"
SectionEnd

Section "Start Menu Shortcuts" SEC02
  CreateDirectory "$SMPROGRAMS\${PRODUCT_NAME}"
  CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\Toontown's Funny Farm.lnk" "$INSTDIR\FFLauncher.exe"
  WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
  CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\Toontown's Funny Farm Official Website.lnk" "$INSTDIR\${PRODUCT_NAME}.url"
  CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\Uninstall.lnk" "$INSTDIR\uninst.exe"
SectionEnd

Section "Desktop Shortcut" SEC03
  CreateShortCut "$DESKTOP\Toontown's Funny Farm.lnk" "$INSTDIR\FFLauncher.exe"
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

  ; Install Visual C++ Redistributable for Visual Studio 2015-2019 (x64), if needed
  ReadRegStr $1 HKLM "SOFTWARE\Microsoft\VisualStudio\14.0\VC\Runtimes\X64" "Installed"
  StrCmp $1 1 x64Installed

  ExecWait "$INSTDIR\VC_redist.x64.exe /install /passive /norestart"

  x64Installed:

  ReadRegStr $1 HKLM "SOFTWARE\Microsoft\VisualStudio\14.0\VC\Runtimes\X64" "Version"
  StrCmp $1 "v14.24.28127.04" x64UpToDate

  ExecWait "$INSTDIR\VC_redist.x64.exe /install /passive /norestart"

  x64UpToDate:

  Delete "$INSTDIR\VC_redist.x64.exe"

  ; Install Visual C++ Redistributable for Visual Studio 2015-2019 (x86), if needed
  ReadRegStr $1 HKLM "SOFTWARE\WOW6432Node\Microsoft\VisualStudio\14.0\VC\Runtimes\X86" "Installed"
  StrCmp $1 1 x86Installed

  ExecWait "$INSTDIR\VC_redist.x86.exe /install /passive /norestart"

  x86Installed:

  ReadRegStr $1 HKLM "SOFTWARE\WOW6432Node\Microsoft\VisualStudio\14.0\VC\Runtimes\X86" "Version"
  StrCmp $1 "v14.24.28127.04" x86UpToDate

  ExecWait "$INSTDIR\VC_redist.x86.exe /install /passive /norestart"

  x86UpToDate:

  Delete "$INSTDIR\VC_redist.x86.exe"
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

  Delete "$APPDATA\Local\Toontown's Funny Farm\win64\cg.dll"
  Delete "$APPDATA\Local\Toontown's Funny Farm\win64\cgGL.dll"
  Delete "$APPDATA\Local\Toontown's Funny Farm\win64\funnyfarm.exe"
  Delete "$APPDATA\Local\Toontown's Funny Farm\win64\libcrypto-1_1.dll"
  Delete "$APPDATA\Local\Toontown's Funny Farm\win64\libp3direct.dll"
  Delete "$APPDATA\Local\Toontown's Funny Farm\win64\libp3dtool.dll"
  Delete "$APPDATA\Local\Toontown's Funny Farm\win64\libp3dtoolconfig.dll"
  Delete "$APPDATA\Local\Toontown's Funny Farm\win64\libp3interrogatedb.dll"
  Delete "$APPDATA\Local\Toontown's Funny Farm\win64\libp3openal_audio.dll"
  Delete "$APPDATA\Local\Toontown's Funny Farm\win64\libp3windisplay.dll"
  Delete "$APPDATA\Local\Toontown's Funny Farm\win64\libpanda.dll"
  Delete "$APPDATA\Local\Toontown's Funny Farm\win64\libpandaexpress.dll"
  Delete "$APPDATA\Local\Toontown's Funny Farm\win64\libpandagl.dll"
  Delete "$APPDATA\Local\Toontown's Funny Farm\win64\libpandaphysics.dll"
  Delete "$APPDATA\Local\Toontown's Funny Farm\win64\libssl-1_1.dll"
  Delete "$APPDATA\Local\Toontown's Funny Farm\win64\python37.dll"
  Delete "$APPDATA\Local\Toontown's Funny Farm\win64\select.pyd"
  Delete "$APPDATA\Local\Toontown's Funny Farm\win64\_cffi_backend.pyd"
  Delete "$APPDATA\Local\Toontown's Funny Farm\win64\_socket.pyd"
  Delete "$APPDATA\Local\Toontown's Funny Farm\win64\_ssl.pyd"
  Delete "$APPDATA\Local\Toontown's Funny Farm\win64\resources\phase_3.5.mf"
  Delete "$APPDATA\Local\Toontown's Funny Farm\win64\resources\phase_3.mf"
  Delete "$APPDATA\Local\Toontown's Funny Farm\win64\resources\phase_4.mf"
  Delete "$APPDATA\Local\Toontown's Funny Farm\win64\resources\phase_5.5.mf"
  Delete "$APPDATA\Local\Toontown's Funny Farm\win64\resources\phase_5.mf"
  Delete "$APPDATA\Local\Toontown's Funny Farm\win64\resources\phase_6.mf"
  Delete "$APPDATA\Local\Toontown's Funny Farm\win64\resources\phase_7.mf"
  Delete "$APPDATA\Local\Toontown's Funny Farm\win64\resources\phase_8.mf"
  Delete "$APPDATA\Local\Toontown's Funny Farm\win64\resources\phase_9.mf"
  Delete "$APPDATA\Local\Toontown's Funny Farm\win64\resources\phase_10.mf"
  Delete "$APPDATA\Local\Toontown's Funny Farm\win64\resources\phase_11.mf"
  Delete "$APPDATA\Local\Toontown's Funny Farm\win64\resources\phase_12.mf"
  Delete "$APPDATA\Local\Toontown's Funny Farm\win64\resources\phase_13.mf"
  Delete "$APPDATA\Local\Toontown's Funny Farm\win64\resources\phase_14.mf"
  Delete "$APPDATA\Local\Toontown's Funny Farm\win64\panda3d\core.pyd"
  Delete "$APPDATA\Local\Toontown's Funny Farm\win64\panda3d\direct.pyd"
  Delete "$APPDATA\Local\Toontown's Funny Farm\win64\panda3d\physics.pyd"
  Delete "$APPDATA\Local\Toontown's Funny Farm\win64\cryptography\hazmat\bindings\_constant_time.pyd"
  Delete "$APPDATA\Local\Toontown's Funny Farm\win64\cryptography\hazmat\bindings\_openssl.pyd"
  Delete "$APPDATA\Local\Toontown's Funny Farm\win64\cryptography\hazmat\bindings\_padding.pyd"

  Delete "$SMPROGRAMS\${PRODUCT_NAME}\Uninstall.lnk"
  Delete "$SMPROGRAMS\${PRODUCT_NAME}\Toontown's Funny Farm Official Website.lnk"
  Delete "$DESKTOP\Toontown's Funny Farm.lnk"
  Delete "$SMPROGRAMS\${PRODUCT_NAME}\Toontown's Funny Farm.lnk"

  RMDir "$SMPROGRAMS\${PRODUCT_NAME}"
  RMDir "$INSTDIR\platforms"
  RMDir "$INSTDIR\styles"
  RMDir "$INSTDIR"
  RMDir "$APPDATA\Local\Toontown's Funny Farm\win64\resources"
  RMDir "$APPDATA\Local\Toontown's Funny Farm\win64\panda3d"
  RMDir "$APPDATA\Local\Toontown's Funny Farm\win64\cryptography\hazmat\bindings"
  RMDir "$APPDATA\Local\Toontown's Funny Farm\win64\cryptography\hazmat"
  RMDir "$APPDATA\Local\Toontown's Funny Farm\win64\cryptography"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"

  ;SetAutoClose true
SectionEnd
