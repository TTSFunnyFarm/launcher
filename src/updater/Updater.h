#ifndef UPDATER_H
#define UPDATER_H

#include "src/core/Constants.h"

#include <exception>
#include <vector>
#include <queue>
#include <cstdint>

#include <QtConcurrent>
#include <QException>
#include <QObject>
#include <QUrl>
#include <QString>
#include <QFile>
#include <QNetworkReply>
#include <QTime>
#include <QByteArray>

class DownloadError : public std::runtime_error
{
public:
    DownloadError(int error_code, const QString &error_string) :
        std::runtime_error(error_string.toStdString()),
        m_error_code(error_code)
    {
    }

    int get_error_code()
    {
        return m_error_code;
    }

private:
    int m_error_code;
};

class DownloadThreadError : public QException
{
public:
    DownloadThreadError(DownloadError &error) : e(error)
    {
    }

    void raise() const
    {
        throw *this;
    }

    DownloadThreadError *clone() const
    {
        return new DownloadThreadError(*this);
    }

    DownloadError error() const
    {
        return e;
    }

private:
    DownloadError e;
};

class Updater : public QObject
{
    Q_OBJECT

public:
    Updater(QUrl url);
    ~Updater();

    QString get_launcher_version();
    QString get_game_version();

    void update_manifest(const QString &filename = MANIFEST_FILENAME);
    void parse_manifest_data(const QByteArray &data);
    bool update();

signals:
    void download_error(int error_code, const QString &error_string);
    void download_progress(qint64 bytes_read, qint64 bytes_total, const QString &status);
    void extract_finished();

private slots:
    void readyRead();
    void finished();
    void downloadProgress(qint64 bytes_read, qint64 bytes_total);

private:
    QUrl m_url;

    QString m_launcher_version;
    QString m_game_version;

    std::queue<QString> m_files;

    size_t m_update_file_number;
    size_t m_update_file_total;

    QFile *m_download_file;
    QNetworkReply *m_download_reply;
    QTime m_download_time;

    void download_file(const QString &relative_path);
    void extract_file(const QString &archive_path, const QString &output_path);
};

#endif // UPDATER_H
