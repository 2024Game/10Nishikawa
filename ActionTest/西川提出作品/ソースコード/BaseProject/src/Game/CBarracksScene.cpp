#include "CBarracksScene.h"
#include "CSceneManager.h"

#include "CGameCamera.h"
#include "CGameCamera2.h"
#include "CInput.h"
#include "CBGMManager.h"
#include "CLineEffect.h"

#include "CSaveManager.h"
#include "CBarracksSceneUI.h"

//コンストラクタ
CBarracksScene::CBarracksScene()
	: CSceneBase(EScene::eGame)
	, mpGameMenu(nullptr)
	, mpPlayer(nullptr)
	, mpWhistleSE(nullptr)
	, mInGame(false)
	, mpSaveManager(nullptr)
{
}

//デストラクタ
CBarracksScene::~CBarracksScene()
{
}

//シーン読み込み
void CBarracksScene::Load()
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
	mpCBarracksSceneUI = new CBarracksSceneUI(mpSaveManager);
	AddTask(mpCBarracksSceneUI);

	mpWhistleSE = CResourceManager::Get<CSound>("WhistleSound");
}

//シーンの更新処理
void CBarracksScene::Update()
{
	// 強化画面が
	if (mpCBarracksSceneUI->IsEnd())
	{
		// 「ホームに戻る」ならば、ホームシーンを読み込む
		if (mpCBarracksSceneUI->IsGoHome())
		{
			CSceneManager::Instance()->LoadScene(EScene::eHome);
		}
	}
}