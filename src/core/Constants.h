#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>

const QString VERSION = "2.0.0";
const QString USER_AGENT = "FF-Launcher/" + VERSION;
const QString MANIFEST_FILENAME = "manifest.json";

#if defined(Q_OS_WIN) && defined(Q_PROCESSOR_X86_32)
const QString URL_DOWNLOAD_MIRROR = "https://cdn.toontownsfunnyfarm.com/win32/";
#elif defined(Q_OS_WIN) && defined(Q_PROCESSOR_X86_64)
const QString URL_DOWNLOAD_MIRROR = "https://cdn.toontownsfunnyfarm.com/win64/";
#elif defined(Q_OS_MAC)
const QString URL_DOWNLOAD_MIRROR = "https://cdn.toontownsfunnyfarm.com/mac/";
#else  // Assume Linux
const QString URL_DOWNLOAD_MIRROR = "https://cdn.toontownsfunnyfarm.com/linux/";
#endif

const QString URL_PLAY = "https://www.toontownsfunnyfarm.com/play";
const QString URL_WEBSITE = "https://www.toontownsfunnyfarm.com/";
const QString URL_DISCORD = "https://discord.gg/toontownsfunnyfarm";

const int ERROR_CODE_WRITE = 902;
const int ERROR_CODE_READ = 903;
const int ERROR_CODE_EXTRACT = 904;

#endif // CONSTANTS_H
