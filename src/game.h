#ifndef GAME_H
#define GAME_H

#include <QProcess>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFileInfoList>
#include <QTextStream>

class Game
{
public:
    Game();
    Game(QString _gamePath, QString _realmList);
    Game(QString _gamePath, QString _realmList, bool _clearCache, bool _clearWtf);
    void setGamePath(QString _path)         { gamePath = _path;       }
    void setRealmList(QString _realmList)   { realmList = _realmList; }
    //clear before start the game
    void setClearWtfBefore(bool clear)      { clearWtf = clear;       }
    void setClearCacheBefore(bool clear)    { clearCache = clear;     }

    void ClearCache();
    void ClearWtf();
    void StartGame();
    void ChangeRealmList(QString _realmList);
    static bool gameExists(QString _gamePath);
private:
    bool removeDir(const QString &dirName);
    void WinStart();
    void NixStart();

    QString gamePath;
    QString realmList;

    bool clearCache;
    bool clearWtf;
};

#endif // GAME_H
