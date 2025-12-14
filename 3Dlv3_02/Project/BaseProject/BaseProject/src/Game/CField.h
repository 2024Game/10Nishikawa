#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

class CField : public CObjectBase
{
public:
	CField();
	~CField();

	void Update();
	void Render();

private:
	void CreateFieldObjects();

	CModel* mpModel;
	CColliderMesh* mpColliderMesh;

	CModel* mpSeaSandModel;
	CModel* mpSkyboxModel;
	CModel* mpCylinderModel;

	TexAnimData mEffectAnimData;
};