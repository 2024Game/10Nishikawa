#pragma once
#include "CWeapon.h"
#include "CModel.h"

// 剣のクラス
class CBusterSword : public CWeapon
{
public:
	// コンストラクタ
	CBusterSword(CObjectBase* owner, ETag tag,
		Tags hitTags, Layers hitLayers);
	// デストラクタ
	~CBusterSword();

	// 描画
	void Render() override;

private:
	CModel* mpModel;	// 剣のモデル
};