#ifndef ADDONSWIDGET_H
#define ADDONSWIDGET_H

#include <QWidget>
#include <QTableWidgetItem>
#include "src/parsers/curse/cursesearchparser.h"

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
    Ui::AddonsWidget *ui;
    CurseSearchParser *curseSearch;
    AddonSearchInfo *addonsSrchInfo;

public slots:
    void UpdateAddonSrchInfo();
private slots:
    void on_searchButton_clicked();
    void on_tableWidget_cellClicked(int row, int column);
};

#endif // ADDONSWIDGET_H
