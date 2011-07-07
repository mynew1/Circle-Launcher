#include "downloadqueue.h"

DownloadQueue::DownloadQueue(QObject *parent) :
    QObject(parent)
{
    Init();
}

DownloadQueue::DownloadQueue(QString _path)
{
    Init();
    setDownloadPath(_path);
}

void DownloadQueue::Init()
{
    dLoadPath  = QDir::currentPath();
    nodeCounts = 0;
    head       = NULL;
    currNode   = NULL;
    connect(&dLoad, SIGNAL(downloadStarted(QUrl)), this, SLOT(emitDLoadStarted(QUrl)));
    connect(&dLoad, SIGNAL(downloadFinished(QUrl)), this, SLOT(emitDLoadFinished(QUrl)));
    connect(&dLoad, SIGNAL(readProgress(int)), this, SLOT(emitProgress(int)));
}

void DownloadQueue::setDownloadPath(QString _path)
{
    dLoadPath = _path;
}

void DownloadQueue::emitDLoadStarted(QUrl url)
{
    emit dLoadStarted(url);
}

void DownloadQueue::emitDLoadFinished(QUrl url)
{
    dLoad.SaveToFile(dLoadPath);
    emit dLoadFinished(url);
    removeFromQueue(url);
    startNewDLoad();
}

void DownloadQueue::startNewDLoad()
{
    if (head)
        dLoad.downloadFile(head->downloadUrl);
}

bool DownloadQueue::addToQueue(QUrl url)
{
    if (!nodeCounts)
    {
        head = new DownloadNode;
        head->downloadUrl = url;
        head->next = NULL;
        tail = head;
        nodeCounts++;
        emit countChanged(nodeCounts);
        startNewDLoad();
        return true;
    }

    if (alreadyInQueue(url))
        return false;

    DownloadNode *tmp = new DownloadNode;
    tmp->downloadUrl = url;
    tmp->next = NULL;
    tail->next = tmp;
    tail = tmp;
    nodeCounts++;
    emit countChanged(nodeCounts);
    return true;
}

bool DownloadQueue::removeFromQueue(QUrl url)
{
    DownloadNode *iter = head;
    //check head
    if (iter->downloadUrl == url)
    {
        DownloadNode *tmp = head->next;
        delete head;
        head = tmp;
        --nodeCounts;
        emit countChanged(nodeCounts);
        return true;
    }

    while (iter->next) {
        if (iter->next->downloadUrl == url)
        {
            DownloadNode *tmp = iter->next->next;
            if (iter->next == tail)
                tail = iter;
            delete iter->next;
            iter->next = tmp;
            --nodeCounts;
            emit countChanged(nodeCounts);
            return true;
        }
        iter = iter->next;
    }

    return false;
}

bool DownloadQueue::alreadyInQueue(QUrl _url)
{
    DownloadNode *iter = head;
    bool haveDuplicates = false;
    while (iter) {
        if(iter->downloadUrl == _url)
            haveDuplicates = true;
        iter = iter->next;
    }
    return haveDuplicates;
}

void DownloadQueue::emitProgress(int prog)
{
    emit progress(head->downloadUrl, prog);
}
