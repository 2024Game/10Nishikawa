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
	mpModel->SetCullFace(false);

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
	mpSkyboxModel = CResourceManager::Get<CModel>("Skybox");

	// 海底
	new CMoveFloor
	(
		mpSeaSandModel,
		CVector(0.0f, -500.0f, 0.0f), CVector(4.2f, 1.0f, 4.2f),
		CVector(0.0f, 0.0f, 0.0f), 5.0f
	);

	// スカイボックス
	new CMoveFloor
	(
		mpSkyboxModel,
		CVector(0.0f, 0.0f, 0.0f), CVector(110.0f, 110.0f, 110.0f),
		CVector(0.0f, 0.0f, 0.0f), 5.0f
	);
}

void CField::Update()
{
}

void CField::Render()
{
	CCamera* camera = CCamera::CurrentCamera();
	CVector camPos = camera->Position();
	CVector camVec = camPos - Position();
	float dot = CVector::Dot(CVector::up, camVec);
	mpModel->SetAlpha(dot >= 0.0f ? 1.0f : 0.4f);
	mpModel->Render(Matrix());
}
