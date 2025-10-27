#pragma once
#include "CObjectBase.h"
#include "CModel.h"
class CTrailEffect;

// 樽爆弾クラス
class CBarrel : public CObjectBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">弾丸の位置</param>
	/// <param name="dir">弾丸の向き</param>
	/// <param name="speed">移動速度</param>
	/// <param name="distance">飛距離</param>
	CBarrel(const CVector& pos, const CVector& dir,
		float speed, float distance);

	// デストラクタ
	~CBarrel();

	// 更新
	void Update() override;
	// 描画
	void Render() override;

private:
	float mMoveSpeed;				// 移動速度
	float mFlyingDistance;			// 飛距離
	float mCurrentFlyingDistance;	// 現在の飛距離
};