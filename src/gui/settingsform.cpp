#include "settingsform.h"
#include "ui_settingsform.h"

SettingsForm::SettingsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsForm)
{
    ui->setupUi(this);
    haveSettings = false;
    settings = new Settings;
}

SettingsForm::~SettingsForm()
{
    delete ui;
}

void SettingsForm::setSettings(Settings *_settings)
{
    settings = _settings;
    haveSettings = true;
    if (settings->IsFirstUse())
        this->show();
}

void SettingsForm::LoadSettings()
{
    if (!haveSettings)
        return;

    ui->gamePath->setText(settings->getGamePath());

    ui->cacheCheck->setChecked(settings->getIsCleanCache());
    ui->wtfCheck->setChecked(settings->getIsCleanWtf());
    ui->exitCheck->setChecked(settings->getExitAfterStart());

    ui->forum1->setText(settings->getForumView1());
    ui->forum2->setText(settings->getForumView2());
    ui->forum3->setText(settings->getForumView3());

    ui->updateTime->setValue(settings->getUpdateTime());
    UpdateRealms();
}

void SettingsForm::show()
{
    LoadSettings();
    QWidget::show();
}

void SettingsForm::SaveSettings()
{
    if (!haveSettings)
        return;

    settings->setGamePath(ui->gamePath->text());

    settings->setIsCleanCache(ui->cacheCheck->isChecked());
    settings->setIsCleanWtf(ui->wtfCheck->isChecked());
    settings->setExitAfterStart(ui->exitCheck->isChecked());

    if(!ui->forum1->text().isEmpty())
        settings->setForumView1(ui->forum1->text());
    if(!ui->forum2->text().isEmpty())
        settings->setForumView2(ui->forum2->text());
    if(!ui->forum3->text().isEmpty())
        settings->setForumView3(ui->forum3->text());

    settings->setUpdateTime(ui->updateTime->value());
    settings->SaveSettings();
}

void SettingsForm::UpdateRealms()
{
    QStringList list;
    list << "Name" << "Address";
    ui->realmsTable->setColumnCount(list.size());
    ui->realmsTable->setHorizontalHeaderLabels(list);
    ui->realmsTable->setColumnWidth(0, 100);
    ui->realmsTable->setColumnWidth(1, 192);
    ui->realmsTable->setRowCount(settings->getRealms().size());

    for (int i = 0; i < settings->getRealms().size(); i++)
    {
        QTableWidgetItem *name = new QTableWidgetItem(settings->getRealms().at(i).realmName);
        QTableWidgetItem *address = new QTableWidgetItem(settings->getRealms().at(i).realmUrl);

        ui->realmsTable->setItem(i, 0, name);
        ui->realmsTable->setItem(i, 1, address);
    }
}

void SettingsForm::on_deleteButton_clicked()
{
    int index = ui->realmsTable->currentRow();
    if (index < 0 || index >= settings->getRealms().size())
        return;

    settings->removeRealm(index);
    settings->SaveSettings();

    UpdateRealms();
    emit RealmsChanged();
}

void SettingsForm::on_browseButton_clicked()
{
    ui->gamePath->setText(QFileDialog::getExistingDirectory());
}

void SettingsForm::on_addButton_clicked()
{
    Realm realm;
    realm.realmName = "";
    realm.realmUrl  = "";

    settings->addRealm(realm);
    settings->SaveSettings();
    UpdateRealms();
    emit RealmsChanged();
}

void SettingsForm::on_generalButton_clicked()
{
    ActivateButton(ui->generalButton, "medal.png");
    ui->tabWidget->setCurrentIndex(0);
}

void SettingsForm::on_guiButton_clicked()
{
    ActivateButton(ui->guiButton, "photo.png");
    ui->tabWidget->setCurrentIndex(1);
}

void SettingsForm::on_realmsButton_clicked()
{
    ActivateButton(ui->realmsButton, "globe.png");
    ui->tabWidget->setCurrentIndex(2);
}

void SettingsForm::on_gamePath_textChanged(QString )
{
    SaveSettings();
}

void SettingsForm::on_wtfCheck_stateChanged(int )
{
    SaveSettings();
}


void SettingsForm::on_cacheCheck_stateChanged(int )
{
    SaveSettings();
}

void SettingsForm::on_updateTime_valueChanged(int )
{
    SaveSettings();
}

void SettingsForm::on_exitCheck_stateChanged(int )
{
    SaveSettings();
}

void SettingsForm::on_forum1_textEdited(QString )
{
    SaveSettings();
}

void SettingsForm::on_forum2_textEdited(QString )
{
    SaveSettings();
}

void SettingsForm::on_forum3_textEdited(QString )
{
    SaveSettings();
}

void SettingsForm::on_realmsTable_cellChanged(int row, int column)
{
    QString checkingText = (column) ? settings->getRealms().at(row).realmUrl :
                                      settings->getRealms().at(row).realmName;
    if (ui->realmsTable->item(row, column)->text() == checkingText)
        return;

    Realm realm;
    realm.realmName = ui->realmsTable->item(row,0)->text();
    realm.realmUrl  = ui->realmsTable->item(row,1)->text();
    settings->setRealmAt(realm, row);
    settings->SaveSettings();
    UpdateRealms();
    emit RealmsChanged();
}

void SettingsForm::UnActivateButtons()
{
    ui->generalButton->setStyleSheet("image: url(:/img/medal.png);");
    ui->guiButton->setStyleSheet("image: url(:/img/photo.png);");
    ui->realmsButton->setStyleSheet("image: url(:/img/globe.png);");
}

void SettingsForm::ActivateButton(QPushButton *button, QString img)
{
    UnActivateButtons();

    QString style = QString("font-weight: bold;"
                            "image: url(:/img/%1);"
                            "border-left: 3px solid rgb(221,75,57);").arg(img);
    button->setStyleSheet(style);
}

void SettingsForm::on_clearOnlineButton_clicked()
{
    for (int i = 0; i < MAX_REALMS_ONLINE_COUNT; i++)
        settings->setMaxOnlineByIndex(i, 0);
    settings->SaveSettings();
}
