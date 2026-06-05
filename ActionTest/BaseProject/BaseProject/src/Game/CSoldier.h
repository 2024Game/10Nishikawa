#pragma once
#include "CEnemy.h"

//キャラクタクラスのインクルード
#include "CRideableObject.h"
#include "CSound.h"
#include "CPlayer.h"

class CCollider;
class CGreatSword;
class CSlash;

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
		eAttack1B,	// 斬り攻撃1B		6
		eAttack2,	// 斬り攻撃2		7
		eAttackX,	// 斬りかかり攻撃	8
		eKick,		// 蹴り攻撃			9
		eJumpStart,	// ジャンプ開始		10
		eJump,		// ジャンプ中		11
		eJumpEnd,	// ジャンプ終了		12
		eAvoidR,	// 回避:右			13
		eAvoidL,	// 回避:左			14
		eAvoidB,	// 回避:後ろ		15
		eHit,		// 仰け反り			16
		eDeath,		// 死亡				17
		eVictory,	// 勝利				18
		
		Num
	};

	// 敵の状態
	enum class EState
	{
		eReserve,	// 戦闘準備
		eIdle,		// 待機
		eChase,		// 追いかける
		eAttack1,	// 斬り攻撃1
		eAttack1B,	// 斬り攻撃1B(右薙ぎ払い)
		eAttack2,	// 斬り攻撃2
		eAttackX,	// 斬りかかり攻撃
		eSlash,		// 斬撃(遠距離)攻撃
		eKick,		// 蹴り攻撃
		eJumpStart,	// ジャンプ開始
		eJump,		// ジャンプ中
		eJumpEnd,	// ジャンプ終了
		eAvoidR,	// 回避:右
		eAvoidL,	// 回避:左
		eAvoidB,	// 回避:後ろ
		eHit,		// 仰け反り
		eNeglect,	// 攻撃後の隙
		eDeath,		// 死亡
		eVictory,	// 勝利
	};

	// 1番目「戦術レイヤー」(HPベース)
	enum class ETactics
	{
		Aggressive,		// 攻撃的 (HP 70%以上)
		Balanced,		// バランス (HP 30%〜70%)
		Cautious		// 慎重     (HP 30%未満)
	};

	// 2番目「行動テンポレイヤー」(スタミナベース)
	enum class EBattleTempo
	{
		HighSt,	// 高スタミナ：積極的に攻撃
		MidSt,	// 中スタミナ：通常の立ち回り
		LowSt	// 低スタミナ：守りに入る
	};

	// 攻撃パーターン用の列挙型
	enum class EAttPattern
	{
		None,		// 攻撃無し
		PatternA,	// 1段目のみ
		PatternB,	// 1段目→2段目
		PatternC,	// 1段目→2段目→3段目
		PatternD	// 考え中
	};

	// アニメーション切り替え
	void ChangeAnimation(int type, bool restart = false) override;

	// 1番目 現在のHPを参照して「戦術レイヤー」切り替え
	void UpdateTactics();

	// 2番目 現在のスタミナを参照して「行動テンポレイヤー」切り替え
	void UpdateBattleTempo();

	// 戦術×テンポのマトリクスから次の行動ステートを決定
	void DecideNextAction();

	// 戦闘相手の方へ向く
	void LookAtBattleTarget(bool immediate = false);

	// 現在地から目的地までの距離を求める
	float GetDistToTarget();

	// スタミナ自動回復
	void STRegene();

	// 行動状態切り替え
	void ChangeState(int state) override;

	// 戦闘準備状態
	void UpdateReserve();
	// 待機状態の更新処理
	void UpdateIdle();
	// 追いかける時の更新処理
	void UpdateChase();
	// 斬り攻撃1の更新処理
	void UpdateAttack1();
	// 斬り攻撃1Bの更新処理
	void UpdateAttack1B();
	// 斬り攻撃2の更新処理
	void UpdateAttack2();
	// 斬り攻撃Xの更新処理
	void UpdateAttackX();
	// 斬り攻撃1の更新処理
	void UpdateSlash();
	// どの方向に回避するべきかを決定する
	void SelectAvoid();
	// 回避:右
	void UpdateAvoidR();
	// 回避:左
	void UpdateAvoidL();
	// 回避:後ろ
	void UpdateAvoidB();
	// 攻撃後の隙あり状態の更新処理
	void UpdateNeg();
	// 仰け反り状態の更新処理
	void UpdateHit();
	// 死亡状態の更新処理
	void UpdateDeath();
	// 勝利
	void UpdateVictory();

	// ----- 連続攻撃などの攻撃パターン -----
	// 行動連結用ギアボックス
	// 行動ごとにここに戻ってきて次の行動に連結する
	void AttPattGearBox();
	// 1段目のみ
	void AttPatternA();
	// 1段目→2段目
	void AttPatternB();
	// 1段目→2段目→3段目
	void AttPatternC();
	// 
	void AttPatternD();


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
	CGreatSword* mpSword;
	// 斬撃オブジェクト
	CSlash* mpSlash;

	bool mCan1B;			// 1段目B攻撃が可能かどうか？
	float m1BProb;			// 1段目B攻撃を行う確率
	CVector mAttackVec;
	bool mInAttack = false;
	int mAttPattern;

	CVector mAvoidVec;
	bool  mAvoidMoving = false;

	int mTactics;			// 現在の戦術レイヤー (ETactics)
};
