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
	, mpLogoFont(nullptr)
	, mpTitleLogo(nullptr)
	, mpTitleBg(nullptr)
{
	// タイトルロゴのフォントデータを生成
	mpLogoFont = new CFont("res\\Font\\misaki_gothic_2nd.ttf");
	mpLogoFont->SetFontSize(30);
	mpLogoFont->SetAlignment(FTGL::TextAlignment::ALIGN_LEFT);
	mpLogoFont->SetLineLength(WINDOW_WIDTH);

	// タイトルロゴのテキストを生成
	mpTitleLogo = new CText
	(
		mpLogoFont, 30,
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

	// [ゲーム開始]ボタンを生成
	CExpandButton* startBtn = new CExpandButton
	(
		CVector2(WINDOW_WIDTH - (250.0f + 20.0f), WINDOW_HEIGHT - (30.0f + 20.0f)),
		CVector2((500.0f * 0.9f), (60.0f * 0.9f)),
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	// ボタンの画像を読み込み
	startBtn->LoadButtonImage("UI/btn03_03_light.png", "UI/btn03_03_light.png");
	// ボタンクリック時に呼び出されるコールバック関数を設定
	startBtn->SetOnClickFunc(std::bind(&CHomeSceneUI::OnClickStartGame, this));
	// ボタンリストに追加
	mButtons.push_back(startBtn);
}

CHomeSceneUI::~CHomeSceneUI()
{
	SAFE_DELETE(mpLogoFont);
	SAFE_DELETE(mpTitleLogo);
	SAFE_DELETE(mpTitleBg);
	SAFE_DELETE(mpStartText);

	int size = mButtons.size();
	for (int i = 0; i < size; i++)
	{
		CButton* btn = mButtons[i];
		mButtons[i] = nullptr;
		SAFE_DELETE(btn);
	}
	mButtons.clear();
}

bool CHomeSceneUI::IsEnd() const
{
	return mIsEnd;
}

bool CHomeSceneUI::IsStartGame() const
{
	// 選択項目が1つ目ならば、ゲーム開始
	return mSelectIndex == 0;
}

bool CHomeSceneUI::IsGoTitle() const
{
	// 選択項目が2つ目ならば、タイトルへ移行
	return mSelectIndex == 1;
}

void CHomeSceneUI::Update()
{
	switch (mState)
	{
		// 待機状態
	case EState::eIdle:
		UpdateIdle();
		break;
		// メニューを開く
	case EState::eOpen:
		UpdateOpen();
		break;
		// メニュー選択
	case EState::eSelect:
		UpdateSelect();
		break;
		// フェードアウト
	case EState::eFadeOut:
		UpdateFadeOut();
		break;
	}

	mpTitleLogo->Update();
	for (CButton* btn : mButtons)
	{
		btn->Update();
	}
}

void CHomeSceneUI::Render()
{
	mpTitleLogo->Render();
	for (CButton* btn : mButtons)
	{
		btn->Render();
	}
}

void CHomeSceneUI::UpdateIdle()
{
}

void CHomeSceneUI::UpdateOpen()
{
}

void CHomeSceneUI::UpdateSelect()
{
}

void CHomeSceneUI::UpdateFadeOut()
{
}

void CHomeSceneUI::ChangeState(EState state)
{
	if (state == mState) return;
	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}

void CHomeSceneUI::OnClickStartGame()
{
	if (mIsEnd) return;

	mSelectIndex = 0;
	mIsEnd = true;
}

void CHomeSceneUI::OnClickGoTitle()
{
	if (mIsEnd) return;

	mSelectIndex = 1;
	mIsEnd = true;
}

void CHomeSceneUI::OnClickIncreaseHP()
{

}
