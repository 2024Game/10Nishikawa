#include "CBarrel.h"
#include "CColliderCapsule.h"
#include "CColliderSphere.h"
#include "CEnemy.h"

#include "CFish.h"
#include "CFishManager.h"

#define BODY_HEIGHT 3.0f			// 本体のコライダーの高さ
#define BODY_RADIUS 1.5f			// 本体のコライダーの幅
#define ATTACK_COL_RADIUS 25.0f		// 爆発半径
#define ATTACK_COL_POS CVector(0.0f, 0.5f, 0.0f)
#define P_POS CVector(0.0f, -0.5f, 0.0f)

// コンストラクタ
CBarrel::CBarrel(float speed, float depth, float damage, float trackspeed, float radius, CPlayer* player , CGameCamera2* camera)
	: CObjectBase(ETag::ePlayer, ETaskPriority::eWeapon, 0, ETaskPauseType::eGame)
	, mpModel(nullptr)
	, mpModel2(nullptr)
	, mpCollider(nullptr)
	, mpAttackCol(nullptr)
	, mpExplosionSound(nullptr)
	, mMoveSpeed(speed)
	, mTrackSpeed(trackspeed)
	, mFireDepth(depth)
	, mpPlayer(player)
	, mpCamera(camera)
	, mFireDamage(damage)
	, mRadius(radius)
{
	// モデルデータ取得
	mpModel = CResourceManager::Get<CModel>("Barrel");
	mpModel2 = CResourceManager::Get<CModel>("Propeller");

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
		mRadius, true
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

	const std::vector<CFish*> fishes = CFishManager::Instance()->GetFishes();
	mTargetPos = CVector(Position().X(), -mFireDepth, Position().Z());

	// ここでfishesの中からmTargetPosに一番近い魚にmTrackSpeedで近づかせたい
	// 追尾処理（3Dで一番近い敵に近づく）
	CFish* nearestFish = nullptr;
	float minDistSq = FLT_MAX;

	for (CFish* fish : fishes)
	{
		// ターゲット位置に対する距離を計算
		CVector diff = fish->Position() - mTargetPos;
		float distSq = diff.X() * diff.X() + diff.Y() * diff.Y() + diff.Z() * diff.Z();

		if (distSq < minDistSq)
		{
			minDistSq = distSq;
			nearestFish = fish;
		}
	}

	if (nearestFish && !mAttackTriggered)
	{
		// ターゲットのXZ座標
		CVector targetXZ(nearestFish->Position().X(), Position().Y(), nearestFish->Position().Z());
		CVector dir = targetXZ - Position();

		// XZ平面距離
		float distXZ = sqrt(dir.X() * dir.X() + dir.Z() * dir.Z());

		// 距離が3以上なら追尾
		if (distXZ > 3.0f)
		{
			// dir正規化（XZのみ）
			CVector dirNorm(dir.X() / distXZ, 0.0f, dir.Z() / distXZ);

			// 予定移動量
			float moveAmount = mTrackSpeed * Times::DeltaTime();

			// 移動量が距離を超えないように制限
			if (moveAmount > distXZ)
				moveAmount = distXZ;

			// 移動
			Position(Position() + dirNorm * moveAmount);
		}
	}

	// 発火深度に達したら、攻撃コライダーを有効にしてタイマー開始
	if (!mAttackTriggered && Position().Y() <= -mFireDepth)
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
			mpPlayer->mInAttack = false;

			Kill();
		}
	}
}

// 描画
void CBarrel::Render()
{
	mpModel->SetColor(mColor);
	mpModel->Render(Matrix());

	mpModel2->SetColor(mColor);
	mpModel2->Render(Matrix() * Matrix::CreateTranslation(P_POS));
}