#pragma once
#include "CWeapon.h"
#include "CModel.h"

// 剣のクラス
class CGreatSword : public CWeapon
{
public:
	// コンストラクタ
	CGreatSword(CObjectBase* owner, ETag tag,
		Tags hitTags, Layers hitLayers);
	// デストラクタ
	~CGreatSword();

	// 描画
	void Render() override;

private:
	CModel* mpModel;	// 剣のモデル
};
