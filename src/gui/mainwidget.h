#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QPushButton>
#include "src/parsers/forummgr.h"
#include "src/gui/realmminiwidget.h"
#include "src/parsers/circle/circleparseonline.h"
#include "src/settings.h"
#include "src/characters.h"
#include <QTimer>

namespace Ui {
    class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(Settings *_settings, QWidget *parent = 0);
    ~MainWidget();
    bool isNewUpdate() { return newUpdate; }

private:
    Ui::MainWidget *ui;
    ForumMgr page1;
    ForumMgr page2;
    ForumMgr page3;
    Settings *settings;
    CircleParseOnline onlineParser;
    Characters *characters;
    uint realmPage;

    QString *pageData;

    int currOnlineIndex;
    QString currRealmName;
    int currCharIndex;

    RealmMiniWidget **realmWidget;
    QTimer updateTimer;

    void setRealmOnline(bool online);
    void setCharClass(QString _class);

    bool newUpdate;

private slots:
    void UpdateAll();

    void Update1Forum();
    void Update2Forum();
    void Update3Forum();
    void UpdateOnline(int page = 0);
    void UpdateMainRealmWidget(int realmIndex);
    void UpdateCharacters(int index, QString realm = "");

    void ActiveForumButton(QPushButton *button);
    void UnActiveForumButton(QPushButton *button);

    void on_forumButton1_clicked();
    void on_forumButton2_clicked();
    void on_forumButton3_clicked();
    void on_prevRealmsButton_clicked();
    void on_nextRealmsButton_clicked();
    void realmMiniWidget_clicked(RealmMiniWidget*);
    void on_saveRealmButton_clicked();

    void on_nextCharButton_clicked();
    void on_saveCharButton_clicked();
};

#endif // MAINWIDGET_H
