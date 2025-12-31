#pragma once
//キャラクタクラスのインクルード
#include "CXCharacter.h"
#include "CRideableObject.h"
#include "CSound.h"
#include "CSaveManager.h"

class CCollider;
class CFlamethrower;
class CSword;

/*
プレイヤークラス
キャラクタクラスを継承
*/
class CPlayer : public CXCharacter
{
public:
	//インスタンスのポインタの取得
	static CPlayer* Instance();

	// コンストラクタ
	CPlayer(CSaveManager* SaveManager);
	// デストラクタ
	~CPlayer();

	// 更新
	void Update();

	// CGameSceneから状態の移行指示を受け取る
	void SetInBattle(int state);

	// 攻撃中か
	bool IsAttacking() const override;
	// 攻撃開始
	void AttackStart() override;
	// 攻撃終了
	void AttackEnd() override;

	// ダメージを受ける
	void TakeDamage(float damage, CObjectBase* causer) override;

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自身のコライダー</param>
	/// <param name="other">衝突した相手のコライダー</param>
	/// <param name="hit">衝突した時の情報</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// 描画
	void Render();

private:
	// オブジェクト削除を伝える
	void DeleteObject(CObjectBase* obj) override;

	// キーの入力情報から移動ベクトルを求める
	CVector CalcMoveVec() const;

	// 戦闘準備状態
	void UpdateReserve();
	// 待機状態
	void UpdateIdle();
	// 斬り攻撃1
	void UpdateAttack1();
	// 斬り攻撃2
	void UpdateAttack2();
	// 斬り攻撃X
	void UpdateAttackX();
	// 蹴り攻撃
	void UpdateKick();
	// ジャンプ開始
	void UpdateJumpStart();
	// ジャンプ中
	void UpdateJump();
	// ジャンプ終了
	void UpdateJumpEnd();
	// 回避:右
	void UpdateAvoidR();
	// 回避:左
	void UpdateAvoidL();
	// 仰け反り
	void UpdateHit();
	// 死亡
	void UpdateDeath();
	// 勝利
	void UpdateVictory();

	// 移動の更新処理
	void UpdateMove();

	// モーションブラーの更新処理
	void UpdateMotionBlur();

	// アニメーションの種類
	enum class EAnimType
	{
		None = -1,

		eTPose,		// Tポーズ
		eIdle,		// 待機
		eWalk,		// 歩行
		eRun,		// ダッシュ
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

		Num
	};
	// アニメーション切り替え
	void ChangeAnimation(EAnimType type, bool restart = false);

	// プレイヤーのインスタンス
	static CPlayer* spInstance;

	// アニメーションデータ
	struct AnimData
	{
		std::string path;	// アニメーションデータのパス
		bool loop;			// ループするかどうか
		float frameLength;	// アニメーションのフレーム数
		float speed;		// アニメーション速度（1.0で等倍）
	};
	// アニメーションデータのテーブル
	static const AnimData ANIM_DATA[];

	// プレイヤーの状態
	enum class EState
	{
		eReserve,	// 戦闘準備
		eIdle,		// 待機
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
	// 状態を切り替え
	void ChangeState(EState state);

	EState mState;				// プレイヤーの状態
	int mStateStep;				// 状態内のステップ管理用
	float mElapsedTime;			// 経過時間計測用

	CVector mMoveSpeed;			// 前後左右の移動速度
	float mMoveSpeedY;			// 重力やジャンプによる上下の移動速度
	bool mIsGravity;

	bool mIsGrounded;			// 接地しているかどうか
	CVector mGroundNormal;		// 接地している地面の法線

	CColliderCapsule* mpBodyCol;	// 本体のコライダー
	CCollider* mpKickCol;			// 蹴り攻撃用コライダー
	CCollider* mpTACol;				// 先行入力(Type Ahead)用コライダー
	CTransform* mpRideObject;

	CSaveManager* mpSaveManager;
	CSound* mpSlashSE;
	bool mIsPlayedSlashSE;
	bool mIsSpawnedSlashEffect;

	// 火炎放射エフェクト
	CFlamethrower* mpFlamethrower;

	// モーションブラーを掛ける残り時間
	float mMotionBlurRemainTime;

	// 剣のクラス
	CSword* mpSword;

	float mA1StCost;
	float mAvoidStCost;

	float mStRegeneMag; // スタミナ回復倍率
	float mAttackMag;	// 攻撃の倍率
	bool mNextAttack;	// 連続攻撃が予約されているかどうか
	CVector mAttackVec;
	bool mInAttack = false;

	CVector mAvoidVec;
	float mAvoidTimer = 0.0f;
	float mAvoidDuration = 0.25f;   // 0.5秒
	bool  mAvoidMoving = false;

	CVector mDeathVec;
	float mDeathTimer = 0.0f;
	bool mToDeath = false;
};
