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

class CApplication
{
public:
	static CTexture* Texture();
	static CCharacterManager* CharacterManager();
	static CCharacterManager mCharacterManager;
	enum class EState
	{
		ESTART,	//ゲーム開始
		EPLAY,	//ゲーム中
		ECLEAR,	//ゲームクリア
		EOVER,	//ゲームオーバー
	};

	//最初に一度だけ実行するプログラム
	void Start();
	//繰り返し実行するプログラム
	void Update();
private:
	CSound mSoundBgm;	//BGM
	CSound mSoundOver;	//GAME OVER
	CGame* mpGame;
	EState mState;

	CMiss* mpMiss;
	CInput mInput;
	CFont mFont;
	CPlayer* mpPlayer;
	CBullet* mpBullet;
//	CRectangle mRectangle;
	//CCharacter mRectangle;
	static CTexture mTexture;
	CEnemy* mpEnemy;
};