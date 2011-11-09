#include "characters.h"
#include <QTextCodec>

Characters::Characters(QString _gamePath)
{
    gamePath = _gamePath;
}

void Characters::setGamePath(QString _gamePath)
{
    gamePath = _gamePath;
}

void Characters::SearchByRealm(QString _realm)
{
    realm = _realm;
    characters.clear();
    QStringList dataFiles = getCharDataFiles();
    foreach (QString fileName, dataFiles)
    {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            continue;

        processFile(&file);
    }
}

QStringList Characters::getCharDataFiles()
{
    QDir dir(gamePath + "/WTF/Account/");
    QFileInfoList list = dir.entryInfoList();
    QFileInfo fileInfo;
    QStringList files;
    foreach (fileInfo, list)
        if (fileInfo.isDir() && QFile::exists(fileInfo.filePath()+"/SavedVariables/CharData.lua"))
            files << fileInfo.filePath() + "/SavedVariables/CharData.lua";
    return files;
}

void Characters::processFile(QFile *file)
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    QStringList names;
    QList< QPair<QString, QString> > charClassStr;
    QList< QPair<QString, QString> > race;
    QList< QPair<QString, QString> > isMale;
    QList< QPair<QString, QString> > level;
    QList< QPair<QString, QString> > lastIn;

    while (!file->atEnd())
    {
        QString line = QObject::tr(file->readLine());
        if (!line.contains(realm))
            continue;

        QPair<QString, QString> tmp = qMakePair(getCharacterName(line), getAssignment(line));

        if (line.contains("-name"))
            names << tmp.first;
        else if (line.contains("-class"))
            charClassStr.append(tmp);
        else if (line.contains("-race"))
            race.append(tmp);
        else if (line.contains("-sex"))
            isMale.append(tmp);
        else if (line.contains("-level"))
            level.append(tmp);
        else if (line.contains("-lastIn"))
            lastIn.append(tmp);
    }

    for (int i = 0; i < names.size(); ++i)
    {
        Character _char;
        _char.name = names.at(i);
        for (int j = 0; j < names.size(); ++j)
        {
            if (names.at(i) == charClassStr.at(j).first)
            {
                _char.charClassStr = getRealClassName(charClassStr.at(j).second);
                _char.charClass = getClassFromStr(_char.charClassStr);
            }
            if (names.at(i) == race.at(j).first)
                _char.race = getRealRace(race.at(j).second);
            if (names.at(i) == isMale.at(j).first)
                _char.isMale = isMaleStrToBool(isMale.at(j).second);
            if (names.at(i) == level.at(j).first)
                _char.level = level.at(j).second.toInt();
            if (names.at(i) == lastIn.at(j).first)
                _char.lastIn = QDateTime::fromString(lastIn.at(j).second, "MM/dd/yy hh:mm:ss");
        }
        characters.append(_char);
    }
}
QString Characters::getAssignment(QString str)
{
    if (!str.contains("="))
        return "";

    str = str.simplified();
    if (str.count("\"") == 4)
    {
        int endIndex = str.lastIndexOf("\"");
        int begIndex = str.lastIndexOf("\"", endIndex - 1) + 1;
        return str.mid(begIndex, endIndex - begIndex);
    }

    int begIndex = str.indexOf("=") + 2;
    int endIndex = str.indexOf(",");
    return str.mid(begIndex, endIndex - begIndex);
}

QString Characters::getCharacterName(QString str)
{
    int begIndex = str.indexOf(realm + "@") + QString(realm + "@").count();
    int endIndex = str.lastIndexOf("-");
    return str.mid(begIndex, endIndex - begIndex);
}

