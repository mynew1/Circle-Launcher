#include "addonintallwidget.h"

AddonIntallWidget::AddonIntallWidget(QWidget *parent) :
    QWidget(parent)
{
    table = new QTableWidget(this);
    connect(table, SIGNAL(cellClicked(int,int)),
            this, SLOT(tableCellClicked(int,int)));
}

AddonIntallWidget::AddonIntallWidget(QString aPath, QString dPath)
{
    setDownloadPath(dPath);
    setAddonPath(aPath);
}

void AddonIntallWidget::show()
{
    QString tableStyle = "background-color: rgb(45, 45, 45);"
                         "font: 9pt \"Arial\";"
                         "color: #AAAAAA;";
    QString buttonStyle =
            "QPushButton {"
                "font: 9pt \"Arial\";"
                "background-color: qlineargradient(spread:pad, x1:1, y1:1, x2:1,"
                "y2:0, stop:0.00966184 rgb(45, 45, 45), stop:0.637681 rgb(75, 75, 75));"
                "color: white;"
                "border: 1px solid black;"
                "border-radius: 5px;"
                "color: white;"
                "font-weight: bold;"
            "}"
            "QPushButton:hover {"
                "background-color: black;"
            "}";

    zip.setPath(dLoadPath);
    QStringList names = zip.getZipsName();
    zipsPath = zip.getZipsPath();
    table->clear();
    table->setStyleSheet("QTableWidget{"+tableStyle+"}");
    table->setShowGrid(false);
    table->verticalHeader()->hide();
    table->horizontalHeader()->hide();
    table->setColumnCount(2);
    table->setRowCount(names.count());
    table->setColumnWidth(0, 180);

    for (int i = 0; i < names.count(); ++i)
    {
        QPushButton *button = new QPushButton("Install");
        button->setDisabled(true);
        button->setStyleSheet(buttonStyle);
        table->setIndexWidget(table->model()->index(i,1),button);
        connect(button, SIGNAL(clicked()), this, SLOT(installClicked()));
        connect(this, SIGNAL(hideButtons()), button, SLOT(hide()));

        QTableWidgetItem *zipName = new QTableWidgetItem(names.at(i));
        table->setItem(i, 0, zipName);
    }

    table->resize(300,300);
    this->resize(300,300);
    QWidget::show();
//    emit hideButtons();
}

void AddonIntallWidget::tableCellClicked(int row, int)
{
    emit hideButtons();
    table->indexWidget(table->model()->index(row,1))->show();
    table->indexWidget(table->model()->index(row,1))->setDisabled(false);
}

void AddonIntallWidget::installClicked()
{
    installer.installAddon(zipsPath.at(table->currentRow()),addonPath);
}
