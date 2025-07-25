#include "CXEnemy.h"

CXEnemy::CXEnemy()
	: mColSphereHead(this, nullptr, CVector(0.0f, 5.0f, -3.0f), 0.5f)
	, mColSphereBody(this, nullptr, CVector(), 0.5f)
	, mColSphereSword(this, nullptr, CVector(-10.0f, 10.0f, 50.0f), 0.3f)
{
}

void CXEnemy::Init(CModelX* model)
{
	CXCharacter::Init(model);
	//合成行列の設定
	//頭
	mColSphereHead.Matrix(&mpCombinedMatrix[11]);
	//体
	mColSphereBody.Matrix(&mpCombinedMatrix[8]);
	//剣
	mColSphereSword.Matrix(&mpCombinedMatrix[21]);
}

void CXEnemy::Collision(CCollider* m, CCollider* o)
{
	if (m->Type() == CCollider::EType::ESPHERE
		&& o->Type() == CCollider::EType::ESPHERE
		&& m->Tag() == CCollider::ETag::EBODY
		&& o->Tag() == CCollider::ETag::ESWORD)
	{
		if (m->Collision(m, o))
		{
			//30フレームかけてダウンし、繰り返さない
			ChangeAnimation(11, false, 30);
		}
	}
}
