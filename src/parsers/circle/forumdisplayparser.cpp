#include "forumdisplayparser.h"
#include <QTextCodec>

ForumDisplayParser::ForumDisplayParser(QObject *parent) :
    QObject(parent)
{

    connect(&parser, SIGNAL(DataCollected(Parser*)), this, SLOT(Parse()));
    threadsCount = 0;
}

void ForumDisplayParser::setPage(QUrl url)
{
    parser.SetParseUrl(url);
}

void ForumDisplayParser::Parse()
{
    //get only threads data
    parser.SearchSegments("<!-- show threads -->", "<!-- end show threads -->");
    parser.setParsingData(parser.GetSegment());

    threadsCount = ParseThreadsCount();
    threads      = new ThreadInfo[threadsCount];

    ParseUrl();
    ParseTitles();
    ParseAuthors();
    ParseLastPostDate();

    emit ParsingDone();
}

int ForumDisplayParser::ParseThreadsCount()
{
    if (parser.SearchSegments("id=\"thread_title_", "\">"))
        return parser.SegmentsCount();
    return 0;
}

void ForumDisplayParser::ParseUrl()
{
    if (parser.SearchSegments("id=\"thread_title_", "\">"))
    {
        int i = 0;
        do {
            QString url = "http://forum.wowcircle.com/showthread.php?t=";
            threads[i].threadUrl = QUrl(url + parser.GetSegment());
            ++i;
        } while (parser.NextSegment());
    }
}

void ForumDisplayParser::ParseAuthors()
{
    if (parser.SearchSegments("<div class=\"smallfont\">", "</div>"))
    {
        int i = 0;
        do {
            threads[i].author = Parser::SrchSegInString(parser.GetSegment(), "'_self')\">","</span>");
            ++i;
        } while (parser.NextSegment());
    }
}

void ForumDisplayParser::ParseLastPostDate()
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    if (parser.SearchSegments("<div class=\"smallfont\" style=\"text-align:right; ", "</span>"))
    {
        int i = 0;
        do {
            QString dateStr = Parser::SrchSegInString(parser.GetSegment().simplified(), "nowrap\"> ", " <span");
            QString timeStr = Parser::SrchSegInString(parser.GetSegment().simplified(), "class=\"time\">");
            if (dateStr == tr("Сегодня"))
                dateStr = QDate::currentDate().toString("dd.MM.yyyy");

            if (dateStr == tr("Вчера"))
            {
                int day;
                int month;
                if (QDate::currentDate().day() == 1) {
                    QDate _date(QDate::currentDate().year(), QDate::currentDate().month() - 1, 1);
                    day   = _date.daysInMonth();
                    month = _date.month();
                } else {
                    day   = QDate::currentDate().day() - 1;
                    month = QDate::currentDate().month();
                }
                int year  = QDate::currentDate().year();
                dateStr   = QDate(year, month, day).toString("dd.MM.yyyy");
            }

            QStringList listDMY = dateStr.split(".");
            QStringList listHM  = timeStr.split(":");
            QDate date;
            QTime time;

            if (listDMY.size() == 3)
                date.setDate(listDMY.at(2).toInt(), listDMY.at(1).toInt(), listDMY.at(0).toInt());
            if (listHM.size() == 2)
                time.setHMS(listHM.at(0).toInt(), listHM.at(1).toInt(), 0);


            QDateTime dataTime(date,time);
            threads[i].lastPostDate = dataTime;

            ++i;
        } while (parser.NextSegment());
    }
}

void ForumDisplayParser::ParseTitles()
{
    QStringList threadsId;
    if (parser.SearchSegments("id=\"thread_title_", "\">"))
    {
        do {
            threadsId << parser.GetSegment();
        } while (parser.NextSegment());
    }
    int i = 0;
    while (i < threadsCount)
    {
        parser.SearchSegments("id=\"thread_title_"+threadsId.at(i)+"\">", "</a>");
        threads[i].threadTitle = parser.GetSegment();
        i++;
    }
}

ThreadInfo ForumDisplayParser::getLastThread()
{
    if (!threadsCount)
        return ThreadInfo();

    QDateTime maxTime = threads[0].lastPostDate;
    int maxIndex      = 0;

    for (int i = 1; i < threadsCount; ++i)
        if (maxTime < threads[i].lastPostDate)
        {
            maxIndex = i;
            maxTime  = threads[i].lastPostDate;
        }

    return threads[maxIndex];
}
