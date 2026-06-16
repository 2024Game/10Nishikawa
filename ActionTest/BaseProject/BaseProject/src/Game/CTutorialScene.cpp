#include "CTutorialScene.h"
#include <Maths.h>

#include "CSceneManager.h"
#include "CGameSceneUI.h"
#include "CField.h"
#include "CGameCamera.h"
#include "CGameCamera2.h"
#include "CInput.h"
#include "CGameMenu.h"
#include "CBGMManager.h"
#include "CLineEffect.h"

#include "CEnemyManager.h"
#include "CPlayer.h"
#include "CTutoSoldier.h"

#include "CEnemyStatusLoader.h"


//コンストラクタ
CTutorialScene::CTutorialScene()
	: CSceneBase(EScene::eTutorial)
	, mState(EState::ephase1)
	, mpGameMenu(nullptr)
	, mpPlayer(nullptr)
	, mTimeCount(0)
	, mElapsedTime(0.0f)
	, mStateStep(0)
	, mpKanseiSE(nullptr)
	, mEnemyLv(0)
{

}

//デストラクタ
CTutorialScene::~CTutorialScene()
{
}

//シーン読み込み
void CTutorialScene::Load()
{
	// ゲーム画面はカーソル非表示
	CInput::ShowCursor(false);
	// 背景色設定
	System::SetClearColor(0.1921569f, 0.3019608f, 0.4745098f, 1.0f);

	//ここでゲーム中に必要な
	//リソースの読み込みやクラスの生成を行う

	// 敵ステータスCSVの読み込み
	CEnemyStatusLoader::Load("EnemyStatus\\Soldier.csv");
	CEnemyStatusLoader::Load("EnemyStatus\\HeavyWarrior.csv");

	CResourceManager::Load<CModel>("Field", "Field\\field.obj");
	CResourceManager::Load<CModel>("Arena", "Field\\Arena\\arena.obj");
	CResourceManager::Load<CModel>("FieldCube", "Field\\Object\\cube.obj");
	CResourceManager::Load<CModel>("FieldCylinder", "Field\\Object\\cylinder.obj");

	CResourceManager::Load<CModelX>("Player", "Character\\TestPlayer\\player.x");
	CResourceManager::Load<CModelX>("Soldier", "Character\\TestPlayer\\player.x");

	CResourceManager::Load<CModel>("Slash", "Effect\\slash.obj");
	CResourceManager::Load<CSound>("SlashSound", "Sound\\SE\\slash.wav");
	CResourceManager::Load<CSound>("KanseiSound", "Sound\\SE\\kanseiSE.wav");

	CResourceManager::Load<CModel>("Sword", "Weapon\\Sword\\sword.obj");
	CResourceManager::Load<CModel>("GreatSword", "Weapon\\GreatSword\\GreatSword.obj");
	CResourceManager::Load<CModel>("BusterSword", "Weapon\\BusterSword\\DRAGON_SLAYER.obj");

	CResourceManager::Load<CModel>("LowHealP", "Items\\LowHealPotion\\lowhealpotion.obj");

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

	mpPlayer->TutorialInit();

	// Enemyを作成
	mEnemyLv = 1;

	CEnemy* enemy;
	int rand = 0;

	// 兵士の敵を1体生成
	enemy = new CTutoSoldier(mpPlayer, mEnemyLv);
	enemy->Position(0.0f, 5.0f, -50.0f);

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
void CTutorialScene::Update()
{
	// 状態に合わせて、更新処理を切り替える
	switch (mState)
	{
		// 戦闘準備状態
	case EState::ephase1:		UpdateTuto1();		break;
		// 待機状態
	case EState::ephase2:		UpdateTuto2();		break;
		// 戦闘結果状態
	case EState::ephase3:		UpdateTuto3();		break;
	}

#ifdef _DEBUG
	CDebugPrint::Print("ElapsedTime:%f\n", mElapsedTime);
	CDebugPrint::Print("EnemyLv:%d\n", mEnemyLv);
	CDebugPrint::Print("selectDiff:%d\n", mpSaveManager->data.selectDiff);
	CDebugPrint::Print("mAttHitCount:%d\n", mpPlayer->mAttHitCount);
	CDebugPrint::Print("mJustAvoidCount:%d\n", mpPlayer->mJustAvoidCount);
	CDebugPrint::Print("mKickHitCount:%d\n", mpPlayer->mKickHitCount);
#endif // _DEBUG
}

void CTutorialScene::UpdateTuto1()
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
		mpPlayer->SetInBattle(0);
		CEnemyManager::Instance()->SetInBattle(4);
		mStateStep++;
		break;
	case 3:
		// プレイヤーが敵に攻撃を５回当てたら次のフェーズへ
		if (mpPlayer->mAttHitCount >= 5)
		{
			CEnemyManager::Instance()->SetInBattle(5);

			// フェーズ2 : 一定間隔で​攻撃を​する​がダメージ無し、無敵の敵
			ChangeState(EState::ephase2);
		}
		break;
	}
}

