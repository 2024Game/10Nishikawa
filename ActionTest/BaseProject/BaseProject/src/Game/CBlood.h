#pragma once
#include "CBillboardImage.h"
#include "CCharaBase.h"

class CBlood : public CBillboardImage
{
public:
	// コンストラクタ
	CBlood(ETag tag, CCharaBase* hitChara);
	// デストラクタ
	~CBlood();

	// 更新
	void Update() override;

private:
	// アニメーションデータ
	static TexAnimData msAnimData;
	CCharaBase* mpHitChara; //このエフェクトが出ているキャラクター
	// スケールの値
	float mBloodScale;
};