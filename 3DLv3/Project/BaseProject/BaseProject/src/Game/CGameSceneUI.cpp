#include "CGameSceneUI.h"
#include "CText.h"
#include "CInput.h"
#include "CImage.h"

CGameSceneUI::CGameSceneUI()
: mpGaugeImg(nullptr)
, mpWhiteImg(nullptr)
, mMaxPoint(100)
, mCurrPoint(mMaxPoint)
, mPercent(1.0f)
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
	//mpGaugeImg->SetWorldUnitPerPixel(WORLD_UNIT_PER_PIXEL);
	//mpGaugeImg->SetDepthMask(true);
	mGaugeSize = mpGaugeImg->GetSize();

	// 白イメージを読み込み
	mpWhiteImg = new CImage
	(
		"UI\\white.png",
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
	//mpWhiteImg->SetWorldUnitPerPixel(WORLD_UNIT_PER_PIXEL);
}

CGameSceneUI::~CGameSceneUI()
{
	// 読み込んだイメージを削除
	SAFE_DELETE(mpGaugeImg);
	SAFE_DELETE(mpWhiteImg);
}
