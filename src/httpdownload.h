#ifndef HttpDownload_H
#define HttpDownload_H

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QDebug>
#include <QFileInfo>
#include <QStringList>
#include <QUrl>

QT_BEGIN_NAMESPACE
class QNetworkReply;
class QFile;
QT_END_NAMESPACE

class HttpDownload : public QObject
{
    Q_OBJECT

public:
    HttpDownload(QObject *parent = 0);
    ~HttpDownload();
    void downloadFile(QUrl _url);


public slots:
    QByteArray getDownloadData();
    bool SaveToFile();

private slots:
    void startRequest(QUrl _url);
    void httpFinished();
    void httpReadyRead();
    void updateDataReadProgress(qint64 bytesRead, qint64 totalBytes);

public slots:
    void cancelDownload();

signals:
    void downloadStarted();
    void downloadFinished();

private:
    QUrl url;
    QNetworkAccessManager *qnam;
    QNetworkReply *reply;
    int httpGetId;
    bool httpRequestAborted;
    bool processDone;
};

#endif
