#ifndef ZIPSEARCH_H
#define ZIPSEARCH_H

#include <QFile>
#include <QDir>
#include <QDebug>

class ZipSearch
{
    QStringList zipsName;
    QStringList zipsPath;
public:
    ZipSearch();
    ZipSearch(QString path);
    void setPath(QString path);

    QStringList getZipsName() { return zipsName; }
    QStringList getZipsPath() { return zipsPath; }
};

#endif // ZIPSEARCH_H
