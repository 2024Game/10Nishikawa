#include "CBarrel.h"
#include "CColliderCapsule.h"
#include "CColliderSphere.h"
#include "CEnemy.h"

#define BODY_HEIGHT 3.0f			// 本体のコライダーの高さ
#define BODY_RADIUS 1.5f			// 本体のコライダーの幅
#define ATTACK_COL_RADIUS 25.0f		// 爆発半径
#define ATTACK_COL_POS CVector(0.0f, 0.5f, 0.0f)

// コンストラクタ
CBarrel::CBarrel(float speed, float dist, float damage, CPlayer* player , CGameCamera2* camera)
	: CObjectBase(ETag::ePlayer, ETaskPriority::eWeapon, 0, ETaskPauseType::eGame)
	, mpModel(nullptr)
	, mpCollider(nullptr)
	, mpAttackCol(nullptr)
	, mpExplosionSound(nullptr)
	, mMoveSpeed(speed)
	, mMoveDist(dist)
	, mCurrDist(0.0f)
	, mpPlayer(player)
	, mpCamera(camera)
	, mFireDamage(damage)
{
	// 針を黄色にする
	mColor = CColor::white;

	// モデルデータ取得
	mpModel = CResourceManager::Get<CModel>("Barrel");

	// 本体のコライダーを作成
	mpCollider = new CColliderCapsule
	(
		this, ELayer::ePlayer,
		CVector(0.0f, BODY_RADIUS, BODY_RADIUS - 0.0f),
		CVector(0.0f, BODY_RADIUS, BODY_HEIGHT - BODY_RADIUS - 0.0f),
		BODY_RADIUS
	);
	// 魚と衝突するように設定
	mpCollider->SetCollisionTags({ ETag::eEnemy});
	mpCollider->SetCollisionLayers({ ELayer::eEnemy, ELayer::eField });

	// 攻撃コライダーを作成
	mpAttackCol = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		ATTACK_COL_RADIUS, true
	);
	// 魚のコライダーとのみヒットするように設定
	mpAttackCol->SetCollisionTags({ ETag::eEnemy });
	mpAttackCol->SetCollisionLayers({ ELayer::eEnemy });
	// 攻撃コライダーの座標を設定
	mpAttackCol->Position(ATTACK_COL_POS);
	// 攻撃コライダーを最初はオフにしておく
	mpAttackCol->SetEnable(false);

	// SEデータ取得
	mpExplosionSound = CResourceManager::Get<CSound>("ExplosionSound");
}

// デストラクタ
CBarrel::~CBarrel()
{
	// コライダー削除
	SAFE_DELETE(mpCollider);
	SAFE_DELETE(mpAttackCol);
}

// 衝突処理
void CBarrel::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 魚に衝突した
	if (other->Layer() == ELayer::eEnemy)
	{
		// 魚にダメージを与える
		CEnemy* fish = dynamic_cast<CEnemy*>(other->Owner());
		if (fish != nullptr)
		{
			fish->TakeDamage(mFireDamage, this);
		}
	}
}

// 更新
void CBarrel::Update()
{
	// このフレームで移動距離を求める
	float moveDist = mMoveSpeed * Times::DeltaTime();

	// 移動距離分、移動させる
	CVector pos = Position();
	pos += VectorZ() * moveDist;
	Position(pos);

	// 移動した距離を加算
	mCurrDist += moveDist;

	// 移動出来る距離を超えたら、攻撃コライダーを有効にしてタイマー開始
	if (!mAttackTriggered && mCurrDist >= mMoveDist)
	{
		mpExplosionSound->Play();

		// 攻撃コライダーをオンにする
		mpAttackCol->SetEnable(true);

		mMoveSpeed = 0.0f;

		// タイマー開始
		mAttackTriggered = true;
		mKillTimer = 0.0f;
	}

	// 攻撃コライダーを有効にしてから2秒経過したらKill
	if (mAttackTriggered)
	{
		mKillTimer += Times::DeltaTime();
		if (mKillTimer >= 2.0f)
		{
			// カメラの追従をプレイヤーに返す
			mpCamera->SetFollowTargetTf(mpPlayer);
			mpCamera->SetFollowTargetOffset(CVector(0.0f, 5.0f, 0.0f));

			// Playerのステートを変更
			mpPlayer->SetState(1);

			Kill();
		}
	}
}

// 描画
void CBarrel::Render()
{
	mpModel->SetColor(mColor);
	mpModel->Render(Matrix());
}