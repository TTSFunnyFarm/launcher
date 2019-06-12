#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>

const QString VERSION = "1.0.0";
const QString USER_AGENT = "FF-Launcher/" + VERSION;
const QString MANIFEST_FILENAME = "manifest.json";

#ifdef WIN32
const QString URL_DOWNLOAD_MIRROR = "https://cdn.toontownsfunnyfarm.com/windows/";
#elif __linux
const QString URL_DOWNLOAD_MIRROR = "https://cdn.toontownsfunnyfarm.com/linux/";
#else
const QString URL_DOWNLOAD_MIRROR = "https://cdn.toontownsfunnyfarm.com/mac/";
#endif

const QString URL_WEBSITE = "https://www.toontownsfunnyfarm.com/";
const QString URL_DISCORD = "https://discord.gg/BFC7agB";

const int ERROR_CODE_WRITE = 902;
const int ERROR_CODE_READ = 903;
const int ERROR_CODE_EXTRACT = 904;

#endif // CONSTANTS_H
