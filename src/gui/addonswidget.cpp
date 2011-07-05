#include "addonswidget.h"
#include "ui_addonswidget.h"
#include <QMessageBox>


AddonsWidget::AddonsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddonsWidget)
{
    ui->setupUi(this);

    curseSearch = new CurseSearchParser();
    connect(curseSearch,SIGNAL(ParsingDone()),this,SLOT(UpdateAddonSrchInfo()));

    ui->tableWidget->setColumnCount(3);
    QStringList ls;
    ls << "name" << "hits" << "rating";
    ui->tableWidget->setHorizontalHeaderLabels(ls);
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->setColumnWidth(0,183);
    ui->tableWidget->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
    currentSrchInfo = 0;
}

AddonsWidget::~AddonsWidget()
{
    delete ui;
}

void AddonsWidget::on_searchButton_clicked()
{
    if (ui->searchEdit->text().count() > 2 && curseSearch->isParsingReady())
        curseSearch->setIndexTerm(ui->searchEdit->text());
    else
        QMessageBox::information(this,"Small index term","Need at least 3 characters.");
}

void AddonsWidget::UpdateAddonSrchInfo()
{
    int addonsCount = curseSearch->getAddonsInfoCount();
    ui->tableWidget->setRowCount(addonsCount);
    ui->resultCountLabel->setText(QString::number(addonsCount));
    ui->tableWidget->clearContents();

    if (addonsCount > 0)
    {
        addonsSrchInfo = new AddonSearchInfo[addonsCount];
        addonsSrchInfo = curseSearch->getAddonsInfo();
        for (int row = 0; row < addonsCount; ++row) {
            QTableWidgetItem *name = new QTableWidgetItem(addonsSrchInfo[row].name);
            ui->tableWidget->setItem(row, 0, name);
            QTableWidgetItem *hits = new QTableWidgetItem(QString::number(addonsSrchInfo[row].hits));
            hits->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->setItem(row, 1, hits);
            double _rating = (addonsSrchInfo[row].rating <= -1) ? 0 : addonsSrchInfo[row].rating;
            QTableWidgetItem *rating = new QTableWidgetItem(QString::number(_rating));
            rating->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->setItem(row, 2, rating);
        }
    } else
        QMessageBox::information(this,"Not found","Sorry, we were unable to find any \n"
                                 "results using your search terms.");
}

void AddonsWidget::on_tableWidget_cellClicked(int row, int column)
{
    ui->nameLabel->setText(QString("<style>a { color: black; } a:visited { color: red; }</style><a href=\"%1\">%2</a>").arg(addonsSrchInfo[row].page.toString()).arg(addonsSrchInfo[row].name));
    ui->categoryLabel->setText(addonsSrchInfo[row].category.join(", "));
    ui->hitsLabel->setText(QString::number(addonsSrchInfo[row].hits));
    ui->ratingLabel->setText(QString::number(addonsSrchInfo[row].rating));
    currentSrchInfo = row;
}

void AddonsWidget::on_updateButton_clicked()
{

}
