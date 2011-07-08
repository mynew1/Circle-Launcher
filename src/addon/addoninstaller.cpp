#include "addoninstaller.h"

AddonInstaller::AddonInstaller(QObject *parent) :
    QObject(parent)
{
}

void AddonInstaller::installAddon(QString addonPath, QString extractDir)
{
    QZipReader zip_reader(addonPath);
    if (!zip_reader.exists())
    {
        qWarning() << "cant find addon path";
        return;
    }

    zip_reader.extractAll(extractDir);
}

bool AddonInstaller::isAlreadyInstalled(QString addonPath, QString checkDir)
{
    QZipReader zip_reader(addonPath);
    if (zip_reader.exists())
        foreach (QZipReader::FileInfo info, zip_reader.fileInfoList())
            if (info.isDir && info.filePath.count("/") == 1)
            {
                QDir dir(checkDir+info.filePath);
                if (dir.exists())
                    return true;
            }

    return false;
}
