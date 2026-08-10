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

	// エネミーの状態を管理
	/*
	0 = 戦闘前待機
	2 = エネミー勝利状態
	4 = チュートリアル1(棒立ち)
	5 = チュートリアル2(近寄ると0ダメ攻撃)
	*/
	void SetInBattle(int num);

	// 指定したエネミーが存在するかどうか(死亡していないか)
	bool Exist(CEnemy* enemy) const;

	// 生成済みのエネミーのリスト内の全エネミーのHPが0より多いか
	// つまり、生存しているエネミーがいるかどうか
	bool Surviv() const;

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