#ifndef DOWNLOADQUEUE_H
#define DOWNLOADQUEUE_H

#include <QObject>
#include <QDir>
#include "src/httpdownload.h"

class DownloadQueue : public QObject
{
    Q_OBJECT

private:
    HttpDownload dLoad;

    struct DownloadNode {
        QUrl downloadUrl;
        DownloadNode *next;
    };

    DownloadNode *head;
    DownloadNode *tail;
    DownloadNode *currNode;
    int nodeCounts;

    QString dLoadPath;

    void Init();
//    DownloadNode* next();
//    void toHead();

    bool alreadyInQueue(QUrl _url);

public:
    explicit DownloadQueue(QObject *parent = 0);
    DownloadQueue(QString _path);
    void setDownloadPath(QString _path);

    bool addToQueue(QUrl);
    bool removeFromQueue(QUrl);

signals:
    void dLoadStarted(QUrl);
    void dLoadFinished(QUrl);
    void countChanged(int);
    void progress(QUrl, int);

private slots:
    void emitDLoadStarted(QUrl);
    void emitDLoadFinished(QUrl);
    void startNewDLoad();
    void emitProgress(int);
};

#endif // DOWNLOADQUEUE_H
