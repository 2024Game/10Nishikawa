#pragma once
#include "CObjectBase.h"
#include "CModel.h"
#include "CPlayer.h"
#include "CSound.h"

// 樽爆弾のクラス
class CBarrel : public CObjectBase
{
public:
	// コンストラクタ
	/// <param name="speed">樽の沈下速度(m/秒)</param>
	/// <param name="dist">樽の発火深度</param>
	/// <param name="damage">樽のダメージ</param>
	CBarrel(float speed, float dist, float damage, CPlayer* player, CGameCamera2* camera);
	// デストラクタ
	~CBarrel();

	// 衝突処理
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// 更新
	void Update() override;
	// 描画
	void Render() override;

private:
	CModel* mpModel;			// 樽のモデルデータ
	CCollider* mpCollider;		// 樽のコライダー
	CCollider* mpAttackCol;		// 攻撃用のコライダー
	CSound* mpExplosionSound;	// 爆発音データ
	float mMoveSpeed;			// 移動速度
	float mTrackSpeed;			// 追尾速度
	float mFireDepth;			// 発火深度

	bool mAttackTriggered = false;   // 攻撃コライダーを有効にしたか
	float mKillTimer = 0.0f;         // Killまでの経過時間
	float mFireDamage;
	CVector mTargetPos;

	CPlayer* mpPlayer;
	CGameCamera2* mpCamera;
};