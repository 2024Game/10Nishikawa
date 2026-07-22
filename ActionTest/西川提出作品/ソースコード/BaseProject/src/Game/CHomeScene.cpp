#include "CHomeScene.h"
#include "CSceneManager.h"
//#include "CGameScene.h"

#include "CGameCamera.h"
#include "CGameCamera2.h"
#include "CInput.h"
#include "CBGMManager.h"
#include "CLineEffect.h"

#include "CSaveManager.h"
#include "CHomeSceneUI.h"

//コンストラクタ
CHomeScene::CHomeScene()
	: CSceneBase(EScene::eGame)
	, mpGameMenu(nullptr)
	, mpPlayer(nullptr)
	, mpWhistleSE(nullptr)
	, mInGame(false)
	, mpSaveManager(nullptr)
{
}

//デストラクタ
CHomeScene::~CHomeScene()
{
}

//シーン読み込み
void CHomeScene::Load()
{
	// ホーム画面はカーソル表示
	CInput::ShowCursor(true);
	// 背景色設定
	System::SetClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	//ここでゲーム中に必要な
	//リソースの読み込みやクラスの生成を行う


	// ゲームBGMを読み込み
	CBGMManager::Instance()->Play(EBGMType::eHome);

	mpSaveManager = &CSaveManager::Instance();
	// セーブファイルがあればロード、なければ初期値のまま
	if (!mpSaveManager->Load())
	{
		mpSaveManager->Reset();
	}

	// UI作成
	mpCHomeSceneUI = new CHomeSceneUI(mpSaveManager);
	AddTask(mpCHomeSceneUI);

	mpWhistleSE = CResourceManager::Get<CSound>("WhistleSound");
}

//シーンの更新処理
void CHomeScene::Update()
{
	/*
	if (CInput::PushKey('H'))
	{
		CSceneManager::Instance()->LoadScene(EScene::eTitle);
	}
	if (CInput::PushKey('R'))
	{
		mpSaveManager->Reset();
		mpCHomeSceneUI->InformationUpdate();
	}
	*/

	// タイトル画面が
	if (mpCHomeSceneUI->IsEnd())
	{
		// ゲーム開始ならば、ゲームシーンを読み込む
		if (mpCHomeSceneUI->IsStartGame())
		{
			CSceneManager::Instance()->LoadScene(EScene::eGame);
		}
		// 兵舎へ移行する
		else if (mpCHomeSceneUI->IsGoBarracks())
		{
			CSceneManager::Instance()->LoadScene(EScene::eBarracks);
		}
		// チュートリアルへ移行する
		else if (mpCHomeSceneUI->IsGoTutorial())
		{
			CSceneManager::Instance()->LoadScene(EScene::eTutorial);
		}
		// タイトルへ移行する
		else if (mpCHomeSceneUI->IsGoTitle())
		{
			CSceneManager::Instance()->LoadScene(EScene::eTitle);
		}
		// ゲーム終了ならば、アプリを閉じる
		else if (mpCHomeSceneUI->IsExitGame())
		{
			System::ExitGame();
		}
	}
}