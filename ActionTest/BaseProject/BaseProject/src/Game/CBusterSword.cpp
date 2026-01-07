#include "CBusterSword.h"
#include "CColliderCapsule.h"

#define SWORD_LENGTH 165.0f
#define SWORD_RADIUS 1.75f

// コンストラクタ
CBusterSword::CBusterSword(CObjectBase* owner, ETag tag,
	Tags hitTags, Layers hitLayers)
	: CWeapon(tag)
	, mpModel(nullptr)
{
	mpOwner = owner;

	// モデルデータ取得
	mpModel = CResourceManager::Get<CModel>("BusterSword");

	// 剣のコライダーを作成
	mpCollider = new CColliderCapsule
	(
		this, ELayer::eAttackCol,
		CVector(30.0f, 0.0f, 0.0f),
		CVector(SWORD_LENGTH - SWORD_RADIUS, 0.0f, 0.0f),
		SWORD_RADIUS
	);
	// タグとレイヤーの衝突設定
	mpCollider->SetCollisionTags(hitTags);
	mpCollider->SetCollisionLayers(hitLayers);
}

// デストラクタ
CBusterSword::~CBusterSword()
{
}

// 描画
void CBusterSword::Render()
{
	mpModel->Render(Matrix());
}