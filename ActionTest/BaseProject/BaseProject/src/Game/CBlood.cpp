#include "CBlood.h"


// アニメーションの1コマ表示時間
#define ANIM_TIME 0.0625f
// 炎のエフェクトのアニメーションデータ
TexAnimData CBlood::msAnimData = TexAnimData(4, 4, false, 16, ANIM_TIME);

CBlood::CBlood(ETag tag, CCharaBase* hitChara)
	: CBillboardImage("Effect/blood.png", tag, ETaskPauseType::eGame)
	, mpHitChara(hitChara)
	, mBloodScale(1.5f)
{
	SetAnimData(&msAnimData);
	Scale(CVector::one * mBloodScale);
}

CBlood::~CBlood()
{

}

void CBlood::Update()
{
	// 基底クラスの更新処理
	CBillboardImage::Update();

	Position(mpHitChara->Position() + CVector(0.0f,10.0f,0.0f));

	// アニメーションが終わったら、削除
	if (IsEndAnim())
	{
		Kill();
	}
}
