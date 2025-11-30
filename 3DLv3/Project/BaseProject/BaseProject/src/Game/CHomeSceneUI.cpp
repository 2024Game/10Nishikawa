#include "CHomeSceneUI.h"
#include "CFont.h"
#include "CText.h"
#include "CImage.h"
#include "Maths.h"
#include "CInput.h"
#include "CFade.h"
#include "CExpandButton.h"
#include "Easing.h"

CHomeSceneUI::CHomeSceneUI()
	: CTask(ETaskPriority::eUI, 0, ETaskPauseType::eDefault)
	, mState(EState::eIdle)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mIsEnd(false)
{
	// タイトルロゴのフォントデータを生成
	mpLogoFont = new CFont("res\\Font\\misaki_gothic_2nd.ttf");
	mpLogoFont->SetFontSize(30);
	mpLogoFont->SetAlignment(FTGL::TextAlignment::ALIGN_LEFT);
	mpLogoFont->SetLineLength(WINDOW_WIDTH);

	// タイトルロゴのテキストを生成
	mpTitleLogo = new CText
	(
		mpLogoFont, 500,
		CVector2(0.0f, 64.0f),
		CVector2(WINDOW_WIDTH, WINDOW_HEIGHT),
		CColor(0.11f, 0.1f, 0.1f),
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eDefault,
		false,
		false
	);
	mpTitleLogo->SetText("燃料タンク容量 Lv.1 cost : 300p\n船体スピード Lv.1 cost : 500p\n");
}

CHomeSceneUI::~CHomeSceneUI()
{
	SAFE_DELETE(mpLogoFont);
	SAFE_DELETE(mpTitleLogo);
	SAFE_DELETE(mpTitleBg);
	SAFE_DELETE(mpStartText);
}

void CHomeSceneUI::Update()
{
	mpTitleLogo->Update();
}

void CHomeSceneUI::Render()
{
	mpTitleLogo->Render();
}