void CTutorialScene::UpdateTuto2()
{
	switch (mStateStep)
	{
	case 0:
		// ジャスト回避のチュートリアル
		if (mpPlayer->mJustAvoidCount >= 1)
		{
			// 次のキックのカウントを0にしておく
			mpPlayer->mKickHitCount = 0;
			mStateStep++;
		}
		break;

	case 1:
		// キックのチュートリアル
		if (mpPlayer->mKickHitCount >= 1)
		{
			// 次のジャストキックのカウントを0にしておく
			mpPlayer->mJustKickHitCount = 0;
			mStateStep++;
		}
		break;

	case 2:
		// ジャストキックのチュートリアル
		if (mpPlayer->mJustKickHitCount >= 1)
		{
			mElapsedTime = 0.0f;
			mStateStep++;
		}
		break;

	case 3:
		// n秒経ったら次のチュートリアルに
		if (mElapsedTime < 15.0f)
		{
			mElapsedTime += Times::DeltaTime();
		}
		// 待機時間が経過した
		else
		{
			// フェーズ2 : 一定間隔で​攻撃を​する​がダメージ無し、無敵の敵
			ChangeState(EState::ephase3);

			mElapsedTime = 0.0f;
		}
		break;
	}
}

void CTutorialScene::UpdateTuto3()
{
	switch (mStateStep)
	{
	case 0:
		// 敵を動ける状態に移行
		CEnemyManager::Instance()->SetInBattle(0);

		mStateStep++;
		break;

	case 1:
		// 決着の瞬間をスローモーションに
		if (mpPlayer->GetHp() <= 0.0f || !CEnemyManager::Instance()->Surviv())
		{
			Times::SetTimeScale(0.25f);
			mStateStep++;
		}
		break;

	case 2:
		if (mpPlayer->GetHp() <= 0.0f)
		{
			CEnemyManager::Instance()->SetInBattle(2);
			// 歓声SEを再生
			mpKanseiSE->Play(0.25f);
			mStateStep++;
		}
		else if (!CEnemyManager::Instance()->Surviv())
		{
			mpPlayer->SetInBattle(2);
			// 歓声SEを再生
			mpKanseiSE->Play(0.25f);
			mElapsedTime = 0.0f;
			mStateStep++;
		}
		break;

	case 3:
		// 戦闘結果時の待機時間待ち
		if (mElapsedTime < 0.75f)
		{
			mElapsedTime += Times::DeltaTime();
		}
		// 待機時間が経過した
		else
		{
			Times::SetTimeScale(1.0f);
			mElapsedTime = 0.0f;
			mStateStep++;
		}
		break;

	case 4:
		// 待機時間待ち
		if (mElapsedTime < 4.0f)
		{
			mElapsedTime += Times::DeltaTime();
		}
		// 待機時間が経過したら兵舎に戻る
		else
		{
			CSceneManager::Instance()->LoadScene(EScene::eHome);
		}
		break;
	}
}

void CTutorialScene::ChangeState(EState state)
{
	if (mState == state) return;

	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}