#ifndef CURSESEARCHPARSER_H
#define CURSESEARCHPARSER_H
#include "src/parsers/parser.h"

struct AddonSearchInfo
{
    QString name;
    QStringList category;
    int hits;
    double rating;
    QUrl page;
};


//TODO: create class for pages
class CurseSearchParser : public QObject
{
    Q_OBJECT

private:
    void Init();
    Parser *parser;
    QUrl parseUrl;

    AddonSearchInfo *addonInfo;
    int addonsCount;
    bool _isParsingReady;

    //cursors
    int addonsNameCursor;
    int addonsPageCursor;
    int addonsCategoryCursor;
    int addonsHitsCursor;
    int addonsRatingCursor;

    int collectedDataCount;
    int pageCount;

public:
    CurseSearchParser();
    CurseSearchParser(QString indexTerm);
    CurseSearchParser(QUrl url);

    void setIndexTerm(QString indexTerm);
    void setUrl(QUrl url);

    static QUrl GenerateUrl(QString indexTerm);

    AddonSearchInfo* getAddonsInfo(){ return addonInfo; }
    int getAddonsInfoCount(){ return addonsCount; }

    bool isParsingReady() { return _isParsingReady; }

signals:
    void ParsingDone();

private slots:
//    void EmitParsingDone(){}

    void Parse();

    int GetPageCount();
    bool isNotFound();

    void ParseAddonsName(Parser *p);
    void ParseAddonsPage(Parser *p);
    void ParseAddonsCategory(Parser *p);
    void ParseAddonsHits(Parser *p);
    void ParseAddonsRating(Parser *p);

    void CollectData(Parser *p);
};

#endif // CURSESEARCHPARSER_H
