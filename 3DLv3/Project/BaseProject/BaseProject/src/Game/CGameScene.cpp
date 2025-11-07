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
	CResourceManager::Load<CModel>(		"FieldCube",		"Field\\Object\\cube.obj");
	CResourceManager::Load<CModel>(		"FieldCylinder",	"Field\\Object\\cylinder.obj");
	CResourceManager::Load<CModel>(		"Player",			"Character\\PlayerBoat\\PlayerBoat.obj");
	CResourceManager::Load<CModel>(		"Barrel",			"Objects\\Barrel\\barrel.obj");
	CResourceManager::Load<CModelX>(	"Cactus",			"Character\\Enemy\\Cactus\\cactus.x");
	CResourceManager::Load<CModel>(		"CactusNeedle",		"Character\\Enemy\\Cactus\\needle.obj");
	CResourceManager::Load<CTexture>(	"Laser",			"Effect\\laser.png");
	CResourceManager::Load<CTexture>(	"LightningBolt",	"Effect\\lightning_bolt.png");
	CResourceManager::Load<CModel>(		"Slash",			"Effect\\slash.obj");
	CResourceManager::Load<CSound>(		"SlashSound",		"Sound\\SE\\slash.wav");
	CResourceManager::Load<CModel>(		"Sword",			"Weapon\\Sword\\sword.obj");
	CResourceManager::Load<CModel>(		"Shield",			"Weapon\\Shield\\shield.obj");

	CResourceManager::Load<CModelX>(	"RainbowTrout",		"Character\\Enemy\\RainbowTrout\\RainbowTrout_x5.x");

	// ゲームBGMを読み込み
	CBGMManager::Instance()->Play(EBGMType::eGame);

	CField* field1 = new CField();
	CField* field2 = new CField();
	field2->Position(0.0f, -425.0f, 0.0f);

	// サボテンの敵を作成
	CCactus* cactus = new CCactus();
	cactus->Position(0.0f, 0.0f, -100.0f);
	cactus->Scale(1.0f, 1.0f, 1.0f);

	// ランダム初期化（Load() の最初で一度だけ呼ぶ）
	srand(static_cast<unsigned int>(time(nullptr)));

	// RainbowTroutをランダムに25体生成
	for (int i = 0; i < 50; ++i)
	{
		float x = -500.0f + static_cast<float>(rand()) / RAND_MAX * 1000.0f; // -500〜500
		float y = -50.0f + static_cast<float>(rand()) / RAND_MAX * 35.0f;    // -50〜-15
		float z = -500.0f + static_cast<float>(rand()) / RAND_MAX * 1000.0f; // -500〜500
		float scale = 0.5f + static_cast<float>(rand()) / RAND_MAX * 1.5f;   // 0.5〜2.0

		CRainbowTrout* rainbowTrout = new CRainbowTrout();
		rainbowTrout->Position(x, y, z);
		rainbowTrout->Scale(scale, scale, scale);

		// 情報を保存
		FishInfo info;
		info.typeName = "RainbowTrout";
		info.scale = scale;
		info.position = CVector(x, y, z);
		fishInfoList.push_back(info);
		fishObjects.push_back(rainbowTrout);
	}

	/*
	for (size_t i = 0; i < fishInfoList.size(); ++i)
	{
		const FishInfo& info = fishInfoList[i];
		CDebugPrint::Print("Fish[%d] Type: %s, Scale: %.2f\n", static_cast<int>(i), info.typeName.c_str(), info.scale);
	}
	*/

	for (size_t i = 0; i < fishInfoList.size(); ++i)
	{
		const FishInfo& info = fishInfoList[i];
		std::cout << "Fish[" << i << "] Type: " << info.typeName << ", Scale: " << info.scale << std::endl;
	}

	// Playerを作成
	CPlayer* player = new CPlayer();
	player->Scale(1.0f, 1.0f, 1.0f);

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

	DrawMiniMap();
}

// ミニマップを描画する
void CGameScene::DrawMiniMap()
{
	// ウィンドウ解像度
	const float screenWidth = 1080.0f;
	const float screenHeight = 720.0f;

	// ミニマップ設定
	const float mapWorldRadius = 300.0f;
	const float mapPixelRadius = 100.0f;
	const float mapCenterX = 150.0f;
	const float mapCenterY = 150.0f; // 左下固定

	CPlayer* player = CPlayer::Instance();
	if (!player) return;
	CVector playerPos = player->Position();

	// --- 魚の位置更新 ---
	for (size_t i = 0; i < fishInfoList.size(); ++i)
	{
		if (i < fishObjects.size() && fishObjects[i])
		{
			fishInfoList[i].position = fishObjects[i]->Position();
		}
	}

	// --- 深度無効化して2Dモードへ ---
	glPushAttrib(GL_ALL_ATTRIB_BITS); // ← 現在のGL状態を全保存
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// --- 射影行列 ---
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, screenWidth, screenHeight, 0, -1, 1);

	// --- モデル行列 ---
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// --- 背景円 ---
	DrawFilledCircle(mapCenterX, mapCenterY, mapPixelRadius, 0.15f, 0.15f, 0.15f, 0.7f);

	// --- 魚を描く ---
	for (const auto& fish : fishInfoList)
	{
		float dx = fish.position.X() - playerPos.X();
		float dz = fish.position.Z() - playerPos.Z();
		float dist = sqrtf(dx * dx + dz * dz);
		if (dist > mapWorldRadius) continue;

		float scale = mapPixelRadius / mapWorldRadius;
		float mapX = dx * scale;
		float mapY = dz * scale;

		float drawX = mapCenterX + mapX;
		float drawY = mapCenterY - mapY;

		DrawFilledCircle(drawX, drawY, 3.0f, 1.0f, 0.3f, 0.3f, 1.0f);
	}

	// --- プレイヤー ---
	DrawFilledCircle(mapCenterX, mapCenterY, 5.0f, 0.2f, 1.0f, 0.2f, 1.0f);

	// --- 行列と状態を戻す ---
	glPopMatrix(); // modelview
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopAttrib(); // ← GL状態を全て復元
}

// 円や点を描く補助関数
void CGameScene::DrawFilledCircle(float cx, float cy, float radius, float r, float g, float b, float a)
{
	glColor4f(r, g, b, a);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(cx, cy); // 中心
	const int numSegments = 32;
	for (int i = 0; i <= numSegments; i++)
	{
		float theta = 2.0f * 3.1415926f * float(i) / float(numSegments);
		float x = radius * cosf(theta);
		float y = radius * sinf(theta);
		glVertex2f(cx + x, cy + y);
	}
	glEnd();
}
