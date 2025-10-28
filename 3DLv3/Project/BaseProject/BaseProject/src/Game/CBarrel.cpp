#include "CBarrel.h"
#include "CColliderSphere.h"
#include "CCharaBase.h"

// コンストラクタ
CBarrel::CBarrel(float speed, float dist, CPlayer* player , CGameCamera2* camera)
	: CObjectBase(ETag::eEnemy, ETaskPriority::eWeapon, 0, ETaskPauseType::eGame)
	, mpModel(nullptr)
	, mpCollider(nullptr)
	, mMoveSpeed(speed)
	, mMoveDist(dist)
	, mCurrDist(0.0f)
	, mpPlayer(player)
	, mpCamera(camera)
{
	// 針を黄色にする
	mColor = CColor::white;

	// モデルデータ取得
	mpModel = CResourceManager::Get<CModel>("Barrel");

	// コライダーを作成
	mpCollider = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		3.0f
	);
	// プレイヤーとフィールドと衝突するように設定
	mpCollider->SetCollisionTags({ ETag::eEnemy/*, ETag::eField*/ });
	mpCollider->SetCollisionLayers({ ELayer::eEnemy, ELayer::eField });
}

// デストラクタ
CBarrel::~CBarrel()
{
	// コライダー削除
	SAFE_DELETE(mpCollider);
}

// 衝突処理
void CBarrel::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// プレイヤーに衝突した
	if (other->Layer() == ELayer::ePlayer)
	{
		// プレイヤーにダメージを与える
		CCharaBase* chara = dynamic_cast<CCharaBase*>(other->Owner());
		if (chara != nullptr)
		{
			chara->TakeDamage(1, this);
		}
	}

	// 何かにぶつかったら、自身を削除
	Kill();
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
	// 移動出来る距離を超えたら、自身を削除
	if (mCurrDist >= mMoveDist)
	{
		// カメラの追従をプレイヤーに返す
		mpCamera->SetFollowTargetTf(mpPlayer);
		mpCamera->SetFollowTargetOffset(CVector(0.0f, 5.0f, 0.0f));

		// Playerのステートを変更
		mpPlayer->SetState(1);

		Kill();
	}
}

// 描画
void CBarrel::Render()
{
	mpModel->SetColor(mColor);
	mpModel->Render(Matrix());
}