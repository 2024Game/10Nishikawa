#pragma once
//キャラクタクラスのインクルード
#include "CXCharacter.h"
#include "CRideableObject.h"
#include "CSound.h"
#include "CSaveManager.h"
#include "GameInput.h"

using namespace GameInput::v3;

class CCollider;
class CFlamethrower;
class CGreatSword;
class CTargetUI;
class CPlayerUI3D;
class CEnemy;

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

	// チュートリアル時にチュートリアル用のステータスに
	void TutorialInit();

	// 更新
	void Update();
	//最後更新
	void LastUpdate() override;

	// CGameSceneから状態の移行指示を受け取る
	/*
	0 = 戦闘前待機
	2 = 勝利状態
	*/
	void SetInBattle(int state);

	// 攻撃中か
	bool IsAttacking() const override;
	// 攻撃開始
	void AttackStart() override;
	// 攻撃終了
	void AttackEnd() override;

	// ダメージを受ける
	void TakeDamage(float damage, CObjectBase* causer) override;

	// プレイヤーの当たり判定を変更して無敵にする
	void SetInvincible(bool invincible);

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自身のコライダー</param>
	/// <param name="other">衝突した相手のコライダー</param>
	/// <param name="hit">衝突した時の情報</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// 描画
	void Render();

	float GetMaxS1();
	float GetS1();

	// ----- チュートリアル用 -----
	int mAttHitCount;		// 攻撃が何回当たったか
	int mJustAvoidCount;	// ジャスト回避成功数
	int mRushHitCount;		// カウンター攻撃成功数
	int mKickHitCount;		// キックをあてた回数
	int mJustKickHitCount;	// ジャストでキックをあてた回数

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
	// スライド斬り攻撃
	void UpdateSlideAttack();
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
	// 回避:後ろ
	void UpdateAvoidB();
	// 仰け反り
	void UpdateHit();
	// 死亡
	void UpdateDeath();
	// 勝利
	void UpdateVictory();

	// ボタンが押されていたら回避へ移行する
	void AvoidJudge();


	// 移動の更新処理
	void UpdateMove();

	// モーションブラーの更新処理
	void UpdateMotionBlur();

	// カメラの向いている向きにプレイヤーを向かせる
	void LookCameraForward();

	void LockOnTarget();
	void UnLockTarget();
	void UpdateLockOn();

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
		eSlideAtt,	// スライド斬り攻撃
		eJumpStart,	// ジャンプ開始
		eJump,		// ジャンプ中
		eJumpEnd,	// ジャンプ終了
		eAvoidR,	// 回避:右
		eAvoidL,	// 回避:左
		eAvoidB,	// 回避:後ろ
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
		eSlideAtt,	// スライド斬り攻撃
		eJumpStart,	// ジャンプ開始
		eJump,		// ジャンプ中
		eJumpEnd,	// ジャンプ終了
		eAvoidR,	// 回避:右
		eAvoidL,	// 回避:左
		eAvoidB,	// 回避:後ろ
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

	CSaveManager* mpSaveManager;	// セーブデータ管理クラス
	CSound* mpSlashSE;				// 攻撃時のサウンド
	bool mIsPlayedSlashSE;
	bool mIsSpawnedSlashEffect;

	// スキル関連
	float mS1RecastTime;		// スキル１のクールタイム
	float mS1CastTime;			// スキル１の経過クールタイム

	bool mIsSA;					// スーパーアーマーかどうか

	// 火炎放射エフェクト
	// CFlamethrower* mpFlamethrower;

	// モーションブラーを掛ける残り時間
	float mMotionBlurRemainTime;

	// 剣のクラス
	CGreatSword* mpGreatSword;

	// GameInputの本体オブジェクト
	IGameInput* mpGameInput;

	// ----- ステータス関連 -----
	float mAttackCost1;
	float mAvoidCost;
	float mStRegeneMag; // スタミナ回復倍率
	float mAttackMag;	// 攻撃の倍率
	bool mNextAttack;	// 連続攻撃が予約されているかどうか
	CVector mAttackVec;
	bool mInAttack = false;

	CVector mAvoidVec;
	bool  mAvoidMoving = false;

	CVector mDeathVec;
	float mDeathTimer = 0.0f;
	bool mToDeath = false;

	bool mIsLockOn;
	CEnemy* mpLockOnTarget;
	CTargetUI* mpTargetUI;
	CModelXFrame* mpHeadFrame;
	CPlayerUI3D* mpIndicator;		// インジケーター
	CVector mIndicatorOffsetPos;	// インジケーターのオフセット座標
	bool mInTypeAhead;				// 先行入力コライダー内に入っているか
	bool mInJustAction;
};
