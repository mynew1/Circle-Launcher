#include "addonswidget.h"
#include "ui_addonswidget.h"
#include <QMessageBox>

AddonsWidget::AddonsWidget(Settings *_settings, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddonsWidget)
{
    ui->setupUi(this);

    settings = _settings;

    curseSearch  = new CurseSearchParser();
    addonDetParser = new CurseAddonDetails();
    connect(curseSearch, SIGNAL(ParsingDone()), this, SLOT(UpdateAddonSrchInfo()));
    connect(addonDetParser, SIGNAL(ParsingDone()), this, SLOT(UpdateAddonDLoadInfo()));
    connect(&dLoadUrlParser, SIGNAL(ParsingDone()), this, SLOT(addAddonToQueue()));
    connect(&dLoadQueue, SIGNAL(countChanged(int)), this, SLOT(updateQueueCount(int)));
    connect(&dLoadQueue, SIGNAL(progress(QUrl,int)), this, SLOT(updateProgress(QUrl,int)));

    ui->tableWidget->setColumnCount(3);
    QStringList ls;
    ls << "name" << "hits" << "rating";
    ui->tableWidget->setHorizontalHeaderLabels(ls);
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->setColumnWidth(0,183);
    ui->tableWidget->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
    ui->downloadFrame->hide();
    ui->progressBar->hide();

    dLoadQueue.setDownloadPath(QDir::currentPath()+"/downloads/");
    installWidget.setDownloadPath(QDir::currentPath()+"/downloads/");
    installWidget.setAddonPath(settings->getGamePath()+"/Interface/AddOns/");

    currentSrchInfo = -1;
}

AddonsWidget::~AddonsWidget()
{
    delete ui;
}

void AddonsWidget::on_searchButton_clicked()
{
    if (ui->searchEdit->text().count() < 3)
    {
        QMessageBox::information(this,tr("Короткое ключ. слово"),tr("Нужно минимум 3 символа."));
        return;
    }

    curseSearch->setIndexTerm(ui->searchEdit->text());
}

void AddonsWidget::UpdateAddonSrchInfo()
{
    int addonsCount = curseSearch->getAddonsInfoCount();
    ui->tableWidget->setRowCount(addonsCount);
    ui->resultCountLabel->setText(QString::number(addonsCount));
    ui->tableWidget->clearContents();
    ui->downloadFrame->hide();
    currentSrchInfo = -1;

    if (addonsCount <= 0)
    {
        QMessageBox::information(this,tr("Не найдено"),tr("Поиск не дал результатов,\n"
                                                  "попробуйте изменить ключ. слово."));
        return;
    }

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
}

void AddonsWidget::on_tableWidget_cellClicked(int row, int /*column*/)
{
    if (currentSrchInfo == row)
        return;

    ui->tableWidget->selectRow(row);
    ui->nameLabel->setText(QString("<style>a { color: black; } a:visited { color: red; }</style><a href=\"%1\">%2</a>").arg(addonsSrchInfo[row].page.toString()).arg(addonsSrchInfo[row].name));
    ui->categoryLabel->setText(addonsSrchInfo[row].category.join(", "));
    ui->hitsLabel->setText(QString::number(addonsSrchInfo[row].hits));
    ui->ratingLabel->setText(QString::number(addonsSrchInfo[row].rating));
    currentSrchInfo = row;
    ui->downloadFrame->hide();
}

void AddonsWidget::on_updateButton_clicked()
{
    if (currentSrchInfo == -1)
    {
        QMessageBox::information(this,tr("Выберите аддон"),tr("Выберите аддон и затем используйте данную кнопку."));
        return;
    }

    addonDetParser->setAddonUrl(addonsSrchInfo[currentSrchInfo].page);
}

void AddonsWidget::UpdateAddonDLoadInfo()
{
    AddonDownloadInfo *dInfo = addonDetParser->getDownloadInfo();
    int dInfoCount = addonDetParser->getCountDownloadInfo();

    ui->versionCombo->clear();
    ui->filenameCombo->clear();

    dInfoFilter.setDownloadInfo(dInfo,dInfoCount);
    dInfoFilter.clearAllFilters();
    ui->versionCombo->addItems(dInfoFilter.getVersions());
    ui->downloadFrame->show();
}

void AddonsWidget::on_versionCombo_currentIndexChanged(QString str)
{
    ui->filenameCombo->clear();

    dInfoFilter.clearAllFilters();

    dInfoFilter.setVersionFilter(str);
    ui->filenameCombo->addItems(dInfoFilter.getFileNames());
    dInfoFilter.setFileNameFilter(ui->filenameCombo->currentText());

    ui->downloadsLabel->setText(QString::number(dInfoFilter.getDownloadsCount()));
}

void AddonsWidget::on_filenameCombo_currentIndexChanged(QString str)
{
    dInfoFilter.setFileNameFilter(str);

    ui->downloadsLabel->setText(QString::number(dInfoFilter.getDownloadsCount()));
}

void AddonsWidget::on_playButton_clicked()
{
    dLoadUrlParser.setAddonUrl(dInfoFilter.getDownloadPage());
}

void AddonsWidget::addAddonToQueue()
{
    dLoadQueue.addToQueue(dLoadUrlParser.getDownloadUrl());
}

void AddonsWidget::updateQueueCount(int count)
{
    ui->inQueueLabel->setText(QString::number(count));
}

void AddonsWidget::updateProgress(QUrl url, int prog)
{
    ui->progressBar->show();
    ui->currDLoadLabel->show();
    ui->progressBar->setValue(prog);
    ui->currDLoadLabel->setText(QFileInfo(url.path()).fileName());
    if (prog == 100)
    {
        ui->progressBar->hide();
        ui->currDLoadLabel->hide();
    }
}

void AddonsWidget::on_installButton_clicked()
{
    installWidget.show();
}
