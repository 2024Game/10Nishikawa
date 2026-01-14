#include "CLowHealP.h"
#include "CColliderCapsule.h"

#define ITEM_LENGTH 3.5f
#define ITEM_RADIUS 0.75f

// コンストラクタ
CLowHealP::CLowHealP()
	: mpModel(nullptr)
{
	// モデルデータ取得
	mpModel = CResourceManager::Get<CModel>("LowHealP");

	// 低級回復薬のコライダーを作成
	mpCollider = new CColliderCapsule
	(
		this, ELayer::eItem,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, ITEM_LENGTH - ITEM_RADIUS, 0.0f),
		ITEM_RADIUS
	);
	// タグとレイヤーの衝突設定
	mpCollider->SetCollisionTags(
		{ ETag::eField, ETag::eRideableObject,
		ETag::ePlayer, ETag::eEnemy });

	mpCollider->SetCollisionLayers(
		{ ELayer::eField, ELayer::ePlayer, ELayer::eEnemy });
}

// デストラクタ
CLowHealP::~CLowHealP()
{
}

// 描画
void CLowHealP::Render()
{
	mpModel->Render(Matrix());
}