#pragma once
#include <fstream>

struct SaveData
{
    int money;
    float maxHP;
    float playerSpeed;
    int barrlSpeed;
    int blastRadius;
};

class CSaveManager
{
public:
    static CSaveManager& Instance()
    {
        static CSaveManager instance;
        return instance;
    }

    // セーブとロード
    void Save();
    bool Load();

    // 初期値に戻す
    void Reset();

    SaveData data;
    SaveData& GetData() { return data; }

private:
    CSaveManager() {}             // private → シングルトン化
    CSaveManager(const CSaveManager&) = delete;
    void operator=(const CSaveManager&) = delete;
};