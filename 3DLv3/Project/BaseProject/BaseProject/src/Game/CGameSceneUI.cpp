#include "CGameSceneUI.h"
#include "CText.h"
#include "CInput.h"

CGameSceneUI::CGameSceneUI()
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
}

CGameSceneUI::~CGameSceneUI()
{
}
