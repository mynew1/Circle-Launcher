#ifndef CURSEADDONDETAILS_H
#define CURSEADDONDETAILS_H

#include <QObject>
#include "src/parsers/parser.h"

enum cursorsEnum {
    CURSOR_FILE_NAME,
    CURSOR_IS_RELEASE,
    CURSOR_GAME_VERSION,
    CURSOR_DOWNLOADS,
    CURSOR_DATE,
    CURSOR_DOWNLOAD_PAGE,
    CURSORS_COUNT
};

struct AddonDownloadInfo
{
    QString filename;
    bool isRelease;
    QString gameVersion;
    int downloads;
    QString date;
    QUrl downloadPage;
};

class CurseAddonDetails : public QObject
{
    Q_OBJECT
private:
    QUrl addonUrl;
    Parser *parser;

    int cursors[CURSORS_COUNT];
    int downloadsCount;

    bool isParsingInProgress;

    AddonDownloadInfo *downloadInfo;
public:
    explicit CurseAddonDetails(QObject *parent = 0);
    CurseAddonDetails(QUrl url);
    void Init();

    void setAddonUrl(QUrl url);

    AddonDownloadInfo* getDownloadInfo(){ return downloadInfo; }
    int getCountDownloadInfo(){ return downloadsCount; }

signals:
    void ParsingBegin();
    void ParsingDone();

private slots:
    void Parse();
    int CountAddonVersions();

    void CollectPageData(Parser *page);

    void ParseFileNames(Parser *page);
    void ParseIsRelease(Parser *page);
    void ParseGameVersions(Parser *page);
    void ParseDownloadsCounts(Parser *page);
    void ParseDates(Parser *page);
    void ParseDownloadPages(Parser *page);
};

#endif // CURSEADDONDETAILS_H
