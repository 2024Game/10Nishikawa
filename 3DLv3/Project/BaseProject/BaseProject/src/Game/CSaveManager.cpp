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
    // SaveData ‚ÌŠe’l‚ğ‰Šú’l‚Éİ’è
    data.money = 0;
    data.fuelTankLv = 0;
    data.playerSpeedLv = 0;
    data.barrlSpeedLv = 0;
    data.blastPowerLv = 0;
    data.blastRadiusLv = 0;
    data.barrlTrackingLv = 0;
    data.radarRecastTimeLv = 0;
}
