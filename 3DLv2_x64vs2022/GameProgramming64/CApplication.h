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

#include "CModel.h"
#include "CCharacter3.h"
#include "CTaskManager.h"
#include "CColliderTriangle.h"
#include "CColliderMesh.h"

class CApplication
{
public:
	~CApplication();
	static CUi* Ui();	//UIクラスのインスタンスを取得
	//モデルビュー行列の取得
	static const CMatrix& ModelViewInverse();

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
	static CUi* spUi;	//UIクラスのポインタ

	//モデルからコライダを生成
	CColliderMesh mColliderMesh;


	//モデルビューの逆行列
	static CMatrix mModelViewInverse;

	//C5モデル
	CModel mModelC5;

	CPlayer mPlayer;

	CModel mBackGround; //背景モデル
	//モデルクラスのインスタンス作成
	CModel mModel;
	CVector mEye;

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