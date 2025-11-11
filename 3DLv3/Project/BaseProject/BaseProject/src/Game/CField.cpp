#include "CField.h"
#include "CCollisionManager.h"
#include "CMoveFloor.h"
#include "CRotateFloor.h"
#include "CLineEffect.h"

CField::CField()
	: CObjectBase(ETag::eField, ETaskPriority::eBackground)
	, mEffectAnimData(1, 11, true, 11, 0.03f)
{
	mpModel = CResourceManager::Get<CModel>("Field");

	mpColliderMesh = new CColliderMesh(this, ELayer::eField, mpModel, true);

	CreateFieldObjects();
}

CField::~CField()
{
	if (mpColliderMesh != nullptr)
	{
		delete mpColliderMesh;
		mpColliderMesh = nullptr;
	}
}

void CField::CreateFieldObjects()
{
	mpSeaSandModel = CResourceManager::Get<CModel>("SeaSand");

	// “®‚©‚È‚¢°‡@
	new CMoveFloor
	(
		mpSeaSandModel,
		CVector(0.0f, -450.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f),
		CVector(0.0f, 0.0f, 0.0f), 5.0f
	);
}

void CField::Update()
{
}

void CField::Render()
{
	mpModel->Render(Matrix());
}
