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
#include "CMinimap.h"
#include "CGameSceneUI.h"

#include "CRainbowTrout.h"
#include "CStripedBass.h"
#include "CTuna.h"

//コンストラクタ
CGameScene::CGameScene()
	: CSceneBase(EScene::eGame)
	, mpGameMenu(nullptr)
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
	CResourceManager::Load<CModel>(		"SeaSand",			"Field\\SeaSand.obj");
	CResourceManager::Load<CModel>(		"Skybox",			"Skybox\\Skybox2.obj");
	CResourceManager::Load<CModel>(		"FieldCube",		"Field\\Object\\cube.obj");
	CResourceManager::Load<CModel>(		"FieldCylinder",	"Field\\Object\\cylinder.obj");
	CResourceManager::Load<CModel>(		"Player",			"Character\\PlayerBoat\\PlayerBoat.obj");
	CResourceManager::Load<CModel>(		"Barrel",			"Objects\\Barrel\\barrel.obj");
	CResourceManager::Load<CModelX>(	"Cactus",			"Character\\Enemy\\Cactus\\cactus.x");
	CResourceManager::Load<CModel>(		"CactusNeedle",		"Character\\Enemy\\Cactus\\needle.obj");
	CResourceManager::Load<CTexture>(	"Laser",			"Effect\\laser.png");
	CResourceManager::Load<CTexture>(	"LightningBolt",	"Effect\\lightning_bolt.png");
	CResourceManager::Load<CModel>(		"Slash",			"Effect\\slash.obj");
	
	CResourceManager::Load<CModel>(		"Sword",			"Weapon\\Sword\\sword.obj");
	CResourceManager::Load<CModel>(		"Shield",			"Weapon\\Shield\\shield.obj");

	CResourceManager::Load<CSound>(		"ExplosionSound",	"Sound\\SE\\Explosion.wav");
	CResourceManager::Load<CSound>(		"FishfinderSound",	"Sound\\SE\\fishfinder.wav");

	CResourceManager::Load<CModelX>(	"RainbowTrout",		"Character\\Enemy\\RainbowTrout\\RainbowTrout_x5.x");
	CResourceManager::Load<CModelX>(	"StripedBass",		"Character\\Enemy\\StripedBass\\StripedBass_x0.5.x");
	CResourceManager::Load<CModelX>(	"Tuna",				"Character\\Enemy\\Tuna\\Tuna.x");

	// ゲームBGMを読み込み
	CBGMManager::Instance()->Play(EBGMType::eGame);

	CField* field1 = new CField();
	//CField* field2 = new CField();
	//field2->Position(0.0f, -425.0f, 0.0f);

	// ランダム初期化（Load() の最初で一度だけ呼ぶ）
	srand(static_cast<unsigned int>(time(nullptr)));

	// 魚の出現数は100匹が限界そう
	// RainbowTroutをランダムにN体生成
	for (int i = 0; i < 20; ++i)
	{
		float x = -500.0f + static_cast<float>(rand()) / RAND_MAX * 1000.0f; // -500〜500
		float y = -75.0f + static_cast<float>(rand()) / RAND_MAX * 50.0f;    // -75〜-25
		float z = -500.0f + static_cast<float>(rand()) / RAND_MAX * 1000.0f; // -500〜500
		float scale = 0.5f + static_cast<float>(rand()) / RAND_MAX * 1.5f;   // 0.5〜2.0

		CRainbowTrout* rainbowTrout = new CRainbowTrout();
		rainbowTrout->Position(x, y, z);
		rainbowTrout->Scale(scale, scale, scale);
	}

	// StripedBassをランダムにN体生成
	for (int i = 0; i < 15; ++i)
	{
		float x = -500.0f + static_cast<float>(rand()) / RAND_MAX * 1000.0f; // -500〜500
		float y = -125.0f + static_cast<float>(rand()) / RAND_MAX * 50.0f;    // -125〜-75
		float z = -500.0f + static_cast<float>(rand()) / RAND_MAX * 1000.0f; // -500〜500
		float scale = 0.5f + static_cast<float>(rand()) / RAND_MAX * 1.5f;   // 0.5〜2.0

		CStripedBass* stripedBass = new CStripedBass();
		stripedBass->Position(x, y, z);
		stripedBass->Scale(scale, scale, scale);
	}

	// TunaをランダムにN体生成
	for (int i = 0; i < 10; ++i)
	{
		float x = -500.0f + static_cast<float>(rand()) / RAND_MAX * 1000.0f; // -500〜500
		float y = -175.0f + static_cast<float>(rand()) / RAND_MAX * 50.0f;    // -175〜-125
		float z = -500.0f + static_cast<float>(rand()) / RAND_MAX * 1000.0f; // -500〜500
		float scale = 0.5f + static_cast<float>(rand()) / RAND_MAX * 1.5f;   // 0.5〜2.0

		CTuna* tuna = new CTuna();
		tuna->Position(x, y, z);
		tuna->Scale(scale, scale, scale);
	}

	// Playerを作成
	CPlayer* player = new CPlayer();
	player->Scale(1.0f, 1.0f, 1.0f);
	player->Position(0.0f, -0.5f, 0.0f);

	// CGameCameraのテスト
	//CGameCamera* mainCamera = new CGameCamera
	//(
	//	//CVector(5.0f, -15.0f, 180.0f),
	//	CVector(0.0f, 50.0f, 75.0f),
	//	player->Position()
	//);

	// CGameCamera2のテスト
	CVector atPos = player->Position() + CVector(0.0f, 5.0f, 0.0f);
	CGameCamera2* mainCamera = new CGameCamera2
	(
		atPos + CVector(0.0f, 0.0f, 40.0f),
		atPos
	);

	mainCamera->SetFollowTargetTf(player);

	// Playerにカメラのポインターを渡す
	player->SetCamera(mainCamera);

	// ゲームメニューを作成
	mpGameMenu = new CGameMenu();

	// ミニマップを生成
	new CMinimap();

	// UI作成
	new CGameSceneUI();
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

}

