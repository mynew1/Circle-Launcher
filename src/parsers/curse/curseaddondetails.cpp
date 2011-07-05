#include "curseaddondetails.h"

const int UrlOnPage = 5;

CurseAddonDetails::CurseAddonDetails(QObject *parent) :
    QObject(parent)
{
    Init();
}

CurseAddonDetails::CurseAddonDetails(QUrl url)
{
    setAddonUrl(url);
}

void CurseAddonDetails::Init()
{
    parser = new Parser();
    connect(parser, SIGNAL(DataCollected(Parser*)), this, SLOT(Parse()));
    isParsingInProgress = false;

    for (int i = 0; i < CURSORS_COUNT; ++i)
        cursors[i] = 0;
    downloadsCount = 0;
}

void CurseAddonDetails::setAddonUrl(QUrl url)
{
    if (isParsingInProgress)
        return;

    Init();
    addonUrl = url;
    isParsingInProgress = true;
    emit ParsingBegin();
    parser->SetParseUrl(addonUrl);
}

void CurseAddonDetails::Parse()
{
    downloadsCount = CountAddonVersions();
    downloadInfo = new AddonDownloadInfo[downloadsCount];
    CollectPageData(parser);

    int pagesCount = (downloadsCount % UrlOnPage) ? 1 : 0;
    pagesCount    += downloadsCount / UrlOnPage;
    if (pagesCount > 1)
    {
        Parser *pages = new Parser[pagesCount + 1];
        for (int i = 2; i < pagesCount + 1; ++i)
        {
            connect(&pages[i],SIGNAL(DataCollected(Parser*)),this,SLOT(CollectPageData(Parser*)));
            pages[i].SetParseUrl(QUrl(addonUrl.toString()+QString("?PageIndex=%1").arg(i)));
        }
    }
}

void CurseAddonDetails::CollectPageData(Parser *page)
{
    //delete trash
    page->DeleteStrFromData("<a class=\"mlink\" href=\"/downloads/wow-addons/details/addon/download/87101.aspx\">Addon Curse.com</a>");

    ParseFileNames(page);
    ParseIsRelease(page);
    ParseGameVersions(page);
    ParseDownloadsCounts(page);
    ParseDates(page);
    ParseDownloadPages(page);

    if (cursors[CURSOR_FILE_NAME] >= downloadsCount)
    {
        //debug
//        for (int i = 0; i < downloadsCount; ++i)
//            qDebug() << downloadInfo[i].gameVersion;
        isParsingInProgress = false;
        emit ParsingDone();
    }
}

int CurseAddonDetails::CountAddonVersions()
{
    int count = 0;
    if (parser->SearchSegments("Downloads <em>(", ")</em>"))
        count  = parser->GetSegment().toInt();
    return count;
}

void CurseAddonDetails::ParseFileNames(Parser *page)
{
    if (page->SearchSegments("<a class=\"mlink\" href=\"","</td>"))
        do {
            QString filename = Parser::SrchSegInString(page->GetSegment(),".aspx\">","</a>");
            downloadInfo[cursors[CURSOR_FILE_NAME]].filename = filename;
            cursors[CURSOR_FILE_NAME]++;
        } while (page->NextSegment());
}

void CurseAddonDetails::ParseIsRelease(Parser *page)
{
    if (page->SearchSegments("<a class=\"mlink\" href=\"","<td class=\"image\" style=\"cursor: pointer;\">"))
        do {
            QString releaseType = Parser::SrchSegInString(page->GetSegment(),"<td class=\"name\" style=\"cursor: pointer;\">","</td>");
            bool isRelease = releaseType.contains("Release");

            downloadInfo[cursors[CURSOR_IS_RELEASE]].isRelease = isRelease;
            cursors[CURSOR_IS_RELEASE]++;
        } while (page->NextSegment());
}

void CurseAddonDetails::ParseGameVersions(Parser *page)
{
    if (page->SearchSegments("<a class=\"mlink\" href=\"","<td class=\"image\" style=\"cursor: pointer;\">"))
        do {
            QString gameVersion = Parser::SrchSegInString(page->GetSegment(),"<td class=\"description\" style=\"cursor: pointer;\">","</td>");
            downloadInfo[cursors[CURSOR_GAME_VERSION]].gameVersion = gameVersion;
            cursors[CURSOR_GAME_VERSION]++;
        } while (page->NextSegment());
}

void CurseAddonDetails::ParseDownloadsCounts(Parser *page)
{
    if (page->SearchSegments("<a class=\"mlink\" href=\"","<td class=\"image\" style=\"cursor: pointer;\">"))
        do {
            QString downloads = Parser::SrchSegInString(page->GetSegment(),"<td class=\"hits\" style=\"cursor: pointer;\">","</td>");
            downloadInfo[cursors[CURSOR_DOWNLOADS]].downloads = downloads.remove(",").toInt();
            cursors[CURSOR_DOWNLOADS]++;
        } while (page->NextSegment());
}

void CurseAddonDetails::ParseDates(Parser *page)
{
    if (page->SearchSegments("<a class=\"mlink\" href=\"","<td class=\"image\" style=\"cursor: pointer;\">"))
        do {
            QString date = Parser::SrchSegInString(page->GetSegment(),"<td class=\"date\" style=\"cursor: pointer;\">","</td>");
            downloadInfo[cursors[CURSOR_DATE]].date = date;
            cursors[CURSOR_DATE]++;
        } while (page->NextSegment());
}

void CurseAddonDetails::ParseDownloadPages(Parser *page)
{
    if (page->SearchSegments("<a class=\"mlink\" href=\"","\">"))
        do {
            QString pageUrl = "http://wow.curse.com/"+page->GetSegment();
            downloadInfo[cursors[CURSOR_DOWNLOAD_PAGE]].downloadPage = QUrl(pageUrl);
            cursors[CURSOR_DOWNLOAD_PAGE]++;
        } while (page->NextSegment());
}
