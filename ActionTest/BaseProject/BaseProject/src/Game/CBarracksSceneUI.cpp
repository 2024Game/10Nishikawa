#include "CBarracksSceneUI.h"
#include "CFont.h"
#include "CText.h"
#include "CImage.h"
#include "Maths.h"
#include "CInput.h"
#include "CFade.h"
#include "CExpandButton.h"
#include "Easing.h"


CBarracksSceneUI::CBarracksSceneUI(CSaveManager* saveManager)
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
	, mpSaveManager(saveManager)
	, mPageNum(0)
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
	mpBarracksBg = new CImage
	(
		"UI/barracks2.png",
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
			float y = 190.0f + j * 100.0f;

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
			y = 200.0f + j * 100.0f;

			img2->SetPos(x, y);

			mBgImages.push_back(img2);
		}
	}

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
	gImg->SetPos(WINDOW_WIDTH - 420.0f, 20.0f);
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

	// [ページめくり左]ボタンを生成
	CExpandButton* pageRBtn = new CExpandButton
	(
		CVector2(115.0f, 515.0f),
		CVector2(75.0f, 50.0f),
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	// ボタンの画像を読み込み
	pageRBtn->LoadButtonImage("UI/previouspage.png", "UI/previouspage.png");
	// ボタンクリック時に呼び出されるコールバック関数を設定
	pageRBtn->SetOnClickFunc(std::bind(&CBarracksSceneUI::OnClickPageUp, this));
	// ボタンリストに追加
	mButtons.push_back(pageRBtn);

	// [ページめくり右]ボタンを生成
	CExpandButton* pageLBtn = new CExpandButton
	(
		CVector2(415.0f, 515.0f),
		CVector2(75.0f, 50.0f),
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	// ボタンの画像を読み込み
	pageLBtn->LoadButtonImage("UI/nextpage.png", "UI/nextpage.png");
	// ボタンクリック時に呼び出されるコールバック関数を設定
	pageLBtn->SetOnClickFunc(std::bind(&CBarracksSceneUI::OnClickPageUp, this));
	// ボタンリストに追加
	mButtons.push_back(pageLBtn);


	// [ホームに戻る]ボタンを生成
	CExpandButton* homeBtn = new CExpandButton
	(
		CVector2(115.0f, WINDOW_HEIGHT - (40.0f + 10.0f)),
		CVector2(200.0f, 50.0f),
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	// ボタンの画像を読み込み
	homeBtn->LoadButtonImage("UI/gohome.png", "UI/gohome.png");
	// ボタンクリック時に呼び出されるコールバック関数を設定
	homeBtn->SetOnClickFunc(std::bind(&CBarracksSceneUI::OnClickGoHome, this));
	// ボタンリストに追加
	mButtons.push_back(homeBtn);

	// ボタン群を生成
	int btnNum = 0;
	if (mPageNum == 1) btnNum = 4;
	for (int i = 0; i < mCols; i++)        // 列
	{
		for (int j = 0; j < mRows; j++)    // 行
		{
			float x = 122.5f + i * 195.0f;
			float y = 460.0f + j * 100.0f;

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
				// [体力UP]ボタンを設定
				// ボタンの画像を読み込み
				Btn->LoadButtonImage("UI/upgrade.png", "UI/upgrade.png");
				// ボタンクリック時に呼び出されるコールバック関数を設定
				Btn->SetOnClickFunc(std::bind(&CBarracksSceneUI::OnClickHpUp, this));
				break;
			case 1:
				// [試合後HP回復UP]ボタンを設定
				// ボタンの画像を読み込み
				Btn->LoadButtonImage("UI/upgrade.png", "UI/upgrade.png");
				// ボタンクリック時に呼び出されるコールバック関数を設定
				Btn->SetOnClickFunc(std::bind(&CBarracksSceneUI::OnClickHpRegeneUp, this));
				break;
			case 2:
				// [スタミナUP]ボタンを設定
				// ボタンの画像を読み込み
				Btn->LoadButtonImage("UI/upgrade.png", "UI/upgrade.png");
				// ボタンクリック時に呼び出されるコールバック関数を設定
				Btn->SetOnClickFunc(std::bind(&CBarracksSceneUI::OnClickStUp, this));
				break;
			case 3:
				// [スタミナ回復UP]ボタンを設定
				// ボタンの画像を読み込み
				Btn->LoadButtonImage("UI/upgrade.png", "UI/upgrade.png");
				// ボタンクリック時に呼び出されるコールバック関数を設定
				Btn->SetOnClickFunc(std::bind(&CBarracksSceneUI::OnClickStRegeneUp, this));
				break;
			case 4:
				// [攻撃力UP]ボタンを設定
				// ボタンの画像を読み込み
				Btn->LoadButtonImage("UI/upgrade.png", "UI/upgrade.png");
				// ボタンクリック時に呼び出されるコールバック関数を設定
				Btn->SetOnClickFunc(std::bind(&CBarracksSceneUI::OnClickAttackUp, this));
				break;
			case 5:
				// [治療院の効果UP]ボタンを設定
				// ボタンの画像を読み込み
				Btn->LoadButtonImage("UI/upgrade.png", "UI/upgrade.png");
				// ボタンクリック時に呼び出されるコールバック関数を設定
				Btn->SetOnClickFunc(std::bind(&CBarracksSceneUI::OnClickHealerUp, this));
				break;
			case 6:
				// [引き継ぎLvUP]ボタンを設定
				// ボタンの画像を読み込み
				Btn->LoadButtonImage("UI/upgrade.png", "UI/upgrade.png");
				// ボタンクリック時に呼び出されるコールバック関数を設定
				Btn->SetOnClickFunc(std::bind(&CBarracksSceneUI::OnClickReincarnationUp, this));
				break;
			default:
				Btn->LoadButtonImage("UI/upgrade.png", "UI/upgrade.png");
				break;
			}
			// ボタンリストに追加
			mURButtons.push_back(Btn);
			btnNum++;
		}
	}

	InformationUpdate();
}

CBarracksSceneUI::~CBarracksSceneUI()
{
	SAFE_DELETE(mpBarracksBg);
	SAFE_DELETE(mpMoneyFont);
	SAFE_DELETE(mpStatusFont);
	SAFE_DELETE(mpPerkFont);
	//SAFE_DELETE(mpMoneyText);
	//SAFE_DELETE(mpStatusText);

	int size = mButtons.size();
	for (int i = 0; i < size; i++)
	{
		CButton* btn = mButtons[i];
		mButtons[i] = nullptr;
		SAFE_DELETE(btn);
	}
	mButtons.clear();

	size = mURButtons.size();
	for (int i = 0; i < size; i++)
	{
		CButton* urBtn = mURButtons[i];
		mURButtons[i] = nullptr;
		SAFE_DELETE(urBtn);
	}
	mURButtons.clear();

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

bool CBarracksSceneUI::IsEnd() const
{
	return mIsEnd;
}

bool CBarracksSceneUI::IsGoHome() const
{
	// 選択項目が1つ目ならば、ホームへ移行
	return mSelectIndex == 0;
}

void CBarracksSceneUI::Update()
{
	mpBarracksBg->Update();
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

	for (CButton* urBtn : mURButtons)
	{
		urBtn->Update();
	}

	for (CText* txt : mTexts)
	{
		//txt->Update();
	}
}

void CBarracksSceneUI::Render()
{
	mpBarracksBg->Render();

	for (CImage* img : mBgImages)
	{
		img->Render();
	}

	for (CButton* btn : mButtons)
	{
		btn->Render();
	}

	for (CButton* urBtn : mURButtons)
	{
		urBtn->Render();
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

void CBarracksSceneUI::InformationUpdate()
{
	int size = mURTexts.size();
	for (int i = 0; i < size; i++)
	{
		CText* urtxt = mURTexts[i];
		mURTexts[i] = nullptr;
		SAFE_DELETE(urtxt);
	}
	mURTexts.clear();

	int btnNum = 0;
	if (mPageNum == 1) btnNum = 4;
	size = mURButtons.size();
	for (int i = 0; i < size; i++)
	{
		switch (btnNum)
		{
		case 0:
			// [体力UP]ボタンを設定
			// ボタンの画像を読み込み
			//mURButtons[i]->LoadButtonImage("UI/btn03_03_light.png", "UI/btn03_03_light.png");
			// ボタンクリック時に呼び出されるコールバック関数を設定
			mURButtons[i]->SetOnClickFunc(std::bind(&CBarracksSceneUI::OnClickHpUp, this));
			break;
		case 1:
			// [試合後HP回復UP]ボタンを設定
			// ボタンの画像を読み込み
			//mURButtons[i]->LoadButtonImage("UI/btn03_03_light.png", "UI/btn03_03_light.png");
			// ボタンクリック時に呼び出されるコールバック関数を設定
			mURButtons[i]->SetOnClickFunc(std::bind(&CBarracksSceneUI::OnClickHpRegeneUp, this));
			break;
		case 2:
			// [スタミナUP]ボタンを設定
			// ボタンの画像を読み込み
			//mURButtons[i]->LoadButtonImage("UI/btn03_03_light.png", "UI/btn03_03_light.png");
			// ボタンクリック時に呼び出されるコールバック関数を設定
			mURButtons[i]->SetOnClickFunc(std::bind(&CBarracksSceneUI::OnClickStUp, this));
			break;
		case 3:
			// [スタミナ回復UP]ボタンを設定
			// ボタンの画像を読み込み
			//mURButtons[i]->LoadButtonImage("UI/btn03_03_light.png", "UI/btn03_03_light.png");
			// ボタンクリック時に呼び出されるコールバック関数を設定
			mURButtons[i]->SetOnClickFunc(std::bind(&CBarracksSceneUI::OnClickStRegeneUp, this));
			break;
		case 4:
			// [攻撃力UP]ボタンを設定
			// ボタンの画像を読み込み
			//mURButtons[i]->LoadButtonImage("UI/btn03_03_light.png", "UI/btn03_03_light.png");
			// ボタンクリック時に呼び出されるコールバック関数を設定
			mURButtons[i]->SetOnClickFunc(std::bind(&CBarracksSceneUI::OnClickAttackUp, this));
			break;
		case 5:
			// [治療院の効果UP]ボタンを設定
			// ボタンの画像を読み込み
			//mURButtons[i]->LoadButtonImage("UI/btn03_03_light.png", "UI/btn03_03_light.png");
			// ボタンクリック時に呼び出されるコールバック関数を設定
			mURButtons[i]->SetOnClickFunc(std::bind(&CBarracksSceneUI::OnClickHealerUp, this));
			break;
		case 6:
			// [引き継ぎLvUP]ボタンを設定
			// ボタンの画像を読み込み
			//mURButtons[i]->LoadButtonImage("UI/btn03_03_light.png", "UI/btn03_03_light.png");
			
			// ボタンクリック時に呼び出されるコールバック関数を設定
			mURButtons[i]->SetOnClickFunc(std::bind(&CBarracksSceneUI::OnClickReincarnationUp, this));
			break;
		case 7:
			// [引き継ぎLvUP]ボタンを設定
			// ボタンの画像を読み込み
			//mURButtons[i]->LoadButtonImage("UI/btn03_03_light.png", "UI/btn03_03_light.png");
			
			// ボタンクリック時に呼び出されるコールバック関数を設定
			mURButtons[i]->SetOnClickFunc(std::bind(&CBarracksSceneUI::OnClickReincarnationUp, this));
			break;
		default:
			mURButtons[i]->LoadButtonImage("UI/btn03_03_light.png", "UI/btn03_03_light.png");
			break;
		}
		btnNum++;
	}

	// 所持金のテキストを生成
	mpMoneyText = new CText
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
		"スタミナ回復量(秒間)\n" + std::to_string(8.25f * (1.0f + (mpSaveManager->data.stRegeneLv * 0.025f))) + "\n"
		"攻撃倍率\n" + std::to_string(1 + (mpSaveManager->data.attackLv * 0.05f)) + "\n"
		"試合後体力回復量\n" + std::to_string(mpSaveManager->data.maxHp * (mpSaveManager->data.hpRegeneLv * 0.01f) + 5) + "\n"
		).c_str());
	mpStatusText->SetShowDebug(true);

	// リストに追加
	mURTexts.push_back(mpStatusText);

	// テキスト群を生成
	int infoNum = 0;
	if (mPageNum == 1) infoNum = 4;
	for (int i = 0; i < mCols; i++)        // 列
	{
		for (int j = 0; j < mRows; j++)    // 行
		{
			int x = 40.0f + i * 195.0f;
			int y = 210.0f + j * 100.0f;

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
				newText->SetText("体力強化\n");
				break;

			case 1:
				newText->SetText("上質な睡眠\n");
				break;

			case 2:
				newText->SetText("スタミナ強化\n");
				break;

			case 3:
				newText->SetText("ST回復強化\n");
				break;

			case 4:
				newText->SetText("武器を強化\n");
				break;

			case 5:
				newText->SetText("治療院を強化\n");
				break;

			case 6:
				newText->SetText("後進育成\n");
				break;

			case 7:
				newText->SetText("None\n");
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
	if (mPageNum == 1) infoNum = 4;
	// テキスト群を生成
	for (int i = 0; i < mCols; i++)        // 列
	{
		for (int j = 0; j < mRows; j++)    // 行
		{
			int x = 40.0f + i * 195.0f;
			int y = 230.0f + j * 100.0f;

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

			int cost = 0;
			switch (infoNum)
			{
			case 0:
				cost = (mpSaveManager->data.hpLv + 1) * 150;
				newText->SetText((
					"最大体力を25増加\n強化費用\n$"
					+ std::to_string(cost)
					).c_str());
				break;

			case 1:
				cost = (mpSaveManager->data.hpRegeneLv + 1) * 75;
				newText->SetText((
					"試合後の\n回復強化\n最大体力の\nLv/1％\n強化費用\n$"
					+ std::to_string(cost)
					).c_str());
				break;

			case 2:
				cost = (mpSaveManager->data.stLv + 1) * 100;
				newText->SetText((
					"スタミナを5増加\n強化費用\n$"
					+ std::to_string(cost)
					).c_str());
				break;

			case 3:
				cost = (mpSaveManager->data.stRegeneLv + 1) * 150;
				newText->SetText((
					"ST回復量を\nLv/2.5％強化\n強化費用\n$"
					+ std::to_string(cost)
					).c_str());
				break;

			case 4:
				cost = (mpSaveManager->data.attackLv + 1) * 125;
				newText->SetText((
					"攻撃力を増加\n基礎攻撃力を\nLv/5％強化\n強化費用\n$"
					+ std::to_string(cost)
					).c_str());
				break;
			case 5:
				cost = (mpSaveManager->data.healerLv + 1) * 75;
				newText->SetText((
					"回復量を5増加\n強化費用\n$"
					+ std::to_string(cost)
					).c_str());
				break;
			case 6:
				cost = (mpSaveManager->data.hpLv + 1) * 0;
				newText->SetText((
					"近日公開\n強化費用\n$"
					+ std::to_string(cost)
					).c_str());
				break;
			case 7:
				cost = (mpSaveManager->data.hpLv + 1) * 0;
				newText->SetText((
					"近日公開\n強化費用\n$"
					+ std::to_string(cost)
					).c_str());
				break;
			}

			// リストに追加
			mURTexts.push_back(newText);
			infoNum++;
		}
	}
}

void CBarracksSceneUI::UpdateIdle()
{
}

void CBarracksSceneUI::UpdateOpen()
{
}

void CBarracksSceneUI::UpdateSelect()
{
}

void CBarracksSceneUI::UpdateFadeOut()
{
}

void CBarracksSceneUI::ChangeState(EState state)
{
	if (state == mState) return;
	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}

void CBarracksSceneUI::OnClickGoHome()
{
	if (mIsEnd) return;

	mSelectIndex = 0;
	mIsEnd = true;
}

void CBarracksSceneUI::OnClickPageUp()
{
	switch (mPageNum)
	{
	case 0:
		mPageNum++;
		break;

	case 1:
		mPageNum--;
		break;
	}

	InformationUpdate();
}

void CBarracksSceneUI::OnClickPageDown()
{
	switch (mPageNum)
	{
	case 0:
		mPageNum++;
		break;

	case 1:
		mPageNum--;
		break;
	}

	InformationUpdate();
}

void CBarracksSceneUI::OnClickHpUp()
{
	int cost = (mpSaveManager->data.hpLv + 1) * 150;
	if (mpSaveManager->data.money >= cost)
	{
		mpSaveManager->data.money -= cost;
		mpSaveManager->data.hpLv++;
		mpSaveManager->data.maxHp += 25;
		mpSaveManager->data.hp += 25;
		mpSaveManager->Save();
		InformationUpdate();
	}
	else
	{

	}
}

void CBarracksSceneUI::OnClickHpRegeneUp()
{
	int cost = (mpSaveManager->data.hpRegeneLv + 1) * 75;
	if (mpSaveManager->data.money >= cost)
	{
		mpSaveManager->data.money -= cost;
		mpSaveManager->data.hpRegeneLv++;
		mpSaveManager->Save();
		InformationUpdate();
	}
	else
	{

	}
}

void CBarracksSceneUI::OnClickStUp()
{
	int cost = (mpSaveManager->data.stLv + 1) * 100;
	if (mpSaveManager->data.money >= cost)
	{
		mpSaveManager->data.money -= cost;
		mpSaveManager->data.stLv++;
		mpSaveManager->Save();
		InformationUpdate();
	}
	else
	{

	}
}

void CBarracksSceneUI::OnClickStRegeneUp()
{
	int cost = (mpSaveManager->data.stRegeneLv + 1) * 150;
	if (mpSaveManager->data.money >= cost)
	{
		mpSaveManager->data.money -= cost;
		mpSaveManager->data.stRegeneLv++;
		mpSaveManager->Save();
		InformationUpdate();
	}
	else
	{

	}
}

void CBarracksSceneUI::OnClickAttackUp()
{
	int cost = (mpSaveManager->data.attackLv + 1) * 125;
	if (mpSaveManager->data.money >= cost)
	{
		mpSaveManager->data.money -= cost;
		mpSaveManager->data.attackLv++;
		mpSaveManager->Save();
		InformationUpdate();
	}
	else
	{

	}
}

void CBarracksSceneUI::OnClickHealerUp()
{
	int cost = (mpSaveManager->data.healerLv + 1) * 75;
	if (mpSaveManager->data.money >= cost)
	{
		mpSaveManager->data.money -= cost;
		mpSaveManager->data.healerLv++;
		mpSaveManager->Save();
		InformationUpdate();
	}
	else
	{

	}
}

void CBarracksSceneUI::OnClickReincarnationUp()
{
}
