#include "curseparsedownloadurl.h"

CurseParseDownloadUrl::CurseParseDownloadUrl(QObject *parent) :
    QObject(parent)
{
}

void CurseParseDownloadUrl::setAddonUrl(QUrl url)
{
    connect(&parser, SIGNAL(DataCollected(Parser*)), this, SLOT(ParseDownloadPage()));
    emit ParsingBegin();
    addonUrl = url;
    parser.SetParseUrl(addonUrl);
}

QUrl CurseParseDownloadUrl::ParseDownloadPage()
{
    if (parser.SearchSegments("FlashVars=\"url=","&fileId"))
        downloadUrl = QUrl(parser.GetSegment());
    qDebug() << downloadUrl;
    emit ParsingDone();
    return downloadUrl;
}
