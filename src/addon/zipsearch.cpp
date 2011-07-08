#include "zipsearch.h"

ZipSearch::ZipSearch()
{
}

void ZipSearch::setPath(QString path)
{
    QDir dir;
    dir.setPath(path);
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    dir.setSorting(QDir::Time);

    QFileInfoList list = dir.entryInfoList();
    zipsName.clear();
    zipsPath.clear();
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        if (fileInfo.completeSuffix().contains("zip"))
        {
            zipsName << fileInfo.baseName();
            zipsPath << fileInfo.absoluteFilePath();
        }
    }
}
