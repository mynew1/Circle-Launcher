#include "game.h"

Game::Game()
{
    clearCache = false;
    clearWtf   = false;
}

Game::Game(QString _gamePath, QString _realmList)
{
    Game();
    gamePath = _gamePath;
    realmList = _realmList;
}

Game::Game(QString _gamePath, QString _realmList, bool _clearCache, bool _clearWtf)
{
    gamePath = _gamePath;
    realmList = _realmList;
    clearCache = _clearCache;
    clearWtf = _clearWtf;
}

bool Game::removeDir(const QString &dirName)
{
    bool result = true;
    QDir dir(dirName);

    if (dir.exists(dirName)) {
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot
                                                    | QDir::System  | QDir::Hidden
                                                    | QDir::AllDirs | QDir::Files,
                                                    QDir::DirsFirst))
        {
            result = (info.isDir())? removeDir(info.absoluteFilePath()):/*
                                     result =*/ QFile::remove(info.absoluteFilePath());

            if (!result)
                return result;
        }
        result = dir.rmdir(dirName);
    }

    return result;
}

void Game::ClearCache()
{
    removeDir(gamePath+"/Cache/ADB");
    removeDir(gamePath+"/Cache/WDB");
}

void Game::ClearWtf()
{
    removeDir(gamePath+"/Wtf/");
}

void Game::ChangeRealmList(QString _realmList)
{
    QString path;

    if(QFile::exists(gamePath + "/Data/ruRU/realmlist.wtf"))
        path = gamePath + "/Data/ruRU/realmlist.wtf";

    if(QFile::exists(gamePath + "/Data/enGB/realmlist.wtf"))
        path = gamePath + "/Data/enGB/realmlist.wtf";

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << "set realmlist " << _realmList;
}

void Game::NixStart()
{
    if (QFile::exists(gamePath + "/Wow.exe"))
        QProcess::startDetached("wine", QStringList(gamePath + "/wow.exe"));
}

void Game::WinStart()
{
    if (QFile::exists(gamePath + "/Wow.exe"))
        QProcess::startDetached(gamePath + "/Wow.exe");
}

void Game::StartGame()
{
    if (clearCache)
        ClearCache();

    if (clearWtf)
        ClearWtf();

    ChangeRealmList(realmList);

#ifdef Q_OS_WIN32
    WinStart();
#else
    NixStart();
#endif
}

bool Game::gameExists(QString _gamePath)
{
    return QFile::exists(_gamePath + "/Wow.exe");
}
