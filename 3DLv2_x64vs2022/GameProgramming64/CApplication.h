#pragma once
#include "CTexture.h"
#include "CBullet.h"
#include "CEnemy.h"
#include "CPlayer.h"
#include "CFont.h"
#include "CSound.h"

#include "CModel.h"
#include "CCharacter3.h"
#include "CTaskManager.h"
#include "CColliderTriangle.h"
#include "CColliderMesh.h"
#include "CUI.h"
#include "CCamera.h"

#include "CModelX.h"
#include "CMatrix.h"

#include "CXCharacter.h"
#include "CXPlayer.h"
#include "CXEnemy.h"
#include "CPaladin.h"

class CApplication
{
public:
	~CApplication();
	static CUi* Ui();	//UIクラスのインスタンスを取得
	//モデルビュー行列の取得
	static const CMatrix& ModelViewInverse();

	static CTexture* Texture();
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
	CMatrix mMatrix;
	static CUi* spUi;	//UIクラスのポインタ

	//モデルからコライダを生成
	CColliderMesh mColliderMesh;

	//モデルビューの逆行列
	static CMatrix mModelViewInverse;

	//C5モデル
	CModel mModelC5;

	//CPlayer mPlayer;

	CModel mBackGround; //背景モデル
	//モデルクラスのインスタンス作成
	CModel mModel;
	CVector mEye;

	CSound mSoundBgm;	//BGM
	CSound mSoundOver;	//GAME OVER
	EState mState;

	CInput mInput;
	CFont mFont;
	CPlayer* mpPlayer;
	CBullet* mpBullet;
	//CRectangle mRectangle;
	//CCharacter mRectangle;
	static CTexture mTexture;
	CEnemy* mpEnemy;
	CModelX mModelX;

	//キャラクタのインスタンス
	CXPlayer mXPlayer;
	//敵のインスタンス
	CXEnemy mXEnemy;
	//Knightのインスタンス
	CModelX mKnight;

	CPaladin* mpPaladin;
	int anim;
};