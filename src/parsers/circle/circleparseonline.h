#ifndef CIRCLEPARSEONLINE_H
#define CIRCLEPARSEONLINE_H

#include <QObject>
#include "src/parsers/parser.h"

struct RealmInfo {
    QString realmName;
    int online;
    bool isON;
};

class CircleParseOnline : public QObject
{
    Q_OBJECT

private:
    Parser     parser;
    RealmInfo *realms;
    int        realmsCount;

public:
    explicit CircleParseOnline(QObject *parent = 0);

    void startParse(QUrl url = QUrl("http://wowcircle.com/online.htm"));

    int getRealmsCount()        { return realmsCount; }
    RealmInfo* getRealmsInfo()  { return realms;      }

signals:
    void ParsingDone();

private slots:
    void Parse();

    int  ParseRealmsCount();
    void ParseOnline();
    void ParseNames();
    void ParseStatus(); //isOn
};

#endif // CIRCLEPARSEONLINE_H
