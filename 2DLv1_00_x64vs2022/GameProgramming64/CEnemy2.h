#pragma once
#include "CCharacter.h"
#include "CApplication.h"

class CEnemy2 : public CCharacter
{

public:
	//“G‚Ì”‚ğæ“¾
	static int Num();
	static void Num(int num);
	//static void Test();
	CEnemy2(float x, float y, float w, float h, CTexture* pt);

	void Update();

	//Õ“Ëˆ—2
	void Collision();
	//Õ“Ëˆ—4
	void Collision(CCharacter* m, CCharacter* o);

private:
	static int sNum;	//“G‚Ì”
	CRectangle* mRectangle;
	
};