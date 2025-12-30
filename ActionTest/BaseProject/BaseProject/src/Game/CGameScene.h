#pragma once
#include "CSceneBase.h"
#include <memory>
#include "CSaveManager.h"
#include "CPlayer.h"
#include "CEnemy.h"
class CGameMenu;
class CSound;

//タイトルシーン
class CGameScene :public CSceneBase
{
public:
	//コンストラクタ
	CGameScene();
	//デストラクタ
	~CGameScene();
	//シーン読み込み
	void Load();
	//シーンの更新処理
	void Update();

private:
	//戦闘準備時の更新処理
	void UpdateBattleReserve();
	//戦闘時の更新処理
	void UpdateBattle();
	//戦闘結果時の更新処理
	void UpdateBattleResult();
	// ゲームの状態
	enum class EState
	{
		ebattlereserve,		// 戦闘準備時
		ebattle,			// 戦闘時
		ebattleresult,		// 戦闘結果時
	};
	// 状態を切り替え
	void ChangeState(EState state);

	EState mState;				// ゲームの状態
	int mStateStep;				// 状態内のステップ管理用
	float mElapsedTime;			// 経過時間計測用
	CSaveManager* mpSaveManager;
	CGameMenu* mpGameMenu;
	CPlayer* mpPlayer;
	CEnemy* mpEnemy;
	int mEnemyLv;
	bool mPlayerWin;
	int mTimeCount;
};
