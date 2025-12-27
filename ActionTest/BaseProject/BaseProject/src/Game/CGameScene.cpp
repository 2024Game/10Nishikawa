#include "CGameScene.h"
#include "CSceneManager.h"
#include "CField.h"
#include "CPlayer.h"
#include "CGameCamera.h"
#include "CGameCamera2.h"
#include "CInput.h"
#include "CGameMenu.h"
#include "CBGMManager.h"
#include "CLineEffect.h"
#include "CCactus.h"
#include "CSoldier.h"

//コンストラクタ
CGameScene::CGameScene()
	: CSceneBase(EScene::eGame)
	, mpGameMenu(nullptr)
	, mTimeCount(0)
{
}

//デストラクタ
CGameScene::~CGameScene()
{
}

//シーン読み込み
void CGameScene::Load()
{
	// ゲーム画面はカーソル非表示
	CInput::ShowCursor(false);
	// 背景色設定
	System::SetClearColor(0.1921569f, 0.3019608f, 0.4745098f, 1.0f);

	//ここでゲーム中に必要な
	//リソースの読み込みやクラスの生成を行う

	CResourceManager::Load<CModel>(		"Field",			"Field\\field.obj");
	CResourceManager::Load<CModel>(		"FieldCube",		"Field\\Object\\cube.obj");
	CResourceManager::Load<CModel>(		"FieldCylinder",	"Field\\Object\\cylinder.obj");

	CResourceManager::Load<CModelX>(	"Player",			"Character\\TestPlayer\\Player.x");
	CResourceManager::Load<CModelX>(	"Cactus",			"Character\\Enemy\\Cactus\\cactus.x");
	CResourceManager::Load<CModelX>(	"Soldier",			"Character\\TestPlayer\\Player.x");

	CResourceManager::Load<CModel>(		"CactusNeedle",		"Character\\Enemy\\Cactus\\needle.obj");
	CResourceManager::Load<CTexture>(	"Laser",			"Effect\\laser.png");
	CResourceManager::Load<CTexture>(	"LightningBolt",	"Effect\\lightning_bolt.png");
	CResourceManager::Load<CModel>(		"Slash",			"Effect\\slash.obj");
	CResourceManager::Load<CSound>(		"SlashSound",		"Sound\\SE\\slash.wav");
	CResourceManager::Load<CModel>(		"Sword",			"Weapon\\Sword\\sword.obj");
	CResourceManager::Load<CModel>(		"Shield",			"Weapon\\Shield\\shield.obj");

	// ゲームBGMを読み込み
	CBGMManager::Instance()->Play(EBGMType::eGame);

	new CField();

	// Playerを作成
	mpPlayer = new CPlayer();
	mpPlayer->Scale(1.0f, 1.0f, 1.0f);

	// サボテンの敵を作成
	//CCactus* cactus = new CCactus();
	//cactus->Scale(1.5f, 1.5f, 1.5f);
	//cactus->Position(0.0f, 20.0f, -100.0f);

	// 兵士の敵を作成
	//CSoldier* soldier1 = new CSoldier();
	//soldier1->Scale(1.0f, 1.0f, 1.0f);
	//soldier1->Position(10.0f, 10.0f, -100.0f);

	// 兵士の敵をランダムにN体生成
	for (int i = 0; i < 1; ++i)
	{
		float x = -100.0f + static_cast<float>(rand()) / RAND_MAX * 200.0f; // -100〜100
		float y = 10.0f;
		float z = -100.0f + static_cast<float>(rand()) / RAND_MAX * 200.0f; // -100〜100

		CSoldier* soldier = new CSoldier(mpPlayer);
		soldier->Position(x, y, z);
	}

	// CGameCameraのテスト
	//CGameCamera* mainCamera = new CGameCamera
	//(
	//	//CVector(5.0f, -15.0f, 180.0f),
	//	CVector(0.0f, 50.0f, 75.0f),
	//	player->Position()
	//);

	// CGameCamera2のテスト
	CVector atPos = mpPlayer->Position() + CVector(0.0f, 10.0f, 0.0f);
	CGameCamera2* mainCamera = new CGameCamera2
	(
		atPos + CVector(0.0f, 0.0f, 40.0f),
		atPos
	);

	mainCamera->SetFollowTargetTf(mpPlayer);

	// ゲームメニューを作成
	mpGameMenu = new CGameMenu();
}

//シーンの更新処理
void CGameScene::Update()
{
	// BGM再生中でなければ、BGMを再生
	//if (!mpGameBGM->IsPlaying())
	//{
	//	mpGameBGM->PlayLoop(-1, 1.0f, false, 1.0f);
	//}

	if (CInput::PushKey('H'))
	{
		CSceneManager::Instance()->LoadScene(EScene::eTitle);
	}

	// ゲームメニューを開いてなければ、[Ｍ]キーでメニューを開く
	if (!mpGameMenu->IsOpened())
	{
		if (CInput::PushKey('M'))
		{
			mpGameMenu->Open();
		}
	}

	mTimeCount++;
	if (mTimeCount >= (60 * 240))
	{
		float x = -450.0f + static_cast<float>(rand()) / RAND_MAX * 900.0f; // -450〜450
		float y = 10.0f;
		float z = -450.0f + static_cast<float>(rand()) / RAND_MAX * 900.0f; // -450〜450

		CSoldier* soldier1 = new CSoldier(mpPlayer);
		soldier1->Position(x, y, z);
		mTimeCount = 0;
	}
}
