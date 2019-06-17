#ifndef LOCALIZER_H
#define LOCALIZER_H

#include <QString>

const QString ERROR_OUT_OF_DATE_TITLE = "Launcher Update";
const QString ERROR_OUT_OF_DATE_TEXT = "Your launcher appears to be out of date. Click 'OK' to download the new version!";
const QString ERROR_WRITE = "Couldn't open file for write: %1";
const QString ERROR_READ = "Couldn't open file for read: %1";
const QString ERROR_EXTRACT = "Couldn't extract archive: %1";

const QString GUI_LAUNCHER_VERSION = "Launcher: %1";
const QString GUI_GAME_VERSION = "Game: %1";
const QString GUI_NO_VERSION = "Unknown";
const QString GUI_CHECKING_FOR_UPDATES = "Checking for updates...";
const QString GUI_DOWNLOAD_WAITING = "Updating file %1 of %2...\n[%3/%4%5 @ %6%7]";

#endif // LOCALIZER_H
