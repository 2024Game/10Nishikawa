#include "CGameScene.h"
#include "CSceneManager.h"
#include "CGameSceneUI.h"
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
	, mState(EState::ebattlereserve)
	, mpGameMenu(nullptr)
	, mpPlayer(nullptr)
	, mTimeCount(0)
	, mElapsedTime(0.0f)
	, mStateStep(0)
	, mPlayerWin(false)
	, mEnemyLv(0)
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
	CResourceManager::Load<CModel>(		"Arena",			"Field\\Arena\\arena.obj");
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

	// ゲームBGMを読み込み
	CBGMManager::Instance()->Play(EBGMType::eGame);

	// ランダム初期化（Load() の最初で一度だけ呼ぶ）
	srand(static_cast<unsigned int>(time(nullptr)));

	mpSaveManager = &CSaveManager::Instance();
	// セーブファイルがあればロード、なければ初期値のまま
	if (!mpSaveManager->Load())
	{
		mpSaveManager->Reset();
	}

	CField* arena = new CField();
	arena->Scale(1.1f, 1.0f, 1.1f);

	// Playerを作成
	mpPlayer = new CPlayer(mpSaveManager);
	mpPlayer->Rotation(CVector(0.0f, 180.0f, 0.0f));
	mpPlayer->Scale(1.0f, 1.0f, 1.0f);
	mpPlayer->Position(0.0f, 5.0f, 100.0f);

	// サボテンの敵を作成
	//CCactus* cactus = new CCactus();
	//cactus->Scale(1.5f, 1.5f, 1.5f);
	//cactus->Position(0.0f, 20.0f, -100.0f);
	
	//mEnemyLv = 2 + (mpSaveManager->data.day / 3);
	mEnemyLv = ((int)mpSaveManager->data.day / 3)+ 2;

	if (mpSaveManager->data.selectDiff == 1)
	{
		mEnemyLv--;
	}
	else if (mpSaveManager->data.selectDiff == 3)
	{
		if (mEnemyLv == 5) return;
		mEnemyLv++;
	}

	// 兵士の敵を1体生成
	mpEnemy = new CSoldier(mpPlayer, mEnemyLv);
	mpEnemy->Position(0.0f, 5.0f, -100.0f);

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

	// UI作成
	//new CGameSceneUI();
	AddTask(new CGameSceneUI(mpPlayer));
}

//シーンの更新処理
void CGameScene::Update()
{
	// 状態に合わせて、更新処理を切り替える
	switch (mState)
	{
		// 戦闘準備状態
	case EState::ebattlereserve:	UpdateBattleReserve();		break;
		// 待機状態
	case EState::ebattle:			UpdateBattle();				break;
		// 戦闘結果状態
	case EState::ebattleresult:		UpdateBattleResult();		break;
	}

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
#ifdef _DEBUG
	CDebugPrint::Print("ElapsedTime:%f\n", mElapsedTime);
	CDebugPrint::Print("EnemyLv:%d\n", mEnemyLv);
	CDebugPrint::Print("selectDiff:%d\n", mpSaveManager->data.selectDiff);
#endif // _DEBUG
}

void CGameScene::UpdateBattleReserve()
{
	switch (mStateStep)
	{
	case 0:
		mStateStep++;
		break;

	case 1:
		// 戦闘準備時の待機時間待ち
		if (mElapsedTime < 5.0f)
		{
			mElapsedTime += Times::DeltaTime();
		}
		// 待機時間が経過した
		else
		{
			mStateStep++;
		}
		break;

	case 2:
		mpPlayer->SetInBattle(0);
		mpEnemy->SetInBattle(0);
		// 戦闘状態へ移行
		ChangeState(EState::ebattle);
		mStateStep++;
		break;
	}
}

void CGameScene::UpdateBattle()
{
	/*
	mTimeCount++;
	if (mTimeCount >= (60 * 120))
	{
		float x = -450.0f + static_cast<float>(rand()) / RAND_MAX * 900.0f; // -450〜450
		float y = 10.0f;
		float z = -450.0f + static_cast<float>(rand()) / RAND_MAX * 900.0f; // -450〜450

		CSoldier* soldier1 = new CSoldier(mpPlayer);
		soldier1->Position(x, y, z);
		mTimeCount = 0;
	}
	*/
	if (mpPlayer->GetHp() <= 0.0f || mpEnemy->GetHp() <= 0.0f)
	{
		ChangeState(EState::ebattleresult);
	}
}

void CGameScene::UpdateBattleResult()
{
	switch (mStateStep)
	{
	case 0:
		mStateStep++;
		break;

	case 1:
		// 戦闘準備時の待機時間待ち
		if (mElapsedTime < 3.0f)
		{
			mElapsedTime += Times::DeltaTime();
		}
		// 待機時間が経過した
		else
		{
			mStateStep++;
		}
		break;

	case 2:
		if (mpPlayer->GetHp() <= 0.0f)
		{
			mPlayerWin = false;
			mpEnemy->SetInBattle(2);
			mStateStep++;
		}
		else if (mpEnemy->GetHp() <= 0.0f)
		{
			mPlayerWin = true;
			mpPlayer->SetInBattle(2);
			mStateStep++;
		}
		break;

	case 3:
		// 待機時間待ち
		if (mElapsedTime < 10.0f)
		{
			mElapsedTime += Times::DeltaTime();
		}
		// 待機時間が経過した
		else
		{
			if (mPlayerWin)
			{
				mpSaveManager->data.hp = mpPlayer->GetHp();

				// HPを最大HPのhpRegeneLv(%)分、回復させる
				float hpCapa = mpSaveManager->data.maxHp - mpSaveManager->data.hp;
				float recovery = mpSaveManager->data.maxHp * (mpSaveManager->data.hpRegeneLv * 0.01f);
				if (hpCapa >= recovery)
				{
					mpSaveManager->data.hp += recovery;
				}
				else
				{
					mpSaveManager->data.hp = mpSaveManager->data.maxHp;
				}
				mpSaveManager->data.day++;
				switch (mEnemyLv)
				{
				case 1:
					mpSaveManager->data.money += 50;
					break;
				case 2:
					mpSaveManager->data.money += 100;
					break;
				case 3:
					mpSaveManager->data.money += 150;
					break;
				case 4:
					mpSaveManager->data.money += 250;
					break;
				case 5:
					mpSaveManager->data.money += 400;
					break;
				default:
					break;
				}
				mpSaveManager->Save();
				CSceneManager::Instance()->LoadScene(EScene::eHome);
			}
			else
			{
				mpSaveManager->Reset();
				CSceneManager::Instance()->LoadScene(EScene::eTitle);
			}
			mStateStep++;
		}
		break;
	case 4:
		
		break;
	}
}

void CGameScene::ChangeState(EState state)
{
	if (mState == state) return;

	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}
