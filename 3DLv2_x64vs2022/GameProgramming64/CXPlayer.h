#ifndef CXPLAYER_H
#define CXPLAYER_H

#include "CXCharacter.h"
#include "CInput.h"
#include "CCollider.h"

class CXPlayer : public CXCharacter
{
public:
	CXPlayer();
	// 更新処理
	void Update();
	void Init(CModelX* model);

private:
	//コライダの宣言
	CCollider mColSphereHead;	//頭
	CCollider mColSphereBody;	//体のコライダ
	CCollider mColSphereSword;	//剣
	CInput mInput;
	bool isAttack = false;
};
#endif // !CXPLAYER_H