#ifndef PARSER_H
#define PARSER_H

#include "src/httpdownload.h"

class Parser : public QObject
{
    Q_OBJECT

private:
    QUrl parseUrl;
    bool dataReady;

    quint32 dataCursor;
    QString currentSegment;
    bool isSegmentExists;
    QString startSegment;
    QString endSegment;

    static QString CutString(QString str, qint32 begIndex, qint32 endIndex);

protected:
    HttpDownload *download;
    QString parsingData;
    bool IsDataReady() { return dataReady; }

public:
    Parser();
    Parser(QUrl url);
    ~Parser();

    void setParsingData(QString _data) { parsingData = _data; }

    QUrl GetParseUrl() { return parseUrl; }
    void SetParseUrl(QUrl _url);
    void Abort();

    bool SearchSegments(QString startSegment, QString endSegment);
    int SegmentsCount();
    bool NextSegment();
    QString GetSegment() { return currentSegment; };

    static QString SrchSegInString(QString data, QString begin, QString end = "");

    void DeleteStrFromData(QString str);

signals:
    void DataCollected(Parser *p);

protected slots:
    void EmitDataCollected();
};

#endif // PARSER_H
