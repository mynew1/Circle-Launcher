#ifndef FORUMDISPLAYPARSER_H
#define FORUMDISPLAYPARSER_H

#include <QObject>
#include <QDateTime>
#include "src/parsers/parser.h"

struct ThreadInfo {
    QString threadTitle;
    QString author;
    QDateTime lastPostDate;
    QUrl threadUrl;
};

class ForumDisplayParser : public QObject
{
    Q_OBJECT

private:
    Parser parser;
    ThreadInfo *threads;
    int threadsCount;

public:
    explicit ForumDisplayParser(QObject *parent = 0);

    void setPage(QUrl url);

    int getThreadsCount()        { return threadsCount; }
    ThreadInfo* getThreadsInfo() { return threads;      }

    ThreadInfo getLastThread();

signals:
    void ParsingDone();

private slots:
    void Parse();

    int  ParseThreadsCount();
    void ParseTitles();
    void ParseAuthors();
    void ParseLastPostDate();
    void ParseUrl();
};

#endif // FORUMDISPLAYPARSER_H
