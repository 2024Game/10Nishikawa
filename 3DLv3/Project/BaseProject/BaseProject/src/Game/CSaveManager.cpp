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
    data.maxHP = 60;  // ‰ŠúHP
    data.playerSpeed = 0.5f;
    data.barrlSpeed = 0;
    data.blastRadius = 0;
}
