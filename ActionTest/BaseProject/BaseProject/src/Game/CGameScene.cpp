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
#include "CEnemyManager.h"
#include "CCactus.h"
#include "CSoldier.h"
#include "CHeavyWarrior.h"
#include "CLowHealP.h"
#include <Maths.h>

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
	, mpKanseiSE(nullptr)
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

	CResourceManager::Load<CModelX>(	"Player",			"Character\\TestPlayer\\player.x");
	CResourceManager::Load<CModelX>(	"Cactus",			"Character\\Enemy\\Cactus\\cactus.x");
	CResourceManager::Load<CModelX>(	"Soldier",			"Character\\TestPlayer\\player.x");

	CResourceManager::Load<CModel>(		"CactusNeedle",		"Character\\Enemy\\Cactus\\needle.obj");
	CResourceManager::Load<CTexture>(	"Laser",			"Effect\\laser.png");
	CResourceManager::Load<CTexture>(	"LightningBolt",	"Effect\\lightning_bolt.png");
	CResourceManager::Load<CModel>(		"Slash",			"Effect\\slash.obj");
	CResourceManager::Load<CSound>(		"SlashSound",		"Sound\\SE\\slash.wav");
	CResourceManager::Load<CSound>(		"KanseiSound",		"Sound\\SE\\kanseiSE.wav");

	CResourceManager::Load<CModel>(		"Sword",			"Weapon\\Sword\\sword.obj");
	CResourceManager::Load<CModel>(		"GreatSword",		"Weapon\\GreatSword\\GreatSword.obj");
	CResourceManager::Load<CModel>(		"BusterSword",		"Weapon\\BusterSword\\DRAGON_SLAYER.obj");

	CResourceManager::Load<CModel>(		"LowHealP",			"Items\\LowHealPotion\\lowhealpotion.obj");

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

	mpKanseiSE = CResourceManager::Get<CSound>("KanseiSound");

	CField* arena = new CField();
	arena->Scale(1.1f, 1.0f, 1.1f);

	// Playerを作成
	mpPlayer = new CPlayer(mpSaveManager);
	mpPlayer->Rotation(CVector(0.0f, 180.0f, 0.0f));
	mpPlayer->Scale(1.0f, 1.0f, 1.0f);
	mpPlayer->Position(0.0f, 5.0f, 100.0f);

	CLowHealP* lowHpPotion = new CLowHealP();
	lowHpPotion->Position(0.0f, 150.0f, 5.0f);
	//lowHpPotion->Scale(10.0f, 10.0f, 10.0f);

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
		if (mEnemyLv == 10) return;
		mEnemyLv++;
	}

	CEnemy* enemy;
	int enemyCount = 0;
	int rand = 0;
	
	switch (mpSaveManager->data.selectDiff)
	{
	case 1:
		mEnemyLv--;
		enemyCount = 1;
		break;
	case 2:
		enemyCount = 1;
		break;
	case 3:
		rand = Math::Rand(0, 5);
		if (rand == 5)
		{
			mEnemyLv--;
			enemyCount = 3;
		}
		else if (rand > 2)
		{
			enemyCount = 2;
		}
		else
		{
			mEnemyLv++;
			enemyCount = 1;
		}
		break;
	default:
		break;
	}
	
	// 兵士の敵を1体生成
	for (int i = 0; i < enemyCount; i++)
	{
		// ランダムで敵の種類を選ぶ
		rand = Math::Rand(0, 99);
		if (rand < 49)
		{
			enemy = new CSoldier(mpPlayer, mEnemyLv);
		}
		else
		{
			enemy = new CHeavyWarrior(mpPlayer, mEnemyLv);
		}

		switch (i)
		{
		case 0:
			enemy->Position(0.0f, 5.0f, -100.0f);
			break;
		case 1:
			enemy->Position(50.0f, 5.0f, -125.0f);
			break;
		case 2:
			enemy->Position(-50.0f, 5.0f, -125.0f);
			break;
		}
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
		// 歓声SEを再生
		mpKanseiSE->Play(0.25f);
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
		CEnemyManager::Instance()->SetInBattle(0);
		
		// ゲームBGMを読み込み
		//CBGMManager::Instance()->Play(EBGMType::eGame);
		// 戦闘状態へ移行
		ChangeState(EState::ebattle);
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
	
	if (mpPlayer->GetHp() <= 0.0f || !CEnemyManager::Instance()->Surviv())
	{
		Times::SetTimeScale(0.25f);
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
		// 戦闘結果時の待機時間待ち
		if (mElapsedTime < 0.75f)
		{
			mElapsedTime += Times::DeltaTime();
		}
		// 待機時間が経過した
		else
		{
			mElapsedTime = 0.0f;
			mStateStep++;
		}
		break;

	case 2:
		Times::SetTimeScale(1.0f);
		if (mpPlayer->GetHp() <= 0.0f)
		{
			mPlayerWin = false;
			CEnemyManager::Instance()->SetInBattle(2);
			// 歓声SEを再生
			mpKanseiSE->Play(0.25f);
			mStateStep++;
		}
		else if (!CEnemyManager::Instance()->Surviv())
		{
			mPlayerWin = true;
			mpPlayer->SetInBattle(2);
			// 歓声SEを再生
			mpKanseiSE->Play(0.25f);
			mStateStep++;
		}
		break;

	case 3:
		
		// 待機時間待ち
		if (mElapsedTime < 6.0f)
		{
			mElapsedTime += Times::DeltaTime();
		}
		// 待機時間が経過した
		else
		{
			if (mPlayerWin)
			{
				mpSaveManager->data.hp = mpPlayer->GetHp();

				// HPを5+最大HPのhpRegeneLv(%)分、回復させる
				float hpCapa = mpSaveManager->data.maxHp - mpSaveManager->data.hp;
				float recovery = (mpSaveManager->data.maxHp * (mpSaveManager->data.hpRegeneLv * 0.01f) + 5);
				if (hpCapa >= recovery)
				{
					mpSaveManager->data.hp += recovery;
				}
				else
				{
					mpSaveManager->data.hp = mpSaveManager->data.maxHp;
				}
				mpSaveManager->data.day++;

				switch (mpSaveManager->data.selectDiff)
				{
				case 1:
					mpSaveManager->data.money += 125 + 50;
					break;
				case 2:
					mpSaveManager->data.money += 150 + 75;
					break;
				case 3:
					mpSaveManager->data.money += 175 + 175;
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
