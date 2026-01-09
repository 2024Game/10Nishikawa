#pragma once
#include "CEnemy.h"

// エネミー管理クラス
class CEnemyManager
{
public:
	// インスタンスを取得
	static CEnemyManager* Instance();
	// インスタンスを破棄
	static void ClearInstance();

	// 指定したエネミーをリストに追加
	void Add(CEnemy* enemy);
	// 指定したエネミーをリストから除外
	void Remove(CEnemy* enemy);

	// 指定したエネミーが存在するかどうか(死亡していないか)
	bool Exist(CEnemy* enemy) const;

	// ロックオン対象
	CEnemy* GetLockOnTarget(float lockOnDist);

private:
	// コンストラクタ
	CEnemyManager();
	// デストラクタ
	~CEnemyManager();

	static CEnemyManager* mpInstance;	// インスタンス
	std::vector<CEnemy*> mEnemies;		// 生成済みのエネミーのリスト
};