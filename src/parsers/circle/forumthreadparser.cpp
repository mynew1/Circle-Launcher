#include "forumthreadparser.h"

ForumThreadParser::ForumThreadParser(QObject *parent) :
    QObject(parent)
{
    connect(&parser, SIGNAL(DataCollected(Parser*)), this, SLOT(Parse()));
    postsCount = 0;
}

void ForumThreadParser::setPage(QUrl url)
{
    parser.SetParseUrl(url);
}

void ForumThreadParser::Parse()
{
    postsCount = ParsePostsCount();
    posts = new Post[postsCount];

    ParseAuthors();
    ParseDatesTimes();
    ParseMessages();

    emit ParsingDone();
}

int ForumThreadParser::ParsePostsCount()
{
    if (parser.SearchSegments("<!-- message -->", "<!-- / message -->"))
        return parser.SegmentsCount();
    return 0;
}

void ForumThreadParser::ParseAuthors()
{
    int i = 0;
    if (parser.SearchSegments("<a class=\"bigusername\" href=\"member.php?u=", "</a>"))
        do {
            QString author = Parser::SrchSegInString(parser.GetSegment(), ";\">","</span>");
            posts[i].author = author;
            i++;
        } while (parser.NextSegment());
}

void ForumThreadParser::ParseDatesTimes()
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    int i = 0;
    if (parser.SearchSegments("<!-- status icon and date -->", "<!-- / status icon and date -->"))
        do {
            QString dateStr = Parser::SrchSegInString(parser.GetSegment().simplified(), "</a> ", ", ");
            QString timeStr = Parser::SrchSegInString(parser.GetSegment().simplified(), ", ");

            if (dateStr == tr("Сегодня"))
                dateStr = QDate::currentDate().toString("dd.MM.yyyy");

            if (dateStr == tr("Вчера"))
            {
                int day   = QDate::currentDate().day() - 1;
                int month = QDate::currentDate().month();
                int year  = QDate::currentDate().year();
                dateStr   = QDate(year, month, day).toString("dd.MM.yyyy");
            }

            QStringList listDMY = dateStr.split(".");
            QStringList listHM  = timeStr.split(":");
            QDate date(listDMY.at(2).toInt(), listDMY.at(1).toInt(), listDMY.at(0).toInt());
            QTime time(listHM.at(0).toInt(), listHM.at(1).toInt());

            QDateTime dataTime(date,time);
            posts[i].dateTime = dataTime;

            i++;
        } while (parser.NextSegment());
}

void ForumThreadParser::ParseMessages()
{
    int i = 0;
    if (parser.SearchSegments("<!-- message -->", "<!-- / message -->"))
        do {
            posts[i].message = parser.GetSegment().simplified();
            i++;
        } while (parser.NextSegment());
}
