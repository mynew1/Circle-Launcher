#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLineEdit>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    settings = new Settings;
    createActions();
    createTrayIcon();
}

MainWindow::MainWindow(Settings *_settings, QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    settings = new Settings;
    setSettings(_settings);
    createActions();
    createTrayIcon();
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
    if (!isGameExists())
        return;

    game.setGamePath(settings->getGamePath());
    game.setClearCacheBefore(settings->getIsCleanCache());
    game.setClearWtfBefore(settings->getIsCleanWtf());
    settings->setDefaultRealmIndex(ui->realmCombo->currentIndex());
    settings->SaveSettings();

    QString realmList = settings->getRealms().at(ui->realmCombo->currentIndex()).realmUrl;
    game.setRealmList(realmList);

    if (mw->isNewUpdate())
        on_cleanButton_clicked();

    game.StartGame();
    if (settings->getExitAfterStart())
        qApp->exit();
}

void MainWindow::on_cleanButton_clicked()
{
    if (!isGameExists())
        return;

    game.setGamePath(settings->getGamePath());
    game.ClearCache();
}

void MainWindow::createActions()
{
    playAction = new QAction(tr("Play"), this);
    playAction->setIcon(QIcon(":/img/play16x16.png"));
    connect(playAction, SIGNAL(triggered()), this, SLOT(on_playButton_clicked()));


    quitAction = new QAction("&Exit", this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void MainWindow::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(playAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/img/logo.png"));
    trayIcon->show();
    trayIcon->setContextMenu(trayIconMenu);
    connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(showMaximized()));
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(activateTrayIcon(QSystemTrayIcon::ActivationReason)));
}

void MainWindow::activateTrayIcon(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::Context)
        trayIconMenu->activateWindow();

    switch(reason)
    {
//    case QSystemTrayIcon::MiddleClick:
//        showTrayMessage();
//        break;
    case QSystemTrayIcon::DoubleClick:
        showNormal();
        break;
    default:
        break;
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (trayIcon->isVisible()) {
        hide();
        event->ignore();
    }
}

bool MainWindow::isGameExists()
{
    if (!Game::gameExists(settings->getGamePath()))
    {
        showNormal();
        QMessageBox::warning(this,"",
                                 QString(tr("Игра не найдена. "
                                         "Текущий каталог:\n \"%1\".\n"
                                         "Отредактируйте ваши настройки.")).arg(settings->getGamePath()));
        return false;
    }
    return true;
}
