#include "CHomeSceneUI.h"
#include "CFont.h"
#include "CText.h"
#include "CImage.h"
#include "Maths.h"
#include "CInput.h"
#include "CFade.h"
#include "CExpandButton.h"
#include "Easing.h"


CHomeSceneUI::CHomeSceneUI(CSaveManager* saveManager)
	: CTask(ETaskPriority::eUI, 0, ETaskPauseType::eDefault)
	, mState(EState::eIdle)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mIsEnd(false)
	, mpLogoFont(nullptr)
	, mpMoneyText(nullptr)
	, mpStatusText(nullptr)
	, mpStartText(nullptr)
	, mpSaveManager(saveManager)
{
	// タイトルロゴのフォントデータを生成
	mpLogoFont = new CFont("res\\Font\\misaki_gothic_2nd.ttf");
	mpLogoFont->SetFontSize(30);
	mpLogoFont->SetAlignment(FTGL::TextAlignment::ALIGN_LEFT);
	mpLogoFont->SetLineLength(WINDOW_WIDTH);

	// 背景イメージ達を生成
	mRows = 9;    // 1列あたりの項目数
	mCols = 1;    // 列数 <- UIのサイズ的に１列で良かった(つまり頑張り損)

	for (int i = 0; i < mCols; i++)        // 列
	{
		for (int j = 0; j < mRows; j++)    // 行
		{
			// 項目の背景イメージ
			CImage* img1 = new CImage
			(
				"UI/white.png",
				ETaskPriority::eUI,
				0,
				ETaskPauseType::eDefault,
				false,
				false
			);
			img1->SetSize(900.0f, 100.0f);

			float x = 20.0f + i * 920.0f;
			float y = 40.0f + j * 115.0f;

			img1->SetPos(x, y);

			mBgImages.push_back(img1);

			// 項目のアイコンイメージ
			CImage* img2 = new CImage
			(
				"UI/btn03_04_light.png",
				ETaskPriority::eUI,
				0,
				ETaskPauseType::eDefault,
				false,
				false
			);
			img2->SetSize(90.0f, 90.0f);

			x = 25.0f + i * 920.0f;
			y = 45.0f + j * 115.0f;

			img2->SetPos(x, y);

			mBgImages.push_back(img2);
		}
	}

	// ボタン群を生成
	int btnNum = 0;
	for (int i = 0; i < mCols; i++)        // 列
	{
		for (int j = 0; j < mRows; j++)    // 行
		{
			float x = 855.0f + i * 920.0f;
			float y = 90.0f + j * 115.0f;

			// ボタンを生成
			CExpandButton* Btn = new CExpandButton
			(
				CVector2(x, y),
				CVector2(120.0f, 90.0f),
				ETaskPriority::eUI, 0, ETaskPauseType::eGame,
				false, false
			);

			switch (btnNum)
			{
			case 0:
				// [燃料タンク増加]ボタンを設定
				// ボタンの画像を読み込み
				Btn->LoadButtonImage("UI/btn03_04_light.png", "UI/btn03_04_light.png");
				// ボタンクリック時に呼び出されるコールバック関数を設定
				Btn->SetOnClickFunc(std::bind(&CHomeSceneUI::OnClickIncreaseHP, this));
				break;
			case 1:
				// [船体速度UP]ボタンを設定
				Btn->LoadButtonImage("UI/btn03_04_light.png", "UI/btn03_04_light.png");
				Btn->SetOnClickFunc(std::bind(&CHomeSceneUI::OnClickIncreaseBoatSpeed, this));
				break;
			default:
				Btn->LoadButtonImage("UI/btn03_04_light.png", "UI/btn03_04_light.png");
				break;
			}
			// ボタンリストに追加
			mButtons.push_back(Btn);
			btnNum++;
		}
	}

	InformationUpdate();

	// 所持金表示枠
	CImage* gImg = new CImage
	(
		"UI/white.png",
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eDefault,
		false,
		false
	);
	gImg->SetSize(600.0f, 70.0f);
	gImg->SetPos(WINDOW_WIDTH - 630.0f, 40.0f);
	// リストに追加
	mBgImages.push_back(gImg);

	// 所持金のテキストを生成
	mpMoneyText = new CText
	(
		mpLogoFont, 30,
		CVector2(WINDOW_WIDTH - 620.0f, 60.0f),
		CVector2(580.0f, 60.0f),
		CColor(0.11f, 0.1f, 0.1f),
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eDefault,
		false,
		false
	);
	mpMoneyText->SetTextAlignV(ETextAlignV::eMiddle);

	int money = static_cast<int>(mpSaveManager->data.money);
	mpMoneyText->SetText(("所持金：" + std::to_string(money) + "p\n").c_str());
	// リストに追加
	mTexts.push_back(mpMoneyText);

	// ステータス表示枠
	CImage* stImg = new CImage
	(
		"UI/white.png",
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eDefault,
		false,
		false
	);
	stImg->SetSize(600.0f, 790.0f);
	stImg->SetPos(WINDOW_WIDTH - 630.0f, 130.0f);
	// リストに追加
	mBgImages.push_back(stImg);
	
	// ステータスのテキストを生成
	mpStatusText = new CText
	(
		mpLogoFont, 30,
		CVector2(WINDOW_WIDTH - 630.0f, 130.0f),
		CVector2(580.0f, 770.0f),
		CColor(0.11f, 0.1f, 0.1f),
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eDefault,
		false,
		false
	);
	mpStatusText->SetTextAlignV(ETextAlignV::eMiddle);
	mpStatusText->SetText("所持金：0p\n");
	// リストに追加
	mTexts.push_back(mpStatusText);

	// [ゲーム開始]ボタンを生成
	CExpandButton* startBtn = new CExpandButton
	(
		CVector2(WINDOW_WIDTH - (250.0f + 50.0f), WINDOW_HEIGHT - (30.0f + 15.0f)),
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

	// [タイトル]ボタンを生成
	CExpandButton* titleBtn = new CExpandButton
	(
		CVector2(WINDOW_WIDTH - (250.0f + 50.0f), WINDOW_HEIGHT - (30.0f + 80.0f)),
		CVector2((500.0f * 0.9f), (60.0f * 0.9f)),
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	// ボタンの画像を読み込み
	titleBtn->LoadButtonImage("UI/btn03_03_light.png", "UI/btn03_03_light.png");
	// ボタンクリック時に呼び出されるコールバック関数を設定
	titleBtn->SetOnClickFunc(std::bind(&CHomeSceneUI::OnClickGoTitle, this));
	// ボタンリストに追加
	mButtons.push_back(titleBtn);

	
}

CHomeSceneUI::~CHomeSceneUI()
{
	SAFE_DELETE(mpLogoFont);
	//SAFE_DELETE(mpMoneyText);
	//SAFE_DELETE(mpStatusText);
	//SAFE_DELETE(mpStartText); 

	int size = mButtons.size();
	for (int i = 0; i < size; i++)
	{
		CButton* btn = mButtons[i];
		mButtons[i] = nullptr;
		SAFE_DELETE(btn);
	}
	mButtons.clear();

	size = mBgImages.size();
	for (int i = 0; i < size; i++)
	{
		CImage* img = mBgImages[i];
		mBgImages[i] = nullptr;
		SAFE_DELETE(img);
	}
	mBgImages.clear();

	size = mTexts.size();
	for (int i = 0; i < size; i++)
	{
		CText* txt = mTexts[i];
		mTexts[i] = nullptr;
		SAFE_DELETE(txt);
	}
	mTexts.clear();

	size = mURTexts.size();
	for (int i = 0; i < size; i++)
	{
		CText* urtxt = mURTexts[i];
		mURTexts[i] = nullptr;
		SAFE_DELETE(urtxt);
	}
	mURTexts.clear();
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

	for (CButton* btn : mButtons)
	{
		btn->Update();
	}

	for (CText* txt : mTexts)
	{
		//txt->Update();
	}
}

void CHomeSceneUI::Render()
{
	for (CImage* img : mBgImages)
	{
		img->Render();
	}

	for (CButton* btn : mButtons)
	{
		btn->Render();
	}

	for (CText* txt : mTexts)
	{
		txt->Render();
	}
	
	for (CText* urtxt : mURTexts)
	{
		urtxt->Render();
	}
}

void CHomeSceneUI::InformationUpdate()
{
	int size = mURTexts.size();
	for (int i = 0; i < size; i++)
	{
		CText* urtxt = mURTexts[i];
		mURTexts[i] = nullptr;
		SAFE_DELETE(urtxt);
	}
	mURTexts.clear();
	
	int infoNum = 0;

	// テキスト群を生成
	for (int i = 0; i < mCols; i++)        // 列
	{
		for (int j = 0; j < mRows; j++)    // 行
		{
			float x = 120.0f + i * 920.0f;
			float y = 40.0f + j * 115.0f;

			// テキストを生成
			CText* newText = new CText
			(
				mpLogoFont, 30,
				CVector2(x, y),
				CVector2(790.0f, 100.0f),	// <-----ココのサイズを変えても右端が変わらないっぽい
				CColor(0.11f, 0.1f, 0.1f),
				ETaskPriority::eUI,
				0,
				ETaskPauseType::eDefault,
				false,
				false
			);
			newText->SetTextAlignV(ETextAlignV::eMiddle);
			//newText->SetTextAlignH(ETextAlignH::eLeft);
			//newText->SetTextAlignH(ETextAlignH::eRight);
			newText->SetTextAlignH(ETextAlignH::eCenter);
			//newText->SetShowDebug(true);
			
			switch (infoNum)
			{
			case 0:
				newText->SetText
				(("燃料タンクの容量　Lv." + std::to_string(mpSaveManager->data.fuelTankLv) + "\n" + "レベルごとに5UP").c_str());
				break;
			case 1:
				newText->SetText
				(("船体速度　Lv." + std::to_string(mpSaveManager->data.playerSpeedLv) + "\n" + "レベルごとに5" + "％UP").c_str());
				break;
			default:
				
				break;
			}
			
			// リストに追加
			mURTexts.push_back(newText);
			infoNum++;
		}
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
	int cost = 500 * (mpSaveManager->data.fuelTankLv + 1);
	if (mpSaveManager->data.money > cost)
	{
		mpSaveManager->data.money -= cost;
		mpSaveManager->data.fuelTankLv++;
		mpSaveManager->Save();
		InformationUpdate();
	}
}

void CHomeSceneUI::OnClickIncreaseBoatSpeed()
{
	int cost = 500 * (mpSaveManager->data.playerSpeedLv + 1);
	if (mpSaveManager->data.money > cost)
	{
		mpSaveManager->data.money -= cost;
		mpSaveManager->data.playerSpeedLv++;
		mpSaveManager->Save();
		InformationUpdate();
	}
}
