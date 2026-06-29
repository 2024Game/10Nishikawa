#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

class CField : public CObjectBase
{
public:
	CField();
	~CField();

	// フィールドとのレイ判定の結果を返す
	static bool CollisionRay(const CVector& start, const CVector& end);

	// 各コライダーを返す
	CCollider* GetFloorCol();
	CCollider* GetWallCol();

	void Update();
	void Render();

private:
	void CreateFieldObjects();

	static CField* mpInstance;

	CModel* mpModel;

	CColliderMesh* mpFloorCol;
	CColliderMesh* mpWallCol;

	CModel* mpCubeModel;
	CModel* mpCylinderModel;

	TexAnimData mEffectAnimData;
};