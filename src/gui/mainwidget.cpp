#include "mainwidget.h"
#include "ui_mainwidget.h"
#include <QScrollBar>

MainWidget::MainWidget(Settings *_settings, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    ui->textBrowser->verticalScrollBar()->hide();
    ui->textBrowser->horizontalScrollBar()->hide();

    settings = _settings;

    pageData = new QString[3];

    int _width  = 160;
    int _height = 55;
    realmWidget = new RealmMiniWidget*[4];
    for (int i = 0; i < 4; ++i)
    {
        realmWidget[i] = new RealmMiniWidget(ui->realmsWidget);
        connect(realmWidget[i], SIGNAL(clicked(RealmMiniWidget*)), this, SLOT(realmMiniWidget_clicked(RealmMiniWidget*)));
    }
    realmWidget[1]->setGeometry(_width,0,_width,_height);
    realmWidget[2]->setGeometry(0,_height,_width,_height);
    realmWidget[3]->setGeometry(_width,_height,_width,_height);

    connect(&page1, SIGNAL(DataCollected(ForumMgr*)), this, SLOT(Update1Forum()));
    connect(&page2, SIGNAL(DataCollected(ForumMgr*)), this, SLOT(Update2Forum()));
    connect(&page3, SIGNAL(DataCollected(ForumMgr*)), this, SLOT(Update3Forum()));
    connect(&onlineParser, SIGNAL(ParsingDone()), this, SLOT(UpdateOnline()));
    connect(&updateTimer, SIGNAL(timeout()), this, SLOT(UpdateAll()));

    UpdateAll();

    currCharIndex = 0;
    realmPage = 1;
    currOnlineIndex = settings->getDefaultOnlineShown();

    updateTimer.start(settings->getUpdateTime());
    characters = new Characters(settings->getGamePath());

    newUpdate = false;

    ui->synchButton->setVisible(false);
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
    if (settings->getLastServerUpdate() != page2.getLastPostDate() || settings->getLastServerUpdate().date() == QDate::currentDate())
    {
        settings->setLastServerUpdate(page2.getLastPostDate());
        newUpdate = true;
    }
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

void MainWidget::UpdateOnline(int page)
{
    if (!page)
        page = realmPage;

    int maxPages = (onlineParser.getRealmsCount() % 4) ? 1 : 0;
    maxPages += onlineParser.getRealmsCount() / 4;

    if (maxPages < page)
        return;

    realmPage = page;
    for (int widget = 0,  parser = (page - 1) * 4; parser < page * 4; ++widget, ++parser )
    {
        if (parser >= onlineParser.getRealmsCount())
        {
            realmWidget[widget]->hide();
            continue;
        }
        int online = onlineParser.getRealmsInfo()[parser].online;
        QString realmName = onlineParser.getRealmsInfo()[parser].realmName;

        if (settings->getMaxOnlineByIndex(parser) < online)
        {
            settings->setMaxOnlineByIndex(parser, online);
            settings->SaveSettings();
        }

        realmWidget[widget]->show();
        realmWidget[widget]->setRealmName(realmName);
        realmWidget[widget]->setMaxOnline(settings->getMaxOnlineByIndex(parser));
        realmWidget[widget]->setOnline(online);
        realmWidget[widget]->setIndex(parser);
    }
    UpdateMainRealmWidget(currOnlineIndex);
}

void MainWidget::on_prevRealmsButton_clicked()
{
    UpdateOnline(realmPage - 1);
}

void MainWidget::on_nextRealmsButton_clicked()
{
    UpdateOnline(realmPage + 1);
}

void MainWidget::UpdateAll()
{
    page1.setForumPage(settings->getForumView1());
    page2.setForumPage(settings->getForumView2());
    page3.setForumPage(settings->getForumView3());
    onlineParser.startParse();
    updateTimer.start();
}

void MainWidget::realmMiniWidget_clicked(RealmMiniWidget *widg)
{
    UpdateMainRealmWidget(widg->getIndex());
}

void MainWidget::UpdateMainRealmWidget(int realmIndex)
{
    if (onlineParser.getRealmsCount() < realmIndex)
        return;

    currOnlineIndex = realmIndex;
    QString realmName = onlineParser.getRealmsInfo()[realmIndex].realmName;
    currRealmName = realmName;
    int online = onlineParser.getRealmsInfo()[realmIndex].online;

    if (settings->getMaxOnlineByIndex(realmIndex) < online)
    {
        settings->setMaxOnlineByIndex(realmIndex, online);
        settings->SaveSettings();
    }

    ui->onlineBar->setMaximum(settings->getMaxOnlineByIndex(realmIndex));
    ui->onlineBar->setValue(online);
    ui->realmName->setText(realmName);
    setRealmOnline(onlineParser.getRealmsInfo()[realmIndex].isON);

    characters->SearchByRealm(realmName);
    int charIndex = characters->getIndexByName(settings->getDefaultCharName());
    UpdateCharacters(charIndex);
}

void MainWidget::on_saveRealmButton_clicked()
{
    settings->setDefaultOnlineShown(currOnlineIndex);
    settings->SaveSettings();
}

void MainWidget::setRealmOnline(bool online)
{
    QString onlineImg = (online) ? "on.png" : "off.png";
    QString style = QString("QWidget {"
                                "background-color: qlineargradient(spread:pad, x1:1, y1:1, x2:1, y2:0, stop:0.00966184 #3d0d07, stop:0.637681 #6b2403);"
                                "color: #dcb100;"
                                "border:1px solid;"
                                "border-color: qlineargradient(spread:pad, x1:1, y1:1, x2:1, y2:0, stop:0.00966184 #531108, stop:0.637681 #953203);"
                                "image: url(:/img/%1);"
                            "}"
                            "QWidget:hover{"
                                    "background-color: qlineargradient(spread:pad, x1:1, y1:1, x2:1, y2:0, stop:0.00966184 #3d0d07, stop:0.637681 #802b04);"
                                    "border-color: qlineargradient(spread:pad, x1:1, y1:1, x2:1, y2:0, stop:0.00966184 #64140a, stop:0.637681 #aa3c04);"
                                    "color:white;"
                            "}").arg(onlineImg);

    ui->statusWidget->setStyleSheet(style);
}

void MainWidget::setCharClass(QString _class)
{
    QString style = QString("QWidget {"
                                    "background-color: qlineargradient(spread:pad, x1:1, y1:1, x2:1, y2:0, stop:0.00966184 #3d0d07, stop:0.637681 #6b2403);"
                                    "color: #dcb100;"
                                    "border:1px solid;"
                                    "border-color: qlineargradient(spread:pad, x1:1, y1:1, x2:1, y2:0, stop:0.00966184 #531108, stop:0.637681 #953203);"
                            "}"
                            "QWidget:hover {"
                                    "background-color: qlineargradient(spread:pad, x1:1, y1:1, x2:1, y2:0, stop:0.00966184 #3d0d07, stop:0.637681 #802b04);"
                                    "border-color: qlineargradient(spread:pad, x1:1, y1:1, x2:1, y2:0, stop:0.00966184 #64140a, stop:0.637681 #aa3c04);"
                                    "color:white;"
                            "}"
                            "QWidget {"
                                    "image: url(:/img/class/%1.png);"
                            "}").arg(_class);

    ui->classWidget->setStyleSheet(style);
}

void MainWidget::UpdateCharacters(int index, QString realm)
{
    if (!realm.isEmpty())
        characters->SearchByRealm(realm);

    if (index >= characters->getCharactersCount())
        index = 0;
    currCharIndex = index;

    QString name = characters->getCharacterByIndex(index).name;
    QString level = QString::number(characters->getCharacterByIndex(index).level);
    QString charClass = characters->getCharacterByIndex(index).charClass;
    ui->charNameLabel->setText(name);
    ui->levelLabel->setText(level);
    setCharClass(charClass);
}

void MainWidget::on_nextCharButton_clicked()
{
    UpdateCharacters(currCharIndex + 1);
}

void MainWidget::on_saveCharButton_clicked()
{
    settings->setDefaultCharName(ui->charNameLabel->text());
    settings->SaveSettings();
}
