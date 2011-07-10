#include "circleparseonline.h"

CircleParseOnline::CircleParseOnline(QObject *parent) :
    QObject(parent)
{
    connect(&parser, SIGNAL(DataCollected(Parser*)), this, SLOT(Parse()));
    realmsCount = 0;
}

void CircleParseOnline::startParse(QUrl url)
{
    parser.SetParseUrl(url);
}

void CircleParseOnline::Parse()
{
    realmsCount = ParseRealmsCount();
    realms      = new RealmInfo[realmsCount];

    ParseOnline();
    ParseNames();
    ParseStatus();

    emit ParsingDone();
}

int CircleParseOnline::ParseRealmsCount()
{
    if (parser.SearchSegments("<div class=\"info\">","<br>"))
        return parser.SegmentsCount();
    return 0;
}

void CircleParseOnline::ParseNames()
{
    int i = 0;
    if (parser.SearchSegments("<div class=\"info\">","<br>"))
        do {
            realms[i].realmName = parser.GetSegment();
            i++;
        } while (parser.NextSegment());
}

void CircleParseOnline::ParseOnline()
{
    int i = 0;
    if (parser.SearchSegments("<div class=\"online\">","</div>"))
        do {
            realms[i].online = parser.GetSegment().toInt();
            i++;
        } while (parser.NextSegment());
}

void CircleParseOnline::ParseStatus()
{
    int i = 0;
    if (parser.SearchSegments("<div class=\" ahx-","  showt\""))
        do {
            realms[i].isON = (parser.GetSegment() == "on") ? true : false;
            i++;
        } while (parser.NextSegment());

}
