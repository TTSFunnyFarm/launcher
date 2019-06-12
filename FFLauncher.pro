#-------------------------------------------------
#
# Project created by QtCreator 2019-06-04T13:50:42
#
#-------------------------------------------------

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT += network concurrent

TARGET = FFLauncher
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

INCLUDEPATH += \
    $$PWD/dependencies/bzip2/include

SOURCES += \
    src/core/main.cpp \
    src/ui/LauncherWindow.cpp \
    src/updater/Updater.cpp

HEADERS += \
    src/core/Constants.h \
    src/core/Localizer.h \
    src/ui/FramelessWindow.h \
    src/ui/LauncherWindow.h \
    src/updater/Updater.h

macx {
    # Only include / compile these files on macOS:
    OBJECTIVE_SOURCES += \
        src/ui/FramelessWindow.mm

    # Include Cocoa for macOS code:
    LIBS += \
        -framework Cocoa

    FORMS += \
        FFLauncher-macx.ui

    RESOURCES += \
        resources-macx.qrc
} win32 {
    CONFIG(debug, debug|release) {
        BUILD_DIR = $$OUT_PWD/debug
    } else {
        BUILD_DIR = $$OUT_PWD/release
    }

    INCLUDEPATH += \
        $$PWD/dependencies/openssl/include

    contains(QT_ARCH, i386) {
        LIBS += \
            $$PWD/dependencies/bzip2/Win32/lib/libbz2.lib \
            $$PWD/dependencies/openssl/Win32/lib/libeay32.lib \
            $$PWD/dependencies/openssl/Win32/lib/ssleay32.lib

        copydata.commands = \
            $(COPY_DIR) $$shell_path($$PWD/dependencies/openssl/Win32/bin) $$shell_path($$BUILD_DIR)
    } else {
        LIBS += \
            $$PWD/dependencies/bzip2/Win64/lib/libbz2.lib \
            $$PWD/dependencies/openssl/Win64/lib/libeay32.lib \
            $$PWD/dependencies/openssl/Win64/lib/ssleay32.lib

        copydata.commands = \
            $(COPY_DIR) $$shell_path($$PWD/dependencies/openssl/Win64/bin) $$shell_path($$BUILD_DIR)
    }

    SOURCES += \
        src/ui/FramelessWindow.cpp

    FORMS += \
        FFLauncher-win32.ui

    RESOURCES += \
        resources-win32.qrc

    RC_ICONS += \
        assets/icon.ico

    first.depends = \
        $(first) \
        copydata

    export(first.depends)
    export(copydata.commands)

    QMAKE_EXTRA_TARGETS += \
        first \
        copydata
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
