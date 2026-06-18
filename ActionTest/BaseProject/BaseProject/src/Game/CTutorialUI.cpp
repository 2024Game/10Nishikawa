#include "CTutorialUI.h"
#include "CFont.h"
#include "CText.h"
#include "CImage.h"
#include "Maths.h"
#include "CInput.h"
#include "CFade.h"
#include "Easing.h"

CTutorialUI::CTutorialUI()
	: CTask(ETaskPriority::eUI, 0, ETaskPauseType::eDefault)
	, mState(EState::eTuto0)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mpIconImg(nullptr)
{
	// テキストのフォントデータを生成
	mpFont = new CFont("res\\Font\\YDWaosagi.otf");
	mpFont->SetFontSize(30);
	mpFont->SetAlignment(FTGL::TextAlignment::ALIGN_LEFT);
	mpFont->SetLineLength(WINDOW_WIDTH);

	// テキストの背景イメージを生成
	mpTextBg = new CImage
	(
		"UI/white.png",
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eDefault,
		false,
		false
	);
	mpTextBg->SetSize(250.0f, 100.0f);
	mpTextBg->SetPos(WINDOW_WIDTH - 260.0f, 10.0f);
	mpTextBg->SetColor(CColor::black);
	mpTextBg->SetAlpha(0.6f);

	// チュートリアルのテキストを生成
	mpText = new CText
	(
		mpFont, 20,
		CVector2(WINDOW_WIDTH - 260.0f, 160.0f),
		CVector2(260.0f, 530.0f),
		CColor(CColor::darkBrown),
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eDefault,
		false,
		false
	);
	mpText->SetTextAlignV(ETextAlignV::eTop);
	mpText->SetTextAlignH(ETextAlignH::eLeft);
	mpText->SetShowDebug(true);

	InformationUpdate();
}

CTutorialUI::~CTutorialUI()
{
	SAFE_DELETE(mpTextBg);
	SAFE_DELETE(mpFont);
	SAFE_DELETE(mpTextBg);
	SAFE_DELETE(mpIconImg);
}

void CTutorialUI::Update()
{
}

void CTutorialUI::Render()
{
	mpTextBg->Render();
	mpText->Render();
}

void CTutorialUI::ChangeState(EState state)
{
	if (state == mState) return;
	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
	InformationUpdate();
}

void CTutorialUI::InformationUpdate()
{
	switch (mState)
	{
	case (EState::eTuto0):
		mpText->SetText("チュートリアルへようこそ\n");
		break;
	case (EState::eTuto1):
		mpText->SetText("WASD：移動\n""攻撃：マウス左クリック\n");
		break;
	case (EState::eTuto2):
		mpText->SetText(
			"回避：マウス右クリック\n"
			"頭の上に下のアイコンが出ている間に回避すると、\nジャスト回避"
		);
		break;
	case (EState::eTuto3):
		mpText->SetText("ジャスト回避後に攻撃を入力で\n強力なカウンター攻撃");
		break;
	case (EState::eTuto4):
		mpText->SetText("チュートリアルへようこそ\n");
		break;
	case (EState::eTuto5):
		mpText->SetText("チュートリアルへようこそ\n");
		break;
	case (EState::eTuto6):
		mpText->SetText("チュートリアルへようこそ\n");
		break;
	}
}