QString Characters::getRealClassName(QString str)
{
    if (str == QObject::tr("� ыцарь смерти") || str.contains("Death"))
        return "DeathKnight";
    if (str == QObject::tr("Друид"))
        return "Druid";
    if (str == QObject::tr("Воин"))
        return "Warrior";
    if (str == QObject::tr("Жрец") || str == QObject::tr("Жрица"))
        return "Priest";
    if (str == QObject::tr("Маг"))
        return "Mage";
    if (str == QObject::tr("Охотник")|| str == QObject::tr("Охотница"))
        return "Hunter";
    if (str == QObject::tr("Паладин"))
        return "Paladin";
    if (str == QObject::tr("� азбойник") || str == QObject::tr("� азбойница"))
        return "Rogue";
    if (str == QObject::tr("Чернокнижник") || str == QObject::tr("Чернокнижница"))
        return "Warlock";
    if (str == QObject::tr("Шаман") || str == QObject::tr("Шаманка"))
        return "Shaman";
    else
        return str;
}

CharClassEnum Characters::getClassFromStr(QString str)
{
    if (str == "DeathKnight")
        return CLASS_DEATH_KNIGHT;
    if (str == "Druid")
        return CLASS_DRUID;
    if (str == "Warrior")
        return CLASS_WARRIOR;
    if (str == "Priest")
        return CLASS_PRIEST;
    if (str == "Mage")
        return CLASS_MAGE;
    if (str == "Hunter")
        return CLASS_HUNTER;
    if (str == "Paladin")
        return CLASS_PALADIN;
    if (str == "Rogue")
        return CLASS_ROGUE;
    if (str == "Warlock")
        return CLASS_WARLOCK;
    if (str == "Shaman")
        return CLASS_SHAMAN;
    return CLASS_UNK_0;
}

CharRaceEnum Characters::getRealRace(QString str)
{
    if (str == QObject::tr("Эльф крови") || str == QObject::tr("Эльфийка крови") || str == "Blood Elf")
        return RACE_BLOODELF;
    if (str == QObject::tr("Человек") || str == "Human")
        return RACE_HUMAN;
    if (str == QObject::tr("Нежить") || str == "Undead" || str == "Forsaken")
        return RACE_UNDEAD;
    if (str == QObject::tr("Ночная эльфийка") || str == QObject::tr("Ночной эльф") || str == "Night Elf")
        return RACE_NIGHTELF;
    if (str == QObject::tr("Орк") || str == "Ork")
        return RACE_ORK;
    if (str == QObject::tr("Тролль") || str == "Troll")
        return RACE_TROLL;
    if (str == QObject::tr("Таурен") || str == "Tauren")
        return RACE_TAUREN;
    if (str == QObject::tr("Гном") || str == "Gnome")
        return RACE_GNOME;
    if (str == QObject::tr("Дворф") || str == "Dwarf")
        return RACE_DWARF;
    if (str == QObject::tr("Дреней") || str == "Draenei")
        return RACE_DRAENEI;
    if (str == QObject::tr("Гоблин") || str == "Goblin")
        return RACE_GOBLIN;

    return RACE_UNKNOWN;
}

bool Characters::isMaleStrToBool(QString str)
{
    if (str.toInt() == 2)
        return true;
    return false;
}

Character Characters::createUnknownChar()
{
    Character tmp;
    tmp.name = "unknown";
    tmp.charClassStr = "Warrior";
    tmp.charClass = CLASS_UNK_0;
    tmp.race = RACE_UNKNOWN;
    tmp.isMale = true;
    tmp.level = 42;
    tmp.lastIn = QDateTime();
    return tmp;
}

Character Characters::getCharacterByIndex(int index)
{
    if (index > characters.size() || !characters.size())
        return createUnknownChar();

    return characters.at(index);
}

QList<Character> Characters::getCharacters()
{
    if (!characters.size())
        characters.append(createUnknownChar());

    return characters;
}

int Characters::getCharactersCount()
{
    return characters.size();
}

int Characters::getIndexByName(QString name)
{
    for (int i = 0; i < characters.size(); ++i)
        if (characters.at(i).name == name)
            return i;
    return 0;
}
