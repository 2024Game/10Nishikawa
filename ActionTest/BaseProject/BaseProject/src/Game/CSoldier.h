#pragma once
#include "CEnemy.h"

//キャラクタクラスのインクルード
#include "CRideableObject.h"
#include "CSound.h"

#include "CPlayer.h"

class CCollider;
class CSword;

// 兵士の敵クラス
class CSoldier : public CEnemy
{
public:
	// コンストラクタ
	CSoldier(CPlayer* player, int level);
	// デストラクタ
	~CSoldier();

	void InitStatus();

	// 攻撃中か
	bool IsAttacking() const override;
	// 攻撃開始
	void AttackStart() override;
	// 攻撃終了
	void AttackEnd() override;

	// ダメージを受ける
	void TakeDamage(float damage, CObjectBase* causer) override;
	// 死亡
	void Death() override;
	// 衝突処理
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// 更新
	void Update() override;

	// CGameSceneから状態の移行指示を受け取る
	void SetInBattle(int state);

private:
	// アニメーションの種類
	enum class EAnimType
	{
		None = -1,

		eTPose,		// Tポーズ			0
		eIdle,		// 待機				1
		eIdleBattle,// 戦闘中の待機		2
		eWalk,		// 歩行				3
		eRun,		// ダッシュ			4
		eAttack1,	// 斬り攻撃1		5
		eAttack2,	// 斬り攻撃2		6
		eAttackX,	// 斬りかかり攻撃	7
		eKick,		// 蹴り攻撃			8
		eJumpStart,	// ジャンプ開始		9
		eJump,		// ジャンプ中		10
		eJumpEnd,	// ジャンプ終了		11
		eAvoidR,	// 回避:右			12
		eAvoidL,	// 回避:左			13
		eHit,		// 仰け反り			14
		eDeath,		// 死亡				15
		eVictory,	// 勝利				16
		
		Num
	};

	// 敵の状態
	enum class EState
	{
		eReserve,	// 戦闘準備
		eIdle,		// 待機
		eChase,		// 追いかける
		eAttack1,	// 斬り攻撃1
		eAttack2,	// 斬り攻撃2
		eAttackX,	// 斬りかかり攻撃
		eKick,		// 蹴り攻撃
		eJumpStart,	// ジャンプ開始
		eJump,		// ジャンプ中
		eJumpEnd,	// ジャンプ終了
		eAvoidR,	// 回避:右
		eAvoidL,	// 回避:左
		eHit,		// 仰け反り
		eDeath,		// 死亡
		eVictory,	// 勝利
	};

	// 戦闘相手の方へ向く
	void LookAtBattleTarget(bool immediate = false);

	// 針を発射
	void ShotNeedle();

	// 状態切り替え
	void ChangeState(int state) override;

	// 戦闘準備状態
	void UpdateReserve();
	// 待機状態の更新処理
	void UpdateIdle();
	// 追いかける時の更新処理
	void UpdateChase();
	// 斬り攻撃1の更新処理
	void UpdateAttack1();
	// 斬り攻撃2の更新処理
	void UpdateAttack2();
	// 斬り攻撃Xの更新処理
	void UpdateAttackX();
	// どの方向に回避するべきかを決定する
	void SelectAvoid();
	// 回避:右
	void UpdateAvoidR();
	// 回避:左
	void UpdateAvoidL();
	// 仰け反り状態の更新処理
	void UpdateHit();
	// 死亡状態の更新処理
	void UpdateDeath();
	// 勝利
	void UpdateVictory();


	// ステータス関連
	float mA1StCost;
	float mAvoidStCost;

	CCollider* mpKickCol;			// 蹴り攻撃用コライダー
	CCollider* mpTACol;				// 先行入力(Type Ahead)用コライダー
	CTransform* mpRideObject;

	CSound* mpSlashSE;
	bool mIsPlayedSlashSE;
	bool mIsSpawnedSlashEffect;

	bool mIsBattle;					// 戦闘状態か
	float mBattleIdletime;			// 戦闘時の待機時間
	CObjectBase* mpBattleTarget;	// 戦闘相手

	// 剣のクラス
	CSword* mpSword;

	bool mNextAttack; //連続攻撃が予約されているかどうか
	CVector mAttackVec;
	float mAttackTimer = 0.0f;
	bool mInAttack = false;
	float mStepMag;

	CVector mAvoidVec;
	float mAvoidTimer = 0.0f;
	float mAvoidDuration = 0.25f;   // 0.5秒
	bool  mAvoidMoving = false;
};
