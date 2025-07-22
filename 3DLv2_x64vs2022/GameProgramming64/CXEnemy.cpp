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
	//‡¬s—ñ‚Ìİ’è
	//“ª
	mColSphereHead.Matrix(&mpCombinedMatrix[11]);
	//‘Ì
	mColSphereBody.Matrix(&mpCombinedMatrix[8]);
	//Œ•
	mColSphereSword.Matrix(&mpCombinedMatrix[21]);
}
