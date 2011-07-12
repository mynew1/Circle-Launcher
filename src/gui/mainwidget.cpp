#include "mainwidget.h"
#include "ui_mainwidget.h"
#include <QScrollBar>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    ui->textBrowser->verticalScrollBar()->setStyleSheet("background-color:transparent;");
}

MainWidget::~MainWidget()
{
    delete ui;
}
