#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "addonswidget.h"
#include "src/gui/mainwidget.h"
#include "src/gui/settingsform.h"
#include "src/game.h"
#include <QPushButton>
#include <QCloseEvent>
#include <QSystemTrayIcon>


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    explicit MainWindow(Settings *_settings, QWidget *parent = 0);
    ~MainWindow();
    void setSettings(Settings *_settings);

private slots:
    void on_mainButton_clicked();

    void on_addonButton_clicked();

    void on_arenaButton_clicked();

    void on_toolButton_clicked();
    void UpdateRealms();
    void activateTrayIcon(QSystemTrayIcon::ActivationReason reason);
    void closeEvent(QCloseEvent *);

    void on_playButton_clicked();

    void on_cleanButton_clicked();

private:
    void Init();
    Ui::MainWindow *ui;
    AddonsWidget   *aw;
    MainWidget     *mw;
    SettingsForm    settingsForm;
    Settings       *settings;
    Game            game;

    void UnActiveTabs();
    void ActiveTab(QPushButton *button);

    //tray items
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    QAction *playAction;
    QAction *quitAction;

    void createActions();
    void createTrayIcon();
};

#endif // MAINWINDOW_H
