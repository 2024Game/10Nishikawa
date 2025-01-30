#pragma once
#include "CRectangle.h"
#include "CTexture.h"
#include "CCharacter.h"
#include "CBullet.h"
#include "CEnemy.h"
#include "CPlayer.h"
#include "CEnemy3.h"
#include "CFont.h"
#include "CMiss.h"
#include "CCharacterManager.h"
#include "CGame.h"
#include "CSound.h"

#include "CVector.h"
#include "CModel.h"

#include "CMatrix.h"
#include "CTransform.h"
#include "CCharacter3.h"
#include "CTaskManager.h"

#include "CCollisionManager.h"
#include "CBillBoard.h"
#include "CColliderTriangle.h"
#include "CColliderMesh.h"
class CApplication
{
public:
	~CApplication();
	static CUi* Ui();	//UIクラスのインスタンスを取得
	//モデルビュー行列の取得
	static const CMatrix& ModelViewInverse();

	//static CTaskManager* TaskManager();
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

	//三角コライダの作成
	//CColliderTriangle mColliderTriangle;
	//CColliderTriangle mColliderTriangle2;

	//モデルビューの逆行列
	static CMatrix mModelViewInverse;

	//static CTaskManager mTaskManager;

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

	CVector mEye;
	//モデルクラスのインスタンス作成
	CModel mModel;
	//C5モデル
	CModel mModelC5;
	CModel mBackGround; //背景モデル
	//CCharacter3 mCharacter;
	CPlayer mPlayer;
};