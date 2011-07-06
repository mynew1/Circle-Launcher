#include "curseparsedownloadurl.h"

CurseParseDownloadUrl::CurseParseDownloadUrl(QObject *parent) :
    QObject(parent)
{
    isParsingReady = true;
}

void CurseParseDownloadUrl::setAddonUrl(QUrl url)
{
//    if (!isParsingReady)
//        return;

    connect(&parser, SIGNAL(DataCollected(Parser*)), this, SLOT(ParseDownloadPage()));
    emit ParsingBegin();
    addonUrl = url;
    isParsingReady = false;
    parser.SetParseUrl(addonUrl);
}

QUrl CurseParseDownloadUrl::ParseDownloadPage()
{
    if (parser.SearchSegments("FlashVars=\"url=","&fileId"))
        downloadUrl = QUrl(parser.GetSegment());
    isParsingReady = true;
    emit ParsingDone();
    return downloadUrl;
}
