#ifndef CENEMY_H
#define CENEMY_H
#include "CXCharacter.h"
#include "CCollider.h"
#include "CModel.h"

class CGaugeUI3D;
class CEnemyBuffUI3D;

/*
エネミークラス
キャラクタクラスを継承
*/
class CEnemy : public CXCharacter
{
public:
	// アニメーションデータ
	struct AnimData
	{
		std::string path;	// アニメーションデータのパス
		bool loop;			// ループするかどうか
		float frameLength;	// アニメーションのフレーム数
		float speed;		// アニメーションの再生速度（1.0で等倍）
	};

	// コンストラクタ
	CEnemy();
	// デストラクタ
	virtual ~CEnemy();

	// オブジェクト削除を伝える関数
	void DeleteObject(CObjectBase* obj) override;

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自身のコライダー</param>
	/// <param name="other">衝突した相手のコライダー</param>
	/// <param name="hit">衝突した時の情報</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// CGameSceneから状態の移行指示を受け取る
	virtual void SetInBattle(int state);

	// 更新
	void Update() override;
	// 描画
	void Render() override;

	void SetGuardBreak(bool isbreak);

protected:
	// 敵の初期化
	void InitEnemy(std::string path, const std::vector<AnimData>* pAnimData);

	// 状態切り替え
	virtual void ChangeState(int state);

	// アニメーション切り替え
	virtual void ChangeAnimation(int type, bool restart = false);


	int mState;				// 状態
	int mAnimType;
	int mBattleTempo;		// Stateとは別の「戦術レイヤー」
	int mStateStep;			// 状態内のステップ管理用
	float mElapsedTime;		// 経過時間計測用
	bool mIsGravity;
	CVector mDeathVec;
	bool mToDeath = false;

	// アニメーションデータのテーブル
	const std::vector<AnimData>* mpAnimData;

	CVector mMoveSpeed;		// 前後左右の移動速度
	float mMoveSpeedY;		// 重力やジャンプによる上下の移動速度

	bool mIsGrounded;		// 接地しているかどうか
	CVector mGroundNormal;	// 接地している地面の法線

	CCollider* mpBodyCol;	// 本体のコライダー

	// ステータス関連
	int mLevel;				// 敵のレベル
	float mGainSt;			// スタミナ回復量
	float mAttackCost1;		// 攻撃のST消費量(少ない)
	float mAttackCost2;		// 攻撃のST消費量(普通)
	float mAttackCost3;		// 攻撃のST消費量(多い)
	float mAvoidCost;		// 回避の基本ST消費量
	float mStepMag;			// 踏み込みの移動距離倍率
	float mAttackMag;		// 攻撃力の倍率
	float mAtSpeedMag;		// 攻撃モーションの速度倍率
	float mNegTime;			// 隙ができる時間
	float mNegProb;			// 隙ができる確率

	CGaugeUI3D* mpHpGauge;		// HPゲージ
	CVector mHpGaugeOffsetPos;	// HPゲージのオフセット座標
	CGaugeUI3D* mpStGauge;		// スタミナゲージ
	CVector mStGaugeOffsetPos;	// スタミナゲージのオフセット座標

	// バフ・デバフ
	CEnemyBuffUI3D* mpGBBuffGauge;		// 防御ダウンデバフゲージ
	CVector mGBBuffOffsetPos;			// 防御ダウンデバフゲージのオフセット座標
	bool mInGuardBreak;					// 防御ダウンデバフ
	float mGuardBreakTime;				// 防御ダウンデバフ効果時間
	float mGBRemainTime;				// 防御ダウンデバフ効果残り時間
};

#endif
