#include "parser.h"

Parser::Parser()
{
    download  = new HttpDownload();
    dataReady = false;
    isSegmentExists = false;
    connect(download, SIGNAL(downloadFinished()), this, SLOT(EmitDataCollected()));
}

Parser::Parser(QUrl url)
{
    dataReady = false;
    parseUrl = url;
    download->downloadFile(parseUrl);
}

void Parser::SetParseUrl(QUrl url)
{
    dataReady = false;
    parseUrl = url;
    download->downloadFile(parseUrl);
}

void Parser::Abort()
{
    download->cancelDownload();
    Parser();
}

void Parser::EmitDataCollected()
{
    parsingData = download->getDownloadData();
    if (!parsingData.isNull())
    {
        dataReady   = true;
        emit DataCollected(this);
    }
}

bool Parser::SearchSegments(QString _startSegment, QString _endSegment)
{
    if (!IsDataReady())
    {
        qWarning() << "Data is not ready.";
        return false;
    }

    startSegment = _startSegment;
    endSegment   = _endSegment;

    dataCursor = 0;
    isSegmentExists = false;

    QString data(parsingData);
    qint32 startIndex = data.indexOf(startSegment, dataCursor);
    qint32 endIndex   = data.indexOf(endSegment, startIndex + startSegment.count());

    currentSegment.clear();

    if (startIndex > 0 && endIndex > 0)
    {
        currentSegment = CutString(data, startIndex + startSegment.count(), endIndex);
        isSegmentExists = true;
        dataCursor = endIndex + endSegment.count();
        return true;
    } else
        return false;
}

int Parser::SegmentsCount()
{
    if (!isSegmentExists)
        return 0;

    QString data(parsingData);
    qint32 startIndex;
    qint32 endIndex;
    qint32 tmpCusror = 0;
    int count = 0;
    do {
        startIndex = data.indexOf(startSegment, tmpCusror);
        endIndex   = data.indexOf(endSegment, startIndex + startSegment.count());
        tmpCusror  = endIndex + endSegment.count();
        count++;
    } while (startIndex > 0 && endIndex > 0);

    return count;
}

bool Parser::NextSegment()
{
    if (!isSegmentExists)
        return 0;

    QString data(parsingData);
    qint32 startIndex = data.indexOf(startSegment, dataCursor);
    qint32 endIndex   = data.indexOf(endSegment, startIndex + startSegment.count());

    currentSegment.clear();

    if (startIndex > 0 && endIndex > 0)
    {
        currentSegment = CutString(data, startIndex + startSegment.count(), endIndex);
        dataCursor = endIndex + endSegment.count();
        return true;
    } else {
        isSegmentExists = false;
        return false;
    }
}

QString Parser::SrchSegInString(QString data, QString begin, QString end /* = "" */)
{
    qint32 begIndex = data.indexOf(begin);
    qint32 endIndex =(end != "") ? data.indexOf(end, begIndex + begin.count()) : data.count();
    return (begIndex > 0 && endIndex > 0) ? Parser::CutString(data,begIndex + begin.count(),endIndex) : "";
}

QString Parser::CutString(QString str, qint32 begIndex, qint32 endIndex)
{
    if (endIndex - begIndex <= 0)
        return "";

    QString res;
    for (int i = begIndex; i < endIndex; ++i)
        res += str[i];
    return res;
}
