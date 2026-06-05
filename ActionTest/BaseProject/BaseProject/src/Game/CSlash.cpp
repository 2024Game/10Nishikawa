#include "CSlash.h"
#include "CColliderSphere.h"

// コンストラクタ
CSlash::CSlash(CObjectBase* owner, const CVector& pos, const CVector& dir,
	float speed, float dist, ETag tag, Tags hitTags, Layers hitLayers)
	: CObjectBase(tag, ETaskPriority::eEffect, 0, ETaskPauseType::eGame)
	, mpOwner(owner)
	, mKillMoveDist(dist)
	, mMovedDist(0.0f)

{
	Position(pos);
	mMoveSpeed = dir.Normalized() * speed;
	Rotation(CQuaternion::LookRotation(mMoveSpeed, CVector::up));

	mpModel = CResourceManager::Get<CModel>("Slash");
	mpModel->SetupEffectSettings();

	// エフェクトのコライダーを作成
	mpCollider = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		10.0f, true, 1.0f
	);
	// タグとレイヤーの衝突設定
	mpCollider->SetCollisionTags(hitTags);
	mpCollider->SetCollisionLayers(hitLayers);
}

// デストラクタ
CSlash::~CSlash()
{
	// コライダーを削除
	SAFE_DELETE(mpCollider);
}

// 更新
void CSlash::Update()
{
	CVector move = mMoveSpeed * Times::DeltaTime();
	float dist = move.Length();
	if (mMovedDist + dist >= mKillMoveDist)
	{
		dist = mKillMoveDist - mMovedDist;
		move = move.Normalized() * dist;
	}
	Position(Position() + move);

	mMovedDist += dist;
	if (mMovedDist >= mKillMoveDist)
	{
		Kill();
	}
}

// 描画
void CSlash::Render()
{
	mpModel->SetColor(mColor);
	mpModel->Render(Matrix());
}

void CSlash::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (mpOwner != nullptr)
	{
		mpOwner->Collision(self, other, hit);
	}
}

CCollider* CSlash::Collider() const
{
	return mpCollider;
}
