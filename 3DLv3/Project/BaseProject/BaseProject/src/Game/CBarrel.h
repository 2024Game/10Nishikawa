#pragma once
#include "CObjectBase.h"
#include "CModel.h"
#include "CPlayer.h"

// 樽爆弾のクラス
class CBarrel : public CObjectBase
{
public:
	// コンストラクタ
	CBarrel(float speed, float dist, CPlayer* player, CGameCamera2* camera);
	// デストラクタ
	~CBarrel();

	// 衝突処理
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// 更新
	void Update() override;
	// 描画
	void Render() override;

private:
	CModel* mpModel;		// 樽のモデルデータ
	CCollider* mpCollider;	// 樽のコライダー
	float mMoveSpeed;		// 移動速度
	float mMoveDist;		// 移動できる距離
	float mCurrDist;		// 現在の移動距離

	CPlayer* mpPlayer;
	CGameCamera2* mpCamera;
};