#include "CField.h"
#include "CCollisionManager.h"
#include "CMoveFloor.h"
#include "CRotateFloor.h"
#include "CLineEffect.h"

CField* CField::mpInstance = nullptr;

CField::CField()
	: CObjectBase(ETag::eField, ETaskPriority::eBackground)
	, mEffectAnimData(1, 11, true, 11, 0.03f)
{
	mpInstance = this;

	mpModel = CResourceManager::Get<CModel>("Arena");

	CModel* floorCol = CResourceManager::Get<CModel>("ArenaFloorCol");
	mpFloorCol = new CColliderMesh(this, ELayer::eField, floorCol, true);

	CModel* wallCol = CResourceManager::Get<CModel>("ArenaWallCol");
	mpWallCol = new CColliderMesh(this, ELayer::eWall, wallCol, true);
	
	//CreateFieldObjects();
}

CField::~CField()
{
	if (mpInstance == this)
	{
		mpInstance = nullptr;
	}

	SAFE_DELETE(mpFloorCol);
	SAFE_DELETE(mpWallCol);
}

void CField::CreateFieldObjects()
{
	mpCubeModel = CResourceManager::Get<CModel>("FieldCube");
	mpCylinderModel = CResourceManager::Get<CModel>("FieldCylinder");

	new CMoveFloor
	(
		mpCubeModel,
		CVector(0.0f, 10.0f, -50.0f), CVector(1.0f, 1.0f, 1.0f),
		CVector(50.0f, 0.0f, 0.0f), 10.0f
	);
	new CRotateFloor
	(
		mpCylinderModel,
		CVector(-40.0f, 15.0f, 20.0f), CVector(1.0f, 1.0f, 1.0f),
		1.0f
	);

	// “®‚©‚È‚¢°‡@
	new CMoveFloor
	(
		mpCubeModel,
		CVector(20.0f, 10.0f, 0.0f), CVector(0.5f, 1.0f, 0.25f),
		CVector(0.0f, 0.0f, 0.0f), 5.0f
	);
	// “®‚­°‡@
	new CMoveFloor
	(
		mpCubeModel,
		CVector(60.0f, 20.0f, 0.0f), CVector(0.25f, 1.0f, 0.25f),
		CVector(20.0f, 0.0f, 0.0f), 5.0f
	);
	// “®‚©‚È‚¢°‡A
	new CMoveFloor
	(
		mpCubeModel,
		CVector(100.0f, 20.0f, 0.0f), CVector(0.25f, 1.0f, 0.25f),
		CVector(0.0f, 0.0f, 0.0f), 5.0f
	);
	// ‰ñ“]‚·‚é°‡@
	new CRotateFloor
	(
		mpCubeModel,
		CVector(135.0f, 20.0f, 0.0f), CVector(1.0f, 1.0f, 0.25f),
		0.5f
	);
	// “®‚©‚È‚¢°‡A
	new CMoveFloor
	(
		mpCubeModel,
		CVector(135.0f, 20.0f, -35.0f), CVector(0.25f, 1.0f, 0.25f),
		CVector(0.0f, 0.0f, 0.0f), 5.0f
	);
	// “®‚©‚È‚¢°‡A
	new CMoveFloor
	(
		mpCubeModel,
		CVector(135.0f, 70.0f, -52.5f), CVector(0.25f, 1.0f, 0.25f),
		CVector(0.0f, 50.0f, 0.0f), 5.0f
	);
	// “®‚©‚È‚¢°‡Biâ“¹j
	CMoveFloor* mf = new CMoveFloor
	(
		mpCubeModel,
		CVector(0.0f, 20.0f, 200.5f), CVector(4.0f, 1.0f, 2.0f),
		CVector(0.0f, 0.0f, 0.0f), 5.0f
	);
	mf->Rotate(0.0f, 0.0f, 30.0f);

	// “dŒ‚ƒGƒtƒFƒNƒg
	CLineEffect* le = new CLineEffect(ETag::eField);
	le->SetTexture("LightningBolt");
	le->SetBaseUV(CRect(0.0f, 0.0f, 128.0f, 1024.0f));
	le->SetAnimData(&mEffectAnimData);

	CVector startPos = CVector(50.0f, 10.0f, 0.0f);
	CVector endPos = CVector(50.0f, 10.0f, 150.0f);
	int div = 3;
	float width = 5.0f;
	le->AddPoint(startPos, width, width);
	for (int i = 0; i < div; i++)
	{
		float alpha = (float)(i + 1) / div;
		CVector pos = CVector::Lerp(startPos, endPos, alpha);
		le->AddPoint(pos, width, width);
	}
}

bool CField::CollisionRay(const CVector& start, const CVector& end)
{
	if (mpInstance == nullptr) return false;

	if (mpInstance->mpFloorCol != nullptr)
	{
		CHitInfo hit;
		if (CCollider::CollisionRay(mpInstance->mpFloorCol, start, end, &hit)) return true;
	}

	if (mpInstance->mpWallCol != nullptr)
	{
		CHitInfo hit;
		if (CCollider::CollisionRay(mpInstance->mpWallCol, start, end, &hit)) return true;
	}
	
	return false;
}

CCollider* CField::GetFloorCol()
{
	return mpFloorCol;
}

CCollider* CField::GetWallCol()
{
	return mpWallCol;
}

void CField::Update()
{
}

void CField::Render()
{
	mpModel->Render(Matrix());
}
