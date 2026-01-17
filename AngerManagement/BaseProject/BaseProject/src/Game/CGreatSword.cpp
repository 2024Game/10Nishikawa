#include "CGreatSword.h"
#include "CColliderCapsule.h"

#define SWORD_LENGTH 80.0f
#define SWORD_RADIUS 0.75f

// コンストラクタ
CGreatSword::CGreatSword(CObjectBase* owner, ETag tag,
	Tags hitTags, Layers hitLayers)
	: CWeapon(tag)
	, mpModel(nullptr)
{
	mpOwner = owner;

	// モデルデータ取得
	mpModel = CResourceManager::Get<CModel>("GreatSword");

	// 剣のコライダーを作成
	mpCollider = new CColliderCapsule
	(
		this, ELayer::eAttackCol,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(SWORD_LENGTH - SWORD_RADIUS, 0.0f, 0.0f),
		SWORD_RADIUS
	);
	// タグとレイヤーの衝突設定
	mpCollider->SetCollisionTags(hitTags);
	mpCollider->SetCollisionLayers(hitLayers);
}

// デストラクタ
CGreatSword::~CGreatSword()
{
}

// 描画
void CGreatSword::Render()
{
	mpModel->Render(Matrix());
}
