#include "mainwidget.h"
#include "ui_mainwidget.h"

QString news = "http://forum.wowcircle.com/forumdisplay.php?f=2";
QString fix  = "http://forum.wowcircle.com/forumdisplay.php?f=120";
QString info = "http://forum.wowcircle.com/forumdisplay.php?f=3";

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    pageData = new QString[3];
    connect(&page1, SIGNAL(DataCollected(ForumMgr*)), this, SLOT(Update1Forum()));
    connect(&page2, SIGNAL(DataCollected(ForumMgr*)), this, SLOT(Update2Forum()));
    connect(&page3, SIGNAL(DataCollected(ForumMgr*)), this, SLOT(Update3Forum()));
    page1.setForumPage(QUrl(news));
    page2.setForumPage(QUrl(fix));
    page3.setForumPage(QUrl(info));
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::Update1Forum()
{
    pageData[0] = page1.getHtmlData();
    on_forumButton1_clicked();
}

void MainWidget::Update2Forum()
{
    pageData[1] = page2.getHtmlData();
}

void MainWidget::Update3Forum()
{
    pageData[2] = page3.getHtmlData();
}

void MainWidget::on_forumButton1_clicked()
{
    ui->textBrowser->setText(pageData[0]);
    ActiveForumButton(ui->forumButton1);
    UnActiveForumButton(ui->forumButton2);
    UnActiveForumButton(ui->forumButton3);
}

void MainWidget::on_forumButton2_clicked()
{
    ui->textBrowser->setText(pageData[1]);
    ActiveForumButton(ui->forumButton2);
    UnActiveForumButton(ui->forumButton1);
    UnActiveForumButton(ui->forumButton3);
}

void MainWidget::on_forumButton3_clicked()
{
    ui->textBrowser->setText(pageData[2]);
    ActiveForumButton(ui->forumButton3);
    UnActiveForumButton(ui->forumButton1);
    UnActiveForumButton(ui->forumButton2);
}

void MainWidget::ActiveForumButton(QPushButton *button)
{
    button->setStyleSheet("QPushButton{	border: none;"
                          "border-top: 1px solid rgba(0,0,0,80);"
                          "border-bottom: 1px solid rgba(0,0,0,80);"
                          "text-align:left;background-color: rgba(0,0,0,50);}");
}

void MainWidget::UnActiveForumButton(QPushButton *button)
{
    button->setStyleSheet("QPushButton{	border: none;"
                          "border-top: 1px solid rgba(0,0,0,80);"
                          "border-bottom: 1px solid rgba(0,0,0,80);"
                          "text-align:left;background-color: rgba(0,0,0,0);}");
}
