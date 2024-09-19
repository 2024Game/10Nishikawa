#pragma once
#include "CCharacter.h"

class CEnemy : public CCharacter
{
public:
	//衝突処理２
	void Collision();
	//衝突処理４
	//Collision(自分のポインタ, 衝突相手のポインタ)
	void Collision(CCharacter* m, CCharacter* o);

	CEnemy(float x, float y, float w, float h, float l, float r, float b, float t, CTexture* pt);

	void Update();
	bool Collision(CRectangle* rect);
};