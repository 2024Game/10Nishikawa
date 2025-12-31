#include "CGameSceneUI.h"
#include "CText.h"
#include "CInput.h"
#include "CImage.h"
#include <Maths.h>

CGameSceneUI::CGameSceneUI(CPlayer* player)
	: mpGaugeImg1(nullptr)
	, mpWhiteImg1(nullptr)
	, mpGaugeImg2(nullptr)
	, mpWhiteImg2(nullptr)
	, mHpMaxPoint(player->GetMaxHp())
	, mHpCurrPoint(mHpMaxPoint)
	, mStMaxPoint(player->GetMaxSt())
	, mStCurrPoint(mStMaxPoint)
	, mHpPercent(1.0f)
	, mStPercent(1.0f)
	, mGaugeSize(CVector2(500.0f, 30.0f))
	, mHpGaugePos(CVector2(25.0f, 25.0f))
	, mStGaugePos(CVector2(25.0f, 50.0f))
	, mpPlayer(player)
{
	mpText = new CText
	(
		nullptr, 24,
		CVector2(WINDOW_WIDTH - 150.0f, WINDOW_HEIGHT - 150.0f),
		CVector2(100.0f, 100.0f),
		CColor::white
	);

	mpText->SetTextAlign(ETextAlignH::eCenter, ETextAlignV::eMiddle);

	// HPゲージ
	// ゲージのイメージを読み込み
	mpGaugeImg1 = new CImage
	(
		"UI\\gauge.png",
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mpGaugeImg1->SetSize(mGaugeSize);
	mpGaugeImg1->SetPos(mHpGaugePos);

	// 白イメージを読み込み
	mpWhiteImg1 = new CImage
	(
		"UI\\white.png",
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);

	// STゲージ
	// ゲージのイメージを読み込み
	mpGaugeImg2 = new CImage
	(
		"UI\\gauge.png",
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mpGaugeImg2->SetSize(mGaugeSize);
	mpGaugeImg2->SetPos(mStGaugePos);

	// 白イメージを読み込み
	mpWhiteImg2 = new CImage
	(
		"UI\\white.png",
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
}

CGameSceneUI::~CGameSceneUI()
{
	// 読み込んだイメージを削除
	SAFE_DELETE(mpGaugeImg1);
	SAFE_DELETE(mpWhiteImg1);
	SAFE_DELETE(mpGaugeImg2);
	SAFE_DELETE(mpWhiteImg2);
}

// HP最大値を設定
void CGameSceneUI::SetHpMaxPoint(float point)
{
	mHpMaxPoint = point;
	ApplyPoint(0);
}

// HP現在値を設定
void CGameSceneUI::SetHpCurrPoint(float point)
{
	mHpCurrPoint = point;
	ApplyPoint(0);
}

// ST最大値を設定
void CGameSceneUI::SetStMaxPoint(float point)
{
	mStMaxPoint = point;
	ApplyPoint(1);
}

// ST現在値を設定
void CGameSceneUI::SetStCurrPoint(float point)
{
	mStCurrPoint = point;
	ApplyPoint(1);
}

// ポイント残量の割合を設定（0.0～1.0）
void CGameSceneUI::SetHpPercent(float per)
{
	mHpPercent = Math::Clamp01(per);
}
// ポイント残量の割合を設定（0.0～1.0）
void CGameSceneUI::SetStPercent(float per)
{
	mStPercent = Math::Clamp01(per);
}

void CGameSceneUI::Update()
{
	SetHpCurrPoint(mpPlayer->GetHp());
	mpGaugeImg1->Update();
	mpWhiteImg1->Update();

	SetStCurrPoint(mpPlayer->GetSt());
	mpGaugeImg2->Update();
	mpWhiteImg2->Update();
	
	/*
	// 表示する情報をテキストに設定
	float depth = mpPlayer->GetDepth();
	int depthInt = static_cast<int>(depth);   // 小数切り捨て
	std::string str = "Depth\n \n" + std::to_string(depthInt) + "m";
	mpText->SetText(str.c_str());
	*/
	Render();
}

void CGameSceneUI::Render()
{
	// ゲージ背景を描画
	mpWhiteImg1->SetPos(mHpGaugePos);
	mpWhiteImg1->SetSize(mGaugeSize);
	mpWhiteImg1->SetColor(CColor::black);
	mpWhiteImg1->SetAlpha(0.5f);
	mpWhiteImg1->Render();

	mpWhiteImg2->SetPos(mStGaugePos);
	mpWhiteImg2->SetSize(mGaugeSize);
	mpWhiteImg2->SetColor(CColor::black);
	mpWhiteImg2->SetAlpha(0.5f);
	mpWhiteImg2->Render();

	// HPゲージ
	// バーのサイズ、座標、色を
	// ポイント残量の割合に合わせて調整して、バーを描画
	// バーのサイズを調整
	CVector2 barSize = mGaugeSize;
	barSize.X(barSize.X() * mHpPercent);
	mpWhiteImg1->SetSize(barSize);
	// バーの座標を調整
	CVector2 barPos = mGaugeSize - barSize;
	mpWhiteImg1->SetPos(-barPos + mHpGaugePos);
	mpWhiteImg1->SetPos(mHpGaugePos);
	// バーの色を設定
	CColor barColor = CColor::green;
	if (mHpPercent <= 0.25f) barColor = CColor::red;
	else if (mHpPercent <= 0.5f)barColor = CColor::yellow;
	mpWhiteImg1->SetColor(barColor);

	// STゲージ
	// バーのサイズを調整
	barSize = mGaugeSize;
	barSize.X(barSize.X() * mStPercent);
	mpWhiteImg2->SetSize(barSize);
	// バーの座標を調整
	barPos = mGaugeSize - barSize;
	mpWhiteImg2->SetPos(-barPos + mStGaugePos);
	mpWhiteImg2->SetPos(mStGaugePos);
	// バーの色を設定
	barColor = CColor::skyBlue;
	mpWhiteImg2->SetColor(barColor);

	// バーを描画
	mpWhiteImg1->Render();
	mpWhiteImg2->Render();
	// ゲージ本体を描画
	mpGaugeImg1->Render();
	mpGaugeImg2->Render();
}

// ポイント残量を反映
void CGameSceneUI::ApplyPoint(int num)
{
	switch (num)
	{
	case 0:
		// HP最大値が不正値でなければ
		if (mHpMaxPoint > 0)
		{
			// 現在値が最大値の何パーセントか求める
			mHpPercent = Math::Clamp01((float)mHpCurrPoint / mHpMaxPoint);
		}
		// 不正値ならば、100%固定
		else
		{
			mHpPercent = 1.0f;
		}
		break;
	case 1:
		// ST最大値が不正値でなければ
		if (mStMaxPoint > 0)
		{
			// 現在値が最大値の何パーセントか求める
			mStPercent = Math::Clamp01((float)mStCurrPoint / mStMaxPoint);
		}
		// 不正値ならば、100%固定
		else
		{
			mStPercent = 1.0f;
		}
		break;
	default:
		break;
	}
	
}