#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "addonswidget.h"
#include <QPushButton>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_mainButton_clicked();

    void on_addonButton_clicked();

    void on_arenaButton_clicked();

private:
    Ui::MainWindow *ui;
    AddonsWidget *aw;

    void UnActiveTabs();
    void ActiveTab(QPushButton *button);
};

#endif // MAINWINDOW_H
