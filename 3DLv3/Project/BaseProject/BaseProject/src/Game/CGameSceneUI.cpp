#include "CGameSceneUI.h"
#include "CText.h"
#include "CInput.h"
#include "CImage.h"
#include <Maths.h>

CGameSceneUI::CGameSceneUI(CPlayer* player)
: mpGaugeImg(nullptr)
, mpWhiteImg(nullptr)
, mMaxPoint(player->GetMaxHp())
, mCurrPoint(mMaxPoint)
, mPercent(1.0f)
, mGaugeSize(CVector2(500.0f, 30.0f))
, mGaugePos(CVector2(25.0f, 25.0f))
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

	// ゲージのイメージを読み込み
	mpGaugeImg = new CImage
	(
		"UI\\gauge.png",
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mpGaugeImg->SetSize(mGaugeSize);
	mpGaugeImg->SetPos(mGaugePos);

	// 白イメージを読み込み
	mpWhiteImg = new CImage
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
	SAFE_DELETE(mpGaugeImg);
	SAFE_DELETE(mpWhiteImg);
}

// 最大値を設定
void CGameSceneUI::SetMaxPoint(float point)
{
	mMaxPoint = point;
	ApplyPoint();
}

// 現在値を設定
void CGameSceneUI::SetCurrPoint(float point)
{
	mCurrPoint = point;
	ApplyPoint();
}

// ポイント残量の割合を設定（0.0～1.0）
void CGameSceneUI::SetPercent(float per)
{
	mPercent = Math::Clamp01(per);
}

void CGameSceneUI::Update()
{
	SetCurrPoint(mpPlayer->GetHp());
	mpGaugeImg->Update();
	mpWhiteImg->Update();

	/*
	// 表示する情報をテキストに設定
	float depth = mpPlayer->GetDepth();
	int depthInt = static_cast<int>(depth);   // 小数切り捨て
	std::string str = "Depth\n \n" + std::to_string(depthInt) + "m";
	mpText->SetText(str.c_str());
	*/

	// DepthのUIをセット
	SetDepthInfo();

	Render();
}

void CGameSceneUI::Render()
{
	// ゲージ背景を描画
	mpWhiteImg->SetPos(mGaugePos);
	mpWhiteImg->SetSize(mGaugeSize);
	mpWhiteImg->SetColor(CColor::black);
	mpWhiteImg->SetAlpha(0.5f);
	mpWhiteImg->Render();

	// バーのサイズ、座標、色を
	// ポイント残量の割合に合わせて調整して、バーを描画
	// バーのサイズを調整
	CVector2 barSize = mGaugeSize;
	barSize.X(barSize.X() * mPercent);
	mpWhiteImg->SetSize(barSize);
	// バーの座標を調整
	CVector2 barPos = mGaugeSize - barSize;
	mpWhiteImg->SetPos(-barPos + mGaugePos);
	mpWhiteImg->SetPos(mGaugePos);
	// バーの色を設定
	CColor barColor = CColor::green;
	if (mPercent <= 0.25f) barColor = CColor::red;
	else if (mPercent <= 0.5f)barColor = CColor::yellow;
	mpWhiteImg->SetColor(barColor);
	// バーを描画
	mpWhiteImg->Render();

	// ゲージ本体を描画
	mpGaugeImg->Render();
}

// ポイント残量を反映
void CGameSceneUI::ApplyPoint()
{
	// 最大値が不正値でなければ
	if (mMaxPoint > 0)
	{
		// 現在値が最大値の何パーセントか求める
		mPercent = Math::Clamp01((float)mCurrPoint / mMaxPoint);
	}
	// 不正値ならば、100%固定
	else
	{
		mPercent = 1.0f;
	}
}

void CGameSceneUI::SetDepthInfo()
{
	float depth = mpPlayer->GetDepth();
	std::string str;

	if (depth == 50.0f)
	{
		str = "Depth\n \n>>50m<<\n100m\n150m\n";
	}
	else if (depth == 100.0f)
	{
		str = "Depth\n \n50m\n>>100m<<\n150m\n";
	}
	else if (depth == 150.0f)
	{
		str = "Depth\n \n100m\n>>150m<<\n200m\n";
	}
	else if (depth == 200.0f)
	{
		str = "Depth\n \n150m\n>>200m<<\n250m\n";
	}
	else if (depth == 250.0f)
	{
		str = "Depth\n \n200m\n>>250m<<\n300m\n";
	}
	else if (depth == 300.0f)
	{
		str = "Depth\n \n250m\n>>300m<<\n350m\n";
	}
	else if (depth == 350.0f)
	{
		str = "Depth\n \n300m\n>>350m<<\n400m\n";
	}
	else if (depth == 400.0f)
	{
		str = "Depth\n \n300m\n350m\n>>400m<<\n";
	}

	mpText->SetText(str.c_str());
}
