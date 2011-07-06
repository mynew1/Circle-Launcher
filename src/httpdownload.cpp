#include "httpdownload.h"

HttpDownload::HttpDownload(QObject *parent) : QObject(parent)
{
    processDone = false;
    qnam = new QNetworkAccessManager(this);
}

void HttpDownload::startRequest(QUrl _url)
{
    reply = qnam->get(QNetworkRequest(_url));
    qDebug() << QString("Downloading '%1' started.").arg(url.toString());
}

void HttpDownload::downloadFile(QUrl _url)
{
    url = _url;
    qnam = new QNetworkAccessManager(this);
    httpRequestAborted = false;
    processDone = false;
    startRequest(_url);
    connect(reply, SIGNAL(finished()),
            this, SLOT(httpFinished()));
    connect(reply, SIGNAL(readyRead()),
            this, SLOT(httpReadyRead()));
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)),
            this, SLOT(updateDataReadProgress(qint64,qint64)));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(ErrorMassage(QNetworkReply::NetworkError)));
}

HttpDownload::~HttpDownload()
{
//    delete reply->manager();
}

void HttpDownload::cancelDownload()
{
    httpRequestAborted = true;
}

void HttpDownload::httpFinished()
{
    processDone = true;
//    reply->deleteLater();
//    qnam->deleteLater();
//    reply = 0;
    if (!httpRequestAborted)
        emit downloadFinished(url);
}

void HttpDownload::httpReadyRead()
{
    emit downloadStarted(url);
}

void HttpDownload::updateDataReadProgress(qint64 bytesRead, qint64 totalBytes)
{
    if (httpRequestAborted)
        return;
    qDebug() << QString(" Download: %1/%2").arg(bytesRead).arg(totalBytes);
}

QByteArray HttpDownload::getDownloadData()
{
    if(!processDone)
        return "\0";
    return reply->readAll();
}

bool HttpDownload::SaveToFile()
{
    if(!processDone)
        return false;

    QFile *file;
    QFileInfo fileInfo(url.path());
    QString fileName = fileInfo.fileName();
    if (QFile::exists(fileName)) {
        qWarning() << "File with name \"" << fileName <<"\" already exists. Rewrite.";
    }

    file = new QFile(fileName);
    if (!file->open(QIODevice::WriteOnly)) {
        qWarning() << "Cant open file.";
        delete file;
        return false;
    }

    file->write(reply->readAll());
    file->close();
    return true;
}

void HttpDownload::ErrorMassage(QNetworkReply::NetworkError er)
{
    qDebug() << QString("Error: %1. In download file -").arg(er) << url.toString() ;

}
