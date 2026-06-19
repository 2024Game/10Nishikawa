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
	, mpMoneyFont(nullptr)
	, mpStatusFont(nullptr)
	, mpPerkFont(nullptr)
	, mpDayText(nullptr)
	, mpMoneyText(nullptr)
	, mpStatusText(nullptr)
	, mpStartText(nullptr)
	, mpSaveManager(saveManager)
{
	// 所持金のフォントデータを生成
	mpMoneyFont = new CFont("res\\Font\\YDWaosagi.otf");
	mpMoneyFont->SetFontSize(30);
	mpMoneyFont->SetAlignment(FTGL::TextAlignment::ALIGN_LEFT);
	mpMoneyFont->SetLineLength(WINDOW_WIDTH);

	// ステータスのフォントデータを生成
	mpStatusFont = new CFont("res\\Font\\YDWaosagi.otf");
	mpStatusFont->SetFontSize(30);
	mpStatusFont->SetAlignment(FTGL::TextAlignment::ALIGN_LEFT);
	mpStatusFont->SetLineLength(WINDOW_WIDTH);

	// 次の日のフォントデータを生成
	mpPerkFont = new CFont("res\\Font\\YDWaosagi.otf");
	mpPerkFont->SetFontSize(30);
	mpPerkFont->SetAlignment(FTGL::TextAlignment::ALIGN_LEFT);
	mpPerkFont->SetLineLength(WINDOW_WIDTH);

	// タイトル画面の背景イメージを生成
	mpHomeBg = new CImage
	(
		"UI/barracks.png",
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eDefault,
		false,
		false
	);

	// 背景イメージ達を生成
	mRows = 1;    // 1列あたりの項目数
	mCols = 4;    // 列数 <- UIのサイズ的に１列で良かった(つまり頑張り損)

	// 次の日の背景イメージ
	CImage* img = new CImage
	(
		"UI/turikanban.png",
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eDefault,
		false,
		false
	);
	img->SetSize(855.0f, 570.0f);
	img->SetPos(-20.0f, -20.0f);
	img->SetColor(CColor::white);
	img->SetAlpha(1.0f);
	mBgImages.push_back(img);

	for (int i = 0; i < mCols; i++)        // 列
	{
		for (int j = 0; j < mRows; j++)    // 行
		{
			// 項目の背景イメージ
			CImage* img1 = new CImage
			(
				"UI/op10.png",
				ETaskPriority::eUI,
				0,
				ETaskPauseType::eDefault,
				false,
				false
			);
			img1->SetSize(185.0f, 300.0f);

			float x = 30.0f + i * 195.0f;
			float y = 240.0f + j * 100.0f;

			img1->SetPos(x, y);
			img1->SetColor(CColor::white);

			mBgImages.push_back(img1);

			// 項目のアイコンイメージ
			CImage* img2 = new CImage
			(
				"UI/btn03_03_light.png",
				ETaskPriority::eUI,
				0,
				ETaskPauseType::eDefault,
				false,
				false
			);
			img2->SetSize(165.0f, 40.0f);

			x = 40.0f + i * 195.0f;
			y = 250.0f + j * 100.0f;

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
			float x = 122.5f + i * 195.0f;
			float y = 510.0f + j * 100.0f;

			// ボタンを生成
			CExpandButton* Btn = new CExpandButton
			(
				CVector2(x, y),
				CVector2(165.0f, 40.0f),
				ETaskPriority::eUI, 0, ETaskPauseType::eGame,
				false, false
			);

			switch (btnNum)
			{
			case 0:
				// [治療院へ行く]ボタンを設定
				// ボタンの画像を読み込み
				Btn->LoadButtonImage("UI/gogame.png", "UI/gogame.png");
				// ボタンクリック時に呼び出されるコールバック関数を設定
				Btn->SetOnClickFunc(std::bind(&CHomeSceneUI::OnClickHealer, this));
				break;
			case 1:
				// [格下との戦い]ボタンを設定
				// ボタンの画像を読み込み
				Btn->LoadButtonImage("UI/gogame.png", "UI/gogame.png");
				// ボタンクリック時に呼び出されるコールバック関数を設定
				Btn->SetOnClickFunc(std::bind(&CHomeSceneUI::OnClickEasy, this));
				break;
			case 2:
				// [同格との戦い]ボタンを設定
				// ボタンの画像を読み込み
				Btn->LoadButtonImage("UI/gogame.png", "UI/gogame.png");
				// ボタンクリック時に呼び出されるコールバック関数を設定
				Btn->SetOnClickFunc(std::bind(&CHomeSceneUI::OnClickNormal, this));
				break;
			case 3:
				// [格上との戦い]ボタンを設定
				// ボタンの画像を読み込み
				Btn->LoadButtonImage("UI/gogame.png", "UI/gogame.png");
				// ボタンクリック時に呼び出されるコールバック関数を設定
				Btn->SetOnClickFunc(std::bind(&CHomeSceneUI::OnClickHard, this));
				break;
			default:
				Btn->LoadButtonImage("UI/gogame.png", "UI/gogame.png");
				break;
			}
			// ボタンリストに追加
			mButtons.push_back(Btn);
			btnNum++;
		}
	}

	InformationUpdate();

	// Day表示枠
	CImage* dayImg = new CImage
	(
		"UI/btn03_03_light.png",
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eDefault,
		false,
		false
	);
	dayImg->SetSize(400.0f, 50.0f);
	dayImg->SetPos(WINDOW_WIDTH - 420.0f, 20.0f);
	// リストに追加
	mBgImages.push_back(dayImg);

	// 所持金表示枠
	CImage* gImg = new CImage
	(
		"UI/btn03_03_light.png",
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eDefault,
		false,
		false
	);
	gImg->SetSize(400.0f, 50.0f);
	gImg->SetPos(WINDOW_WIDTH - 420.0f, 80.0f);
	// リストに追加
	mBgImages.push_back(gImg);

	// ステータス表示枠
	CImage* stImg = new CImage
	(
		"UI/op10.png",
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eDefault,
		false,
		false
	);
	stImg->SetSize(400.0f, 570.0f);
	stImg->SetPos(WINDOW_WIDTH - 420.0f, 140.0f);
	stImg->SetAlpha(0.90f);
	// リストに追加
	mBgImages.push_back(stImg);


	// [ゲーム終了]ボタンを生成
	CExpandButton* startBtn = new CExpandButton
	(
		CVector2(115.0f, WINDOW_HEIGHT - (40.0f + 10.0f)),
		CVector2(200.0f, 50.0f),
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	// ボタンの画像を読み込み
	startBtn->LoadButtonImage("UI/goquit.png", "UI/goquit.png");
	// ボタンクリック時に呼び出されるコールバック関数を設定
	startBtn->SetOnClickFunc(std::bind(&CHomeSceneUI::OnClickQuit, this));
	// ボタンリストに追加
	mButtons.push_back(startBtn);

	// [タイトル画面へ]ボタンを生成
	CExpandButton* titleBtn = new CExpandButton
	(
		CVector2(330.0f, WINDOW_HEIGHT - (40.0f + 10.0f)),
		CVector2(200.0f, 50.0f),
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	// ボタンの画像を読み込み
	titleBtn->LoadButtonImage("UI/gotitle.png", "UI/gotitle.png");
	// ボタンクリック時に呼び出されるコールバック関数を設定
	titleBtn->SetOnClickFunc(std::bind(&CHomeSceneUI::OnClickGoTitle, this));
	// ボタンリストに追加
	mButtons.push_back(titleBtn);

	// [強化画面へ]ボタンを生成
	CExpandButton* barracksBtn = new CExpandButton
	(
		CVector2(545.0f, WINDOW_HEIGHT - (40.0f + 10.0f)),
		CVector2(200.0f, 50.0f),
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	// ボタンの画像を読み込み
	barracksBtn->LoadButtonImage("UI/blacksmith.png", "UI/blacksmith.png");
	// ボタンクリック時に呼び出されるコールバック関数を設定
	barracksBtn->SetOnClickFunc(std::bind(&CHomeSceneUI::OnClickGoBarracks, this));
	// ボタンリストに追加
	mButtons.push_back(barracksBtn);

	// [チュートリアル]ボタンを生成
	CExpandButton* tutorialBtn = new CExpandButton
	(
		CVector2(760.0f, WINDOW_HEIGHT - (40.0f + 10.0f)),
		CVector2(200.0f, 50.0f),
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	// ボタンの画像を読み込み
	tutorialBtn->LoadButtonImage("UI/tutorial.png", "UI/tutorial.png");
	// ボタンクリック時に呼び出されるコールバック関数を設定
	tutorialBtn->SetOnClickFunc(std::bind(&CHomeSceneUI::OnClickGoTutorial, this));
	// ボタンリストに追加
	mButtons.push_back(tutorialBtn);
}

CHomeSceneUI::~CHomeSceneUI()
{
	SAFE_DELETE(mpHomeBg);
	SAFE_DELETE(mpMoneyFont);
	SAFE_DELETE(mpStatusFont);
	SAFE_DELETE(mpPerkFont);
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

bool CHomeSceneUI::IsExitGame() const
{
	// 選択項目が3つ目ならば、ゲームを終了
	return mSelectIndex == 2;
}

bool CHomeSceneUI::IsGoBarracks() const
{
	// 選択項目が4つ目ならば、兵舎へ移行
	return mSelectIndex == 3;
}

bool CHomeSceneUI::IsGoTutorial() const
{
	// 選択項目が5つ目ならば、チュートリアルへ移行
	return mSelectIndex == 4;
}

void CHomeSceneUI::Update()
{
	mpHomeBg->Update();
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
	mpHomeBg->Render();

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

	// Dayのテキストを生成
	mpDayText = new CText
	(
		mpMoneyFont, 30,
		CVector2(WINDOW_WIDTH - 410.0f, 35.0f),
		CVector2(380.0f, 40.0f),
		CColor(CColor::darkBrown),
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eDefault,
		false,
		false
	);
	mpDayText->SetTextAlignV(ETextAlignV::eMiddle);
	mpDayText->SetShowDebug(true);
	int day = static_cast<int>(mpSaveManager->data.day);
	mpDayText->SetText(("Day：" + std::to_string(day) + "\n").c_str());
	// リストに追加
	mURTexts.push_back(mpDayText);

	// 所持金のテキストを生成
	mpMoneyText = new CText
	(
		mpMoneyFont, 30,
		CVector2(WINDOW_WIDTH - 410.0f, 95.0f),
		CVector2(380.0f, 40.0f),
		CColor(CColor::darkBrown),
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eDefault,
		false,
		false
	);
	mpMoneyText->SetTextAlignV(ETextAlignV::eMiddle);
	mpMoneyText->SetShowDebug(true);
	int money = static_cast<int>(mpSaveManager->data.money);
	mpMoneyText->SetText(("所持金：$" + std::to_string(money) + "\n").c_str());
	// リストに追加
	mURTexts.push_back(mpMoneyText);

	// ステータスのテキストを生成
	mpStatusText = new CText
	(
		mpStatusFont, 30,
		CVector2(WINDOW_WIDTH - 400.0f, 160.0f),
		CVector2(360.0f, 530.0f),
		CColor(CColor::darkBrown),
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eDefault,
		false,
		false
	);
	mpStatusText->SetTextAlignV(ETextAlignV::eTop);
	mpStatusText->SetTextAlignH(ETextAlignH::eLeft);
	mpStatusText->SetText((
		"体力\n" + std::to_string((int)mpSaveManager->data.hp) + "/"
		+ std::to_string((int)mpSaveManager->data.maxHp) + "\n"
		"スタミナ\n" + std::to_string(150 + (mpSaveManager->data.stLv * 5)) + "\n"
		"スタミナ回復量(秒間)\n" + std::to_string(10.0f * (1.0f + (mpSaveManager->data.stRegeneLv * 0.01f))) + "\n"
		"攻撃倍率\n" + std::to_string(1 + (mpSaveManager->data.attackLv * 0.05f)) + "\n"
		"試合後体力回復量\n" + std::to_string(mpSaveManager->data.maxHp * (mpSaveManager->data.hpRegeneLv * 0.01f) + 5) + "\n"
		).c_str());
	mpStatusText->SetShowDebug(true);

	// リストに追加
	mURTexts.push_back(mpStatusText);

	// テキスト群を生成
	for (int i = 0; i < mCols; i++)        // 列
	{
		for (int j = 0; j < mRows; j++)    // 行
		{
			int x = 40.0f + i * 195.0f;
			int y = 260.0f + j * 100.0f;

			// テキストを生成
			CText* newText = new CText
			(
				mpPerkFont, 25,
				CVector2(x, y),
				CVector2(165.0f, 40.0f),	// <---ココのサイズを変えても右端が変わらないっぽい(解決済み)
				CColor(CColor::darkBrown),
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
			newText->SetShowDebug(true);


			switch (infoNum)
			{
			case 0:
				newText->SetText("治療院へ行く\n");
				break;

			case 1:
				newText->SetText("格下との戦い\n");
				break;

			case 2:
				newText->SetText("同格との戦い\n");
				break;

			case 3:
				newText->SetText("格上との戦い\n");
				break;

			default:

				break;
			}

			// リストに追加
			mURTexts.push_back(newText);
			infoNum++;
		}
	}

	infoNum = 0;
	// テキスト群を生成
	for (int i = 0; i < mCols; i++)        // 列
	{
		for (int j = 0; j < mRows; j++)    // 行
		{
			int x = 40.0f + i * 195.0f;
			int y = 300.0f + j * 100.0f;

			// テキストを生成
			CText* newText = new CText
			(
				mpPerkFont, 25,
				CVector2(x, y),
				CVector2(165.0f, 180.0f),	// <---ココのサイズを変えても右端が変わらないっぽい(解決済み)
				CColor(CColor::darkBrown),
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
			newText->SetShowDebug(true);

			
			switch (infoNum)
			{
			case 0:
				newText->SetText((
					"体力を" + std::to_string(50 + (mpSaveManager->data.healerLv * 5))
					+ "回復します\n1日経過します\n$100必要"
					).c_str());
				break;

			case 1:
				newText->SetText((
					"敵のレベル" + std::to_string((((int)mpSaveManager->data.day / 3) - 1) + 2)
					+ "\n報酬\n$?"
					).c_str());
				break;

			case 2:
				newText->SetText((
					"敵のレベル" + std::to_string(((int)mpSaveManager->data.day / 3) + 2)
					+ "\n報酬\n$??"
					).c_str());
				break;

			case 3:
				newText->SetText((
					"敵のレベル" + std::to_string((((int)mpSaveManager->data.day / 3) + 1) + 2)
					 + "\n報酬\n$???"
					).c_str());
				break;

			case 4:
				break;
			case 5:
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

void CHomeSceneUI::OnClickQuit()
{
	if (mIsEnd) return;

	mSelectIndex = 2;
	mIsEnd = true;
}

void CHomeSceneUI::OnClickGoBarracks()
{
	if (mIsEnd) return;

	mSelectIndex = 3;
	mIsEnd = true;
}

void CHomeSceneUI::OnClickGoTutorial()
{
	if (mIsEnd) return;

	mSelectIndex = 4;
	mIsEnd = true;
}

void CHomeSceneUI::OnClickGoReincarnation()
{
}

void CHomeSceneUI::OnClickHealer()
{
	if (mpSaveManager->data.money >= 100)
	{
		mpSaveManager->data.money -= 100;
		mpSaveManager->data.selectDiff = 0;
		// HPを最大HPの50、回復させる
		float hpCapa = mpSaveManager->data.maxHp - mpSaveManager->data.hp;
		float recovery = 50 + (mpSaveManager->data.healerLv * 5);
		if (hpCapa >= recovery)
		{
			mpSaveManager->data.hp += recovery;
		}
		else
		{
			mpSaveManager->data.hp = mpSaveManager->data.maxHp;
		}
		mpSaveManager->data.day++;
		mpSaveManager->Save();
	}
	InformationUpdate();
	//OnClickStartGame();
}

void CHomeSceneUI::OnClickEasy()
{
	mpSaveManager->data.selectDiff = 1;
	mpSaveManager->Save();
	OnClickStartGame();
}

void CHomeSceneUI::OnClickNormal()
{
	mpSaveManager->data.selectDiff = 2;
	mpSaveManager->Save();
	OnClickStartGame();
}

void CHomeSceneUI::OnClickHard()
{
	mpSaveManager->data.selectDiff = 3;
	mpSaveManager->Save();
	OnClickStartGame();
}
