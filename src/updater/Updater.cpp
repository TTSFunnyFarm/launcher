#include "Updater.h"

#include "src/core/Constants.h"
#include "src/core/Localizer.h"

#include <bzlib.h>

Updater::Updater(QUrl url) :
    QObject(),
    m_url(url),
    m_launcher_version(""),
    m_game_version(""),
    m_update_file_number(0),
    m_update_file_total(0)
{
}

Updater::~Updater()
{
    if (m_download_reply)
    {
        delete m_download_reply;
        m_download_reply = nullptr;
    }

    if (m_download_file)
    {
        m_download_file->close();
        delete m_download_file;
        m_download_file = nullptr;
    }
}

QString Updater::get_launcher_version()
{
    return m_launcher_version;
}

QString Updater::get_game_version()
{
    return m_game_version;
}

void Updater::update_manifest(const QString &filename)
{
    // Clear the data from the previous manifest:
    m_game_version = "";
    m_launcher_version = "";

    QNetworkRequest request(QUrl(m_url.toString() + filename));
    request.setHeader(QNetworkRequest::UserAgentHeader, USER_AGENT);
    QNetworkAccessManager access_manager;
    QNetworkReply *reply = access_manager.get(request);

    // Block until the network request is finished:
    QEventLoop event_loop;
    QObject::connect(&access_manager, SIGNAL(finished(QNetworkReply *)),
                     &event_loop, SLOT(quit()));
    event_loop.exec();

    if (reply->error() == QNetworkReply::NoError)
    {
        this->parse_manifest_data(reply->readAll());
    }

    delete reply;
}

void Updater::parse_manifest_data(const QByteArray &data)
{
    QJsonDocument document = QJsonDocument::fromJson(data);
    QJsonObject object = document.object();
    if (object.contains("game-version"))
    {
        m_game_version = object["game-version"].toString();
    }

    if (object.contains("launcher-version"))
    {
        m_launcher_version = object["launcher-version"].toString();
    }

    if (object.contains("files"))
    {
        m_files = {}; // Clear m_files
        QJsonObject files = object["files"].toObject();
        QStringList filenames = files.keys();
        for (auto filename : filenames)
        {
            QDir directory(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));

            QString hash = files[filename].toObject()["hash"].toString();
            QString path = files[filename].toObject()["path"].toString();

            if (!path.isEmpty())
            {
                directory.mkpath(path);
            }

            QString absolute_path = directory.absoluteFilePath(filename);
            QFile file(absolute_path);
            if (!file.open(QIODevice::ReadOnly))
            {
                m_files.push(filename);
                continue;
            }

            QCryptographicHash hashval(QCryptographicHash::Md5);
            qint64 bytes_read;
            char buffer[8192];
            while ((bytes_read = file.read(buffer, 8192)) > 0)
            {
                hashval.addData(buffer, bytes_read);
            }

            QString hash_result(hashval.result().toHex());
            if (hash_result != hash)
            {
                m_files.push(filename);
            }

            file.close();
        }
    }
}

bool Updater::update()
{
    m_update_file_total = m_files.size();
    if (!m_update_file_total)
    {
        return true;
    }

    for (m_update_file_number = 1; !m_files.empty(); m_update_file_number++)
    {
        QString relative_path = m_files.front();
        QString archive_path = relative_path + ".bz2";

        try
        {
            this->download_file(archive_path);
        }
        catch (DownloadError &e)
        {
            emit download_error(e.get_error_code(), e.what());
            return false;
        }

        try
        {
            QFuture<void> future = QtConcurrent::run(
                this, &Updater::extract_file, archive_path, relative_path);
            QEventLoop event_loop;
            QObject::connect(this, SIGNAL(extract_finished()),
                             &event_loop, SLOT(quit()));
            event_loop.exec();
            future.waitForFinished();
        }
        catch (DownloadThreadError &e)
        {
            emit download_error(e.error().get_error_code(), e.error().what());
            return false;
        }

        m_files.pop();
    }

    return true;
}

void Updater::download_file(const QString &relative_path)
{
    QString relative = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    relative.append("/" + relative_path);

    m_download_file = new QFile(relative);
    if (!m_download_file->open(QIODevice::WriteOnly))
    {
        delete m_download_file;
        m_download_file = nullptr;

        QString file_name = QFileInfo(relative).fileName();
        throw DownloadError(ERROR_CODE_WRITE, ERROR_WRITE.arg(file_name));
    }

    QNetworkRequest request(QUrl(m_url.toString() + relative_path));
    request.setHeader(QNetworkRequest::UserAgentHeader, USER_AGENT);
    QNetworkAccessManager access_manager;
    m_download_reply = access_manager.get(request);

    m_download_time.start();

    QObject::connect(m_download_reply, SIGNAL(readyRead()),
                     this, SLOT(readyRead()));
    QObject::connect(m_download_reply, SIGNAL(finished()),
                     this, SLOT(finished()));
    QObject::connect(m_download_reply, SIGNAL(downloadProgress(qint64, qint64)),
                     this, SLOT(downloadProgress(qint64, qint64)));

    // Block until the network request is finished:
    QEventLoop event_loop;
    QObject::connect(m_download_reply, SIGNAL(finished()),
                     &event_loop, SLOT(quit()));
    event_loop.exec();

    if (m_download_reply)
    {
        delete m_download_reply;
        m_download_reply = nullptr;
    }
}

