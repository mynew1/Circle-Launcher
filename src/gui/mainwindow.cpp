#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLineEdit>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    settings = new Settings;
}

MainWindow::MainWindow(Settings *_settings, QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    settings = new Settings;
    setSettings(_settings);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::UnActiveTabs()
{
    QString style = "QPushButton {"
                            "background-color: transparent;"
                            "border: none;"
                    "}"

                    "QPushButton:hover{"
                            "background-color: rgb(76, 76, 76);"
                    "}";
    ui->mainButton->setStyleSheet(style);
    ui->addonButton->setStyleSheet(style);
    ui->arenaButton->setStyleSheet(style);
}

void MainWindow::ActiveTab(QPushButton *button)
{
    UnActiveTabs();
    QString style = "QPushButton {"
                            "color: white;"
                            "font-weight: bold;"
                            "background-image: url(:/img/orange-header.png);"
                            "border: none;"
                    "}"
                    "QPushButton:hover{"
                            "background-color: rgb(76, 76, 76);"
                    "}";
    button->setStyleSheet(style);
}

void MainWindow::on_mainButton_clicked()
{
    ActiveTab(ui->mainButton);
    aw->hide();
    mw->show();
}

void MainWindow::on_addonButton_clicked()
{
    ActiveTab(ui->addonButton);
    mw->hide();
    aw->show();
}

void MainWindow::on_arenaButton_clicked()
{
    ActiveTab(ui->arenaButton);
}

void MainWindow::on_toolButton_clicked()
{
    settingsForm.show();
}

void MainWindow::Init()
{

    QString style = "QLineEdit:hover {"
                        "background-color: transparent;"
                    "}";

    ui->realmCombo->setEditable(true);
    ui->realmCombo->lineEdit()->setStyleSheet(style);
    ui->realmCombo->lineEdit()->setReadOnly(true);
    ui->realmCombo->lineEdit()->setAlignment(Qt::AlignRight);

    for(int i = 0; i < ui->realmCombo->count(); i++)
        ui->realmCombo->setItemData(i, Qt::AlignRight, Qt::TextAlignmentRole);

    aw = new AddonsWidget(settings, ui->mainWidget);
    mw = new MainWidget(settings, ui->mainWidget);

    connect(&settingsForm, SIGNAL(RealmsChanged()), this, SLOT(UpdateRealms()));

    UpdateRealms();
    on_mainButton_clicked();
}

void MainWindow::setSettings(Settings *_settings)
{
    settings = _settings;
    settingsForm.setSettings(_settings);
    Init();
}

void MainWindow::UpdateRealms()
{
    QList<Realm> realms = settings->getRealms();
    QStringList ls;
    for(int i = 0; i < realms.size(); ++i)
        ls << realms.at(i).realmName;

    ui->realmCombo->clear();
    ui->realmCombo->addItems(ls);
    ui->realmCombo->setCurrentIndex(settings->getDefaultRealmIndex());
}

void MainWindow::on_playButton_clicked()
{
    game.setGamePath(settings->getGamePath());
    game.setClearCacheBefore(settings->getIsCleanCache());
    game.setClearWtfBefore(settings->getIsCleanWtf());
    settings->setDefaultRealmIndex(ui->realmCombo->currentIndex());
    settings->SaveSettings();

    QString realmList = settings->getRealms().at(ui->realmCombo->currentIndex()).realmUrl;
    game.setRealmList(realmList);

    game.StartGame();
    if (settings->getExitAfterStart())
        this->close();
}

void MainWindow::on_cleanButton_clicked()
{
    game.setGamePath(settings->getGamePath());
    game.ClearCache();
}
