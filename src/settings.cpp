#include "settings.h"

Settings::Settings() : settings("walkline", "CircleLauncher")
{
    isAlreadyLoaded = true;
    exitAfterStart = false;
    gamePath = "";

    Realm logon1, logon2, logon3;

    logon1.realmName = "logon";
    logon2.realmName = "logon2";
    logon3.realmName = "logon3";

    logon1.realmUrl = "logon.wowcircle.com";
    logon2.realmUrl = "logon2.wowcircle.com";
    logon3.realmUrl = "logon3.wowcircle.com";

    realms.append(logon1);
    realms.append(logon2);
    realms.append(logon3);

    defaultRealmIndex = 0;

    isCleanCache = false;
    isCleanWtf   = false;

    defaultOnlineShown = 0;
    for(int i = 0; i < MAX_REALMS_ONLINE_COUNT; ++i)
        maxRealmsOnline[i] = 0;

    forumView1 = "http://forum.wowcircle.com/forumdisplay.php?f=2";
    forumView2 = "http://forum.wowcircle.com/forumdisplay.php?f=120";
    forumView3 = "http://forum.wowcircle.com/forumdisplay.php?f=3";

    updateTime = 300000; // 5 minutes
    isFirstUse = false;

    if(!settings.value("isAlreadyLoaded").toBool()) {
        isFirstUse = true;
        SaveSettings();
    } else
        LoadSettings();
}

void Settings::LoadSettings()
{
    isAlreadyLoaded = settings.value("isAlreadyLoaded").toBool();
    exitAfterStart = settings.value("exitAfterStart").toBool();

    gamePath = settings.value("gamePath").toString();

    realms.clear();
    int size  = settings.beginReadArray("Realms");
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        Realm realm;
        realm.realmName = settings.value("realmName").toString();
        realm.realmUrl = settings.value("realmUrl").toString();
        realms.append(realm);
    }
    settings.endArray();
    defaultRealmIndex = settings.value("defaultRealmIndex").toInt();

    isCleanCache = settings.value("isCleanCache").toBool();
    isCleanWtf = settings.value("isCleanWtf").toBool();

    defaultOnlineShown = settings.value("defaultOnlineShown").toInt();

    size = settings.beginReadArray("MaxRealmsOnline");
    for (int i = 0; i < MAX_REALMS_ONLINE_COUNT; ++i) {
        settings.setArrayIndex(i);
        maxRealmsOnline[i] = settings.value("maxOnline").toInt();
    }
    settings.endArray();

    forumView1 = settings.value("forumView1").toString();
    forumView2 = settings.value("forumView2").toString();
    forumView3 = settings.value("forumView3").toString();

    updateTime = settings.value("updateTime").toInt();
}

void Settings::SaveSettings()
{
    settings.clear();

    settings.setValue("isAlreadyLoaded", isAlreadyLoaded);
    settings.setValue("exitAfterStart", exitAfterStart);


    settings.setValue("gamePath", gamePath);

    settings.beginWriteArray("Realms");
    for (int i = 0; i < realms.size(); ++i) {
        settings.setArrayIndex(i);
        settings.setValue("realmName", realms.at(i).realmName);
        settings.setValue("realmUrl", realms.at(i).realmUrl);
    }
    settings.endArray();
    settings.setValue("defaultRealmIndex", defaultRealmIndex);

    settings.setValue("isCleanCache", isCleanCache);
    settings.setValue("isCleanWtf", isCleanWtf);

    settings.setValue("defaultOnlineShown", defaultOnlineShown);

    settings.beginWriteArray("MaxRealmsOnline");
    for (int i = 0; i < MAX_REALMS_ONLINE_COUNT; ++i) {
        settings.setArrayIndex(i);
        settings.setValue("maxOnline", maxRealmsOnline[i]);
    }
    settings.endArray();

    settings.setValue("forumView1", forumView1);
    settings.setValue("forumView2", forumView2);
    settings.setValue("forumView3", forumView3);

    settings.setValue("updateTime", updateTime);
}
