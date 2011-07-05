#ifndef DOWNLOADINFOFILTER_H
#define DOWNLOADINFOFILTER_H
#include "src/parsers/curse/curseaddondetails.h"

class DownloadInfoFilter
{
    AddonDownloadInfo *dInfo;
    int dInfoCount;

    QString versionFilter;
    QString fileNameFilter;
public:
    DownloadInfoFilter();
    DownloadInfoFilter(AddonDownloadInfo *_dInfo, int count);

    void setDownloadInfo(AddonDownloadInfo *_dInfo, int count);

    void setVersionFilter(QString filter);
    void setFileNameFilter(QString filter);
    void clearAllFilters();

    QStringList getVersions();
    QStringList getFileNames();

    //use when two filters active
    QUrl getDownloadPage();
    int  getDownloadsCount();
};

#endif // DOWNLOADINFOFILTER_H
