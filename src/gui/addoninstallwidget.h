#ifndef ADDONINTALLWIDGET_H
#define ADDONINTALLWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QDebug>
#include <QHeaderView>
#include "src/addon/zipsearch.h"
#include "src/addon/addoninstaller.h"

class AddonInstallWidget : public QWidget
{
    Q_OBJECT
private:
    QTableWidget *table;
    QString addonPath;
    QString dLoadPath;

    ZipSearch zip;
    QStringList zipsPath;

    AddonInstaller installer;

public:
    explicit AddonInstallWidget(QWidget *parent = 0);
    AddonInstallWidget(QString aPath, QString dPath);

    void setDownloadPath(QString dPath) { dLoadPath = dPath; }
    void setAddonPath(QString aPath)    { addonPath = aPath; }

signals:
    void hideButtons();

public slots:
    void show();

private slots:
    void installClicked();
    void tableCellClicked(int,int);
};

#endif // ADDONINTALLWIDGET_H
