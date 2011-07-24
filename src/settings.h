#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QDebug>
#include <QFile>

#define MAX_REALMS_ONLINE_COUNT 25

struct Realm {
    QString realmName;
    QString realmUrl;
};


class Settings
{
public:
    Settings();

    void LoadSettings();
    void SaveSettings();

    QString       getGamePath()  { return gamePath;          }
    QList<Realm>    getRealms()  { return realms;            }
    int  getDefaultRealmIndex()  { return defaultRealmIndex; }
    bool      getIsCleanCache()  { return isCleanCache;      }
    bool        getIsCleanWtf()  { return isCleanWtf;        }
    int getDefaultOnlineShown()  { return defaultOnlineShown;}
    QString     getForumView1()  { return forumView1;        }
    QString     getForumView2()  { return forumView2;        }
    QString     getForumView3()  { return forumView3;        }
    int         getUpdateTime()  { return updateTime;        }
    bool           IsFirstUse()  { return isFirstUse;        }
    bool    getExitAfterStart()  { return exitAfterStart;    }
    int getMaxOnlineByIndex(int index)  { return maxRealmsOnline[index]; }

    void setGamePath(QString path)        { gamePath = path; }
    void setDefaultRealmIndex(int index)  { defaultRealmIndex = index; }
    void setIsCleanCache(bool cache)      { isCleanCache = cache; }
    void setIsCleanWtf(bool wtf)          { isCleanWtf = wtf; }
    void setDefaultOnlineShown(int index) { defaultOnlineShown = index; }
    void setForumView1(QString url)       { forumView1 = url; }
    void setForumView2(QString url)       { forumView2 = url; }
    void setForumView3(QString url)       { forumView3 = url; }
    void setUpdateTime(int msec)          { updateTime = msec; }
    void setExitAfterStart(bool exit)     { exitAfterStart = exit; }
    void setMaxOnlineByIndex(int index, int value) { maxRealmsOnline[index] = value; }

    void addRealm(Realm realm)            { realms.append(realm); }
    void removeRealm(int index)           { realms.removeAt(index); }
private:
    QSettings settings;

    bool isFirstUse;
    bool isAlreadyLoaded;
    bool exitAfterStart;

    QString gamePath;

    QList<Realm> realms;
    int defaultRealmIndex;

    //clean everytime before start
    bool isCleanCache;
    bool isCleanWtf;

    int defaultOnlineShown; //default realm which show online
    int maxRealmsOnline[MAX_REALMS_ONLINE_COUNT];

    QString forumView1;
    QString forumView2;
    QString forumView3;

    int updateTime; //in milliseconds
};

#endif // SETTINGS_H
