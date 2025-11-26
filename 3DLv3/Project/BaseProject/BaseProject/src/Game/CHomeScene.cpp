#include "CHomeScene.h"
#include "CSceneManager.h"

#include "CGameCamera.h"
#include "CGameCamera2.h"
#include "CInput.h"
#include "CBGMManager.h"
#include "CLineEffect.h"

#include "CSaveManager.h"

//コンストラクタ
CHomeScene::CHomeScene()
	: CSceneBase(EScene::eGame)
	, mpGameMenu(nullptr)
	, mpPlayer(nullptr)
	, mpWhistleSE(nullptr)
	, mInGame(false)
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
	System::SetClearColor(0.1921569f, 0.3019608f, 0.4745098f, 1.0f);

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
	

	mpWhistleSE = CResourceManager::Get<CSound>("WhistleSound");
}

//シーンの更新処理
void CHomeScene::Update()
{
	CDebugPrint::Print("Money:%d\n", mpSaveManager->data.money);

	if (CInput::PushKey('H'))
	{
		CSceneManager::Instance()->LoadScene(EScene::eTitle);
	}
}