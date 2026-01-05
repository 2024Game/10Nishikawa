#include "CSaveManager.h"

void CSaveManager::Save()
{
    std::ofstream file("save.bin", std::ios::binary);
    if (!file) return;

    file.write(reinterpret_cast<const char*>(&data), sizeof(SaveData));
}

bool CSaveManager::Load()
{
    std::ifstream file("save.bin", std::ios::binary);
    if (!file) return false;

    file.read(reinterpret_cast<char*>(&data), sizeof(SaveData));
    return true;
}

void CSaveManager::Reset()
{
    // SaveData の各値を初期値に設定
    data.day = 1;
    data.selectDiff = 0;
    data.money = 0;
    data.maxHp = 100;
    data.hp = 100;
    data.hpLv = 0;
    data.hpRegeneLv = 0;
    data.stLv = 0;
    data.stRegeneLv = 0;
    data.attackLv = 0;
    data.healerLv = 0;
    data.reincarnationLv = 0;
#ifdef _DEBUG
    // SaveData の各値を初期値に設定
    data.day = 1;
    data.selectDiff = 0;
    data.money = 5000;
    data.maxHp = 100;
    data.hp = 100;
    data.hpLv = 0;
    data.hpRegeneLv = 0;
    data.stLv = 0;
    data.stRegeneLv = 0;
    data.attackLv = 0;
    data.healerLv = 0;
    data.reincarnationLv = 0;
#endif // _DEBUG

    Save();
}