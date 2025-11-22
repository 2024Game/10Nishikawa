#pragma once
//キャラクタクラスのインクルード
#include "CCharaBase.h"
#include "CModel.h"
#include "CGameCamera2.h"

class CCollider;

/*
プレイヤークラス
キャラクタクラスを継承
*/
class CPlayer : public CCharaBase
{
public:
	//インスタンスのポインタの取得
	static CPlayer* Instance();

	// コンストラクタ
	CPlayer();
	// デストラクタ
	~CPlayer();

	// 更新
	void Update();

	void DropBarrel();

	// ダメージを受ける
	void TakeDamage(int damage, CObjectBase* causer) override;

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="self">衝突した自身のコライダー</param>
	/// <param name="other">衝突した相手のコライダー</param>
	/// <param name="hit">衝突した時の情報</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	float GetDepth();

	// 描画
	void Render();

	void SetCamera(CGameCamera2* camera);

	void SetState(int stateNum);

private:
	// オブジェクト削除を伝える
	void DeleteObject(CObjectBase* obj) override;

	// キーの入力情報から移動ベクトルを求める
	CVector CalcMoveVec() const;

	// 待機状態
	void UpdateIdle();
	// 仰け反り
	void UpdateHit();

	// 移動の更新処理
	void UpdateMove();

	// プレイヤーのインスタンス
	static CPlayer* spInstance;

	// プレイヤーの状態
	enum class EState
	{
		eIdle,		// 待機
		eMovable,	// 移動可能
		eHit,		// 仰け反り
	};
	// 状態を切り替え
	void ChangeState(EState state);

	EState mState;				// プレイヤーの状態
	int mStateStep;				// 状態内のステップ管理用
	float mElapsedTime;			// 経過時間計測用

	CVector mMoveSpeed;			// 前後左右の移動速度
	float mMoveSpeedY;			// 重力やジャンプによる上下の移動速度

	CModel* mpModel;			// 本体のモデルデータ
	CCollider* mpBodyCol;		// 本体のコライダー

	CGameCamera2* mpCamera;		// カメラのポインター

	float mFireDepth;			// 樽爆弾の発火深度
};
