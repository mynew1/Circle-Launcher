#ifndef FORUMTHREADPARSER_H
#define FORUMTHREADPARSER_H

#include <QObject>
#include <QDateTime>
#include "src/parsers/parser.h"

struct Post {
    QString author;
    QDateTime dateTime;
    QString message;
};

class ForumThreadParser : public QObject
{
    Q_OBJECT
private:
    Parser parser;
    Post  *posts;
    int    postsCount;
    QUrl   threadPage;
public:
    explicit ForumThreadParser(QObject *parent = 0);

    void setPage(QUrl url);

    Post* getPosts()      { return posts;      }
    int   getPostsCount() { return postsCount; }
    Post  getFirstPost()  { return posts[0];   }
    QUrl  getThreadPage() { return threadPage; }
signals:
    void ParsingDone();

private slots:
    void Parse();

    int  ParsePostsCount();
    void ParseAuthors();
    void ParseDatesTimes();
    void ParseMessages();
};

#endif // FORUMTHREADPARSER_H
