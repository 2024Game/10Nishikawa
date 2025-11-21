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

	// 表示するメニュー情報をテキストに設定
	std::string str = "Depth\n \n50\n100\n150";
	mpText->SetText(str.c_str());

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