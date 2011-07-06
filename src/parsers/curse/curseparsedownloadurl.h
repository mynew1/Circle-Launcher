#ifndef PARSEDOWNLOADURL_H
#define PARSEDOWNLOADURL_H

#include <QObject>
#include "src/parsers/parser.h"

class CurseParseDownloadUrl : public QObject
{
    Q_OBJECT
private:
    Parser parser;
    QUrl addonUrl;
    QUrl downloadUrl;
    bool isParsingReady;

public:
    explicit CurseParseDownloadUrl(QObject *parent = 0);

    void setAddonUrl(QUrl url);
    QUrl getDownloadUrl() { return downloadUrl; }

signals:
    void ParsingBegin();
    void ParsingDone();
private slots:
    QUrl ParseDownloadPage();
};

#endif // PARSEDOWNLOADURL_H

