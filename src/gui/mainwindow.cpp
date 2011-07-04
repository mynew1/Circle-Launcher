#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLineEdit>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QStringList ls;
    ls << "logon" << "logon2" << "logon3";
    ui->realmCombo->addItems(ls);
    ui->realmCombo->setEditable(true);
    QString style = "QLineEdit:hover {"
            "background-color: transparent;"
            "}";
    ui->realmCombo->lineEdit()->setStyleSheet(style);
    ui->realmCombo->lineEdit()->setReadOnly(true);
    ui->realmCombo->lineEdit()->setAlignment(Qt::AlignRight);
    for(int i = 0; i < ui->realmCombo->count(); i++)
        ui->realmCombo->setItemData(i, Qt::AlignRight, Qt::TextAlignmentRole);

    aw = new AddonsWidget(ui->mainWidget);

    ActiveTab(ui->addonButton);
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
}

void MainWindow::on_addonButton_clicked()
{
    ActiveTab(ui->addonButton);
}

void MainWindow::on_arenaButton_clicked()
{
    ActiveTab(ui->arenaButton);
}
