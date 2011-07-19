#ifndef ADDONINSTALLER_H
#define ADDONINSTALLER_H

#include <QObject>
#include <QDebug>
#include <QDir>
#include "src/addon/zip/myqzipreader_p.h"

class AddonInstaller : public QObject
{
    Q_OBJECT

public:
    explicit AddonInstaller(QObject *parent = 0);

    void installAddon(QString addonPath, QString extractDir);
    bool isAlreadyInstalled(QString addonPath, QString checkDir);
};

#endif // ADDONINSTALLER_H
