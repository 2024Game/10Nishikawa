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
	CCollider mColSphereSword0;	//剣 1
	CCollider mColSphereSword1;	//剣 2
	CCollider mColSphereSword2;	//剣 3
};
#endif // !CXENEMY_H
