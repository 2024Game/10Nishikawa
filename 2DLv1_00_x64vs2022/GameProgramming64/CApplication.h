#pragma once
#include "CRectangle.h"
#include "CTexture.h"
#include "CCharacter.h"
#include "CBullet.h"
#include "CEnemy.h"
#include "CPlayer.h"
#include "CFont.h"
#include "CMiss.h"
#include "CCharacterManager.h"
#include "CGame.h"
#include "CSound.h"
#include "CEnemy2.h"

class CApplication
{
public:
	static CTexture* Texture();
	static CCharacterManager* CharacterManager();
	enum  class EState
	{
		ESTART,
		EPLAY,
		ECLEAR,
		EOVER,
	};
	//最初に一度だけ実行するプログラム
	void Start();
	//繰り返し実行するプログラム
	void Update();

private:
	CGame* mpGame;
	EState mState;
	CInput mInput;
	CPlayer*mpPlayer;
	static CTexture mTexture;
	CEnemy*mpEnemy;
	CBullet*mpBullet;
	CFont mFont;
	CMiss*mpMiss;
	static CCharacterManager mCharacterManager;
	CSound mSoundBgm;	//BGM
	CSound mSoundOver;	//ゲームオーバー音声
};