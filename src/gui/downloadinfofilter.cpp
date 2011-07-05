#include "downloadinfofilter.h"

DownloadInfoFilter::DownloadInfoFilter()
{
}

DownloadInfoFilter::DownloadInfoFilter(AddonDownloadInfo *_dInfo, int count)
{
    setDownloadInfo(_dInfo,count);
}

void DownloadInfoFilter::setDownloadInfo(AddonDownloadInfo *_dInfo, int count)
{
    dInfo = _dInfo;
    dInfoCount = count;
    clearAllFilters();
}

void DownloadInfoFilter::setVersionFilter(QString filter)
{
    versionFilter = filter;
}

void DownloadInfoFilter::setFileNameFilter(QString filter)
{
    fileNameFilter = filter;
}

void DownloadInfoFilter::clearAllFilters()
{
    versionFilter.clear();
    fileNameFilter.clear();
}

QStringList DownloadInfoFilter::getVersions()
{
    if (versionFilter.isEmpty() && fileNameFilter.isEmpty())
    {
        QStringList ls;
        for (int i = 0; i < dInfoCount; ++i)
            ls << dInfo[i].gameVersion;
        ls.removeDuplicates();
        return ls;
    }

    if (!versionFilter.isEmpty())
        return QStringList(versionFilter);

    QStringList ls;
    for (int i = 0; i < dInfoCount; ++i)
        if (dInfo[i].filename == fileNameFilter)
            ls << dInfo[i].gameVersion;
    ls.removeDuplicates();
    return ls;
}

QStringList DownloadInfoFilter::getFileNames()
{
    if (versionFilter.isEmpty() && fileNameFilter.isEmpty())
    {
        QStringList ls;
        for (int i = 0; i < dInfoCount; ++i)
            ls << dInfo[i].filename;
        return ls;
    }

    if (!fileNameFilter.isEmpty())
        return QStringList(fileNameFilter);

    QStringList ls;
    for (int i = 0; i < dInfoCount; ++i)
        if (dInfo[i].gameVersion == versionFilter)
            ls << dInfo[i].filename;
    return ls;
}

QUrl DownloadInfoFilter::getDownloadPage()
{
    for (int i = 0; i < dInfoCount; ++i)
        if (dInfo[i].gameVersion == versionFilter && dInfo[i].filename == fileNameFilter)
            return dInfo[i].downloadPage;
    return QUrl("cant find");
}

int DownloadInfoFilter::getDownloadsCount()
{
    for (int i = 0; i < dInfoCount; ++i)
        if (dInfo[i].gameVersion == versionFilter && dInfo[i].filename == fileNameFilter)
            return dInfo[i].downloads;
    return -1;
}
