#include "cursesearchparser.h"

void CurseSearchParser::Init()
{
    parser = new Parser();

    addonsNameCursor     = 0;
    addonsPageCursor     = 0;
    addonsCategoryCursor = 0;
    addonsHitsCursor     = 0;
    addonsRatingCursor   = 0;

    collectedDataCount   = 0;

    _isParsingReady = true;

    addonsCount = 0;
    connect(parser,SIGNAL(DataCollected(Parser*)),this,SLOT(Parse()));
}

CurseSearchParser::CurseSearchParser()
{
    Init();
}

CurseSearchParser::CurseSearchParser(QString indexTerm)
{
    setIndexTerm(indexTerm);
}

CurseSearchParser::CurseSearchParser(QUrl url)
{
    setUrl(url);
}

CurseSearchParser::~CurseSearchParser()
{
    delete addonInfo;
    delete parser;
}

QUrl CurseSearchParser::GenerateUrl(QString indexTerm)
{
    QString tmp = indexTerm;
    return QUrl("http://wow.curse.com/downloads/wow-addons/SearchResults.aspx?q="+tmp.replace(" ","%20"));
}

void CurseSearchParser::setIndexTerm(QString indexTerm)
{
    Init();
    _isParsingReady = false;
    parser->Abort();
    parseUrl = GenerateUrl(indexTerm);
    parser->SetParseUrl(parseUrl);
}

void CurseSearchParser::setUrl(QUrl url)
{
    Init();
    _isParsingReady = false;
    parser->Abort();
    parseUrl = url;
    parser->SetParseUrl(parseUrl);
}

void CurseSearchParser::Parse()
{
    if(isNotFound())
    {
        _isParsingReady = true;
        emit ParsingDone();
        return;
    }

    pageCount = GetPageCount();
    addonsCount = pageCount * 40;
    addonInfo = new AddonSearchInfo[addonsCount];

    CollectData(parser);
    if (pageCount  > 1)
    {
        Parser *pages = new Parser[pageCount + 1];
        for (int i = 2; i < pageCount + 1; ++i)
        {
            connect(&pages[i],SIGNAL(DataCollected(Parser*)),this,SLOT(CollectData(Parser*)));
            pages[i].SetParseUrl(QUrl(parseUrl.toString()+QString("&PageIndex=%1").arg(i)));
        }
    }
}

int CurseSearchParser::GetPageCount()
{
    parser->SearchSegments("<li class=\"total\">", " page(s)</li>");
    return parser->GetSegment().toInt();
}

void CurseSearchParser::ParseAddonsName(Parser *p)
{
    int i = addonsNameCursor;
    p->SearchSegments("<td class=\"name first\">", "</td>");
    do {
        addonInfo[i].name = Parser::SrchSegInString(p->GetSegment(),".aspx\">", "</a>");
        i++;
    } while (p->NextSegment());
    addonsNameCursor = i;
}

void CurseSearchParser::ParseAddonsPage(Parser *p)
{
    int i = addonsPageCursor;
    p->SearchSegments("<td class=\"name first\">", "</td>");
    do {
        addonInfo[i].page = Parser::SrchSegInString(p->GetSegment(),"href=\"", "\">");
        i++;
    } while (p->NextSegment());
    addonsPageCursor = i;
}

void CurseSearchParser::ParseAddonsCategory(Parser *p)
{
    int i = addonsCategoryCursor;
    p->SearchSegments("<td class=\"category\">", "</td>");
    do {
        addonInfo[i].category = p->GetSegment().remove("and ").split(", ");
        i++;
    } while (p->NextSegment());
    addonsCategoryCursor = i;
}


void CurseSearchParser::ParseAddonsHits(Parser *p)
{
    int i = addonsHitsCursor;
    p->SearchSegments("<td class=\"hits\">", "</td>");
    do {
        addonInfo[i].hits = p->GetSegment().remove(",").toInt();
        i++;
    } while (p->NextSegment());
    addonsHitsCursor = i;
}

void CurseSearchParser::ParseAddonsRating(Parser *p)
{
    int i = addonsRatingCursor;
    p->SearchSegments("<td class=\"rating\">", "</td>");
    do {
        addonInfo[i].rating = Parser::SrchSegInString(p->GetSegment(),"value=\"","\" ").toDouble();
        i++;
    } while (p->NextSegment());
    addonsRatingCursor = i;
}

bool CurseSearchParser::isNotFound()
{
    return parser->SearchSegments("<div class=\"CommonMessageError\">", "</div>");
}

void CurseSearchParser::CollectData(Parser *p)
{
    ParseAddonsName(p);
    ParseAddonsPage(p);
    ParseAddonsCategory(p);
    ParseAddonsHits(p);
    ParseAddonsRating(p);

    collectedDataCount++;
    if (collectedDataCount >= pageCount)
    {
        addonsCount = addonsNameCursor;
        _isParsingReady = true;
        //debug
//        for (int i = 0; i < addonsCount; ++i)
//            qDebug() << QString("Addon %1: hits - %2, rating - %3, page - %4").arg(addonInfo[i].name).arg(addonInfo[i].hits).arg(addonInfo[i].rating).arg(addonInfo[i].page.toString());

        emit ParsingDone();
    }
}
