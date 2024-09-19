#pragma once
#include "CCharacter.h"
#include "CInput.h"
#include "CSound.h"

class CPlayer2 : public CCharacter
{
public:
	//
	static int Hp();
	//Õ“Ëˆ—2
	void Collision();
	//Õ“Ëˆ—4
	void Collision(CCharacter* m, CCharacter* o);

	CPlayer2(float x, float y, float w, float h, CTexture* pt);

	void Update();

private:
	CSound mSoundJump;	//ƒWƒƒƒ“ƒvSE
	static int sHp;  //HP
	int mInvincible; //
	CInput mInput;
};