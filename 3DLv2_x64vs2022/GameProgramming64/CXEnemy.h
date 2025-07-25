#ifndef CXENEMY_H
#define CXENEMY_H

#include "CXCharacter.h"
#include "CCollider.h"

class CXEnemy : public CXCharacter
{
public:
	CXEnemy();
	void Init(CModelX* model);
	//衝突処理
	void Collision(CCollider* m, CCollider* o);
private:
	//コライダの宣言
	CCollider mColSphereHead;	//頭
	CCollider mColSphereBody;	//体のコライダ
	CCollider mColSphereSword;	//剣
};
#endif // !CXENEMY_H
