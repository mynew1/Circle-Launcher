#include "settingsform.h"
#include "ui_settingsform.h"

SettingsForm::SettingsForm(QWidget *parent) :
    QDialog(parent),
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
    QDialog::show();
}

void SettingsForm::SaveSettings()
{
    if (!haveSettings)
        return;

    settings->setGamePath(ui->gamePath->text());

    settings->setIsCleanCache(ui->cacheCheck->isChecked());
    settings->setIsCleanWtf(ui->wtfCheck->isChecked());
    settings->setExitAfterStart(ui->exitCheck->isChecked());

    settings->setForumView1(ui->forum1->text());
    settings->setForumView2(ui->forum2->text());
    settings->setForumView3(ui->forum3->text());

    settings->setUpdateTime(ui->updateTime->value());

    settings->SaveSettings();
}

void SettingsForm::on_buttonBox_accepted()
{
    SaveSettings();
}

void SettingsForm::UpdateRealms()
{
    QStringList realms;
    for (int i = 0; i < settings->getRealms().size(); i++)
        realms << settings->getRealms().at(i).realmName;

    ui->realmsCombo->clear();
    ui->realmsCombo->addItems(realms);
}

void SettingsForm::on_deleteButton_clicked()
{
    settings->removeRealm(ui->realmsCombo->currentIndex());
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
    if (ui->realmName->text().isEmpty() && ui->realmUrl->text().isEmpty())
        return;

    Realm realm;
    realm.realmName = ui->realmName->text();
    realm.realmUrl  = ui->realmUrl->text();

    settings->addRealm(realm);
    settings->SaveSettings();
    UpdateRealms();
    emit RealmsChanged();
}
