#ifndef FORUMMGR_H
#define FORUMMGR_H

#include <QObject>
#include "src/parsers/circle/forumdisplayparser.h"
#include "src/parsers/circle/forumthreadparser.h"

class ForumMgr : public QObject
{
    Q_OBJECT
private:
    QUrl forumPage;
    ForumDisplayParser forum;
    ForumThreadParser  thread;

    QString htmlData;

public:
    explicit ForumMgr(QObject *parent = 0);

    void setForumPage(QUrl url);
    QString getHtmlData() { return htmlData; }

signals:
    void DataCollected(ForumMgr *fMgr);

private slots:
    void startThreadLoad();
    void DataToHtml();
public slots:
    void Update();
};

#endif // FORUMMGR_H
