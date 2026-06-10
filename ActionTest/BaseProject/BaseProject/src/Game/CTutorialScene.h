#pragma once
#include "CSceneBase.h"
#include <memory>
#include "CSaveManager.h"
#include "CTutoPlayer.h"
#include "CEnemy.h"
class CGameMenu;
class CSound;

//タイトルシーン
class CTutorialScene :public CSceneBase
{
public:
	//コンストラクタ
	CTutorialScene();
	//デストラクタ
	~CTutorialScene();
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
		ephase1,		// 戦闘準備時
		ephase2,		// 戦闘時
		ephase3,		// 戦闘結果時
	};
	// 状態を切り替え
	void ChangeState(EState state);

	EState mState;				// ゲームの状態
	int mStateStep;				// 状態内のステップ管理用
	float mElapsedTime;			// 経過時間計測用
	CSound* mpKanseiSE;
	CSaveManager* mpSaveManager;
	CGameMenu* mpGameMenu;
	CPlayer* mpPlayer;
	int mEnemyLv;
	bool mPlayerWin;
	int mTimeCount;
};