void Updater::readyRead()
{
    if (m_download_file && m_download_reply)
    {
        m_download_file->write(m_download_reply->readAll());
    }
}

void Updater::finished()
{
    if (m_download_file)
    {
        m_download_file->close();
        delete m_download_file;
        m_download_file = nullptr;
    }
}

void Updater::downloadProgress(qint64 bytes_read, qint64 bytes_total)
{
    // Choose the unit:
    QString unit = "B";
    double read = bytes_read;
    double total = bytes_total;
    if (bytes_total >= 1024)
    {
        if (bytes_total < (1024 * 1024))
        {
            read /= 1024;
            total /= 1024;
            unit = "kB";
        }
        else
        {
            read /= 1024 * 1024;
            total /= 1024 * 1024;
            unit = "MB";
        }
    }

    // Round the values to the nearest tenth:
    read = floor((read * 10.0) + 0.5) / 10.0;
    total = floor((total * 10.0) + 0.5) / 10.0;

    // Calculate the download speed:
    double speed = (bytes_read * 1000.0) / m_download_time.elapsed();

    // Choose the speed unit:
    QString speed_unit = "B/s";
    if (speed >= 1024)
    {
        if (speed < (1024 * 1024))
        {
            speed /= 1024;
            speed_unit = "kB/s";
        }
        else
        {
            speed /= 1024 * 1024;
            speed_unit = "MB/s";
        }
    }

    // Round the value to the nearest tenth:
    speed = floor((speed * 10.0) + 0.5) / 10.0;

    emit this->download_progress(bytes_read, bytes_total,
        GUI_DOWNLOAD_WAITING.arg(
            QString::number(m_update_file_number),
            QString::number(m_update_file_total), QString::number(read),
            QString::number(total), unit, QString::number(speed), speed_unit));
}

void Updater::extract_file(const QString &archive_path, const QString &output_path)
{
    QString archive = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    archive.append("/" + archive_path);

    QString output = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    output.append("/" + output_path);

    FILE *f = fopen(archive.toStdString().c_str(), "rb");
    if (f == nullptr)
    {
        emit this->extract_finished();
        try
        {
            QString file_name = QFileInfo(archive).fileName();
            throw DownloadError(ERROR_CODE_READ, ERROR_READ.arg(file_name));
        }
        catch (DownloadError &e)
        {
            throw DownloadThreadError(e);
        }
    }

    int bzerror;
    BZFILE *archive_file = BZ2_bzReadOpen(&bzerror, f, 0, 0, nullptr, 0);
    if (bzerror != BZ_OK)
    {
        emit this->extract_finished();
        try
        {
            QString file_name = QFileInfo(archive).fileName();
            throw DownloadError(ERROR_CODE_EXTRACT, ERROR_EXTRACT.arg(file_name));
        }
        catch (DownloadError &e)
        {
            throw DownloadThreadError(e);
        }
    }

    FILE *output_file = fopen(output.toStdString().c_str(), "wb");
    if (output_file == nullptr)
    {
        emit this->extract_finished();
        try
        {
            QString file_name = QFileInfo(output).fileName();
            throw DownloadError(ERROR_CODE_WRITE, ERROR_WRITE.arg(file_name));
        }
        catch (DownloadError &e)
        {
            throw DownloadThreadError(e);
        }
    }

    char buffer[4096];
    while (bzerror == BZ_OK)
    {
        int nread = BZ2_bzRead(&bzerror, archive_file, buffer, sizeof(buffer));
        if ((bzerror == BZ_OK) || bzerror == BZ_STREAM_END)
        {
            size_t nwritten = fwrite(buffer, 1, nread, output_file);
            if (nwritten != (size_t) nread)
            {
                emit this->extract_finished();
                try
                {
                    QString file_name = QFileInfo(archive).fileName();
                    throw DownloadError(ERROR_CODE_EXTRACT, ERROR_EXTRACT.arg(file_name));
                }
                catch (DownloadError &e)
                {
                    throw DownloadThreadError(e);
                }
            }
        }
    }

    if (bzerror != BZ_STREAM_END)
    {
        emit this->extract_finished();
        try
        {
            QString file_name = QFileInfo(archive).fileName();
            throw DownloadError(ERROR_CODE_EXTRACT, ERROR_EXTRACT.arg(file_name));
        }
        catch (DownloadError &e)
        {
            throw DownloadThreadError(e);
        }
    }

    BZ2_bzReadClose(&bzerror, archive_file);
    fclose(f);
    fclose(output_file);
    QFile::remove(archive);

    emit this->extract_finished();
}
