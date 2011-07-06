#ifndef ADDONSWIDGET_H
#define ADDONSWIDGET_H

#include <QWidget>
#include <QTableWidgetItem>
#include "src/parsers/curse/cursesearchparser.h"
#include "src/parsers/curse/curseaddondetails.h"
#include "src/parsers/curse/curseparsedownloadurl.h"
#include "src/gui/downloadinfofilter.h"
#include "src/addon/downloadqueue.h"

namespace Ui {
    class AddonsWidget;
}

class AddonsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AddonsWidget(QWidget *parent = 0);
    ~AddonsWidget();

private:
    Ui::AddonsWidget      *ui;
    CurseSearchParser     *curseSearch;
    AddonSearchInfo       *addonsSrchInfo;
    CurseAddonDetails     *addonDetParser;
    DownloadInfoFilter     dInfoFilter;
    CurseParseDownloadUrl  dLoadUrlParser;
    DownloadQueue          dLoadQueue;

    int currentSrchInfo;

public slots:
    void UpdateAddonSrchInfo();
    void UpdateAddonDLoadInfo();

private slots:
    void on_searchButton_clicked();
    void on_tableWidget_cellClicked(int row, int column);
    void on_updateButton_clicked();
    void on_versionCombo_currentIndexChanged(QString );
    void on_filenameCombo_currentIndexChanged(QString );
    void on_playButton_clicked();
    void addAddonToQueue();
};

#endif // ADDONSWIDGET_H
