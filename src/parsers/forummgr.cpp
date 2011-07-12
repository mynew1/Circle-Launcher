#include "forummgr.h"

ForumMgr::ForumMgr(QObject *parent) :
    QObject(parent)
{
    connect(&forum, SIGNAL(ParsingDone()), this, SLOT(startThreadLoad()));
    connect(&thread, SIGNAL(ParsingDone()), this, SLOT(DataToHtml()));
}

void ForumMgr::setForumPage(QUrl url)
{
    forumPage = url;
    forum.setPage(url);
}

void ForumMgr::startThreadLoad()
{
    thread.setPage(forum.getLastThread().threadUrl);
}

void ForumMgr::DataToHtml()
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QString style = "<style>a { color: yellow; } a:visited { color: red; }</style>";

    QString title  = tr("<p align=\"center\">Тема: <a href=\"") +
                     forum.getLastThread().threadUrl.toString() +"\">" +
                     forum.getLastThread().threadTitle + "</a></p>";

    QString author = tr("<p align=\"center\">Автор: ") +
                     forum.getLastThread().author + "</p>";

    htmlData = style + title + author + thread.getFirstPost().message;

    emit DataCollected(this);
}

void ForumMgr::Update()
{
    setForumPage(forumPage);
}
