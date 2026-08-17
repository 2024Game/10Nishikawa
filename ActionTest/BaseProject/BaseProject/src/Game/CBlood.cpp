#include "CBlood.h"


// アニメーションの1コマ表示時間
#define ANIM_TIME 0.05f
// 血のエフェクトのアニメーションデータ
TexAnimData CBlood::msAnimData = TexAnimData(4, 4, false, 16, ANIM_TIME);

CBlood::CBlood(ETag tag)
	: CBillboardImage("Effect/blood.png", tag, ETaskPauseType::eGame)
	, mpHitChara(nullptr)
	, mBloodScale(1.0f)
	, mIsPlay(false)
{
	SetAnimData(&msAnimData);

	// 最初は動かさない・描画しない
	SetEnable(false);
	SetShow(false);
}

CBlood::~CBlood()
{

}

void CBlood::Update()
{
	if (mIsPlay)
	{
		// 基底クラスの更新処理
		CBillboardImage::Update();

		Position(mpHitChara->Position() + CVector(0.0f, 10.0f, 0.0f));

		// アニメーションが終わったら、削除
		if (IsEndAnim())
		{
			// Kill();
			mIsPlay = false;
			CTask::SetEnable(false);
			SetShow(false); // 描画フラグをOFFにする
		}
	}
}

void CBlood::SetEffect(CCharaBase* hitChara, float size)
{
	mpHitChara = hitChara;
	// 開始した瞬間に敵の位置に合わせる（1フレームのズレを防ぐ）
	Position(mpHitChara->Position() + CVector(0.0f, 10.0f, 0.0f));
	Scale(CVector::one * mBloodScale * size);
	
	// アニメーションをリセットする
	SetAnimData(&msAnimData);
	
	// エフェクト開始時にアクティブ化・表示ON
	CTask::SetEnable(true);
	SetShow(true); // 描画フラグをONにする
	mIsPlay = true;
}
