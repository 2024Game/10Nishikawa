#pragma once
#include "CRectangle.h"
#include "CTexture.h"
#define VELOCITY 2.0f //移動速度

class CCharacter : public CRectangle
{
public:

	bool Enabled();
	enum class ETag	//識別子
	{
		EZERO,		//初期値
		EBULLET,	//弾
		EENEMY,		//敵
		EPLAYER,	//プレイヤー
		ETURN,		//折り返し
		EBLOCK,		//ブロック
	};
	
	ETag Tag();

	//衝突処理２
	virtual void Collision() {};
	//衝突処理４
	//Collision(自分のポインタ, 衝突相手のポインタ)
	virtual void Collision(CCharacter* m, CCharacter* o) {};

	virtual void Update() = 0;

	enum class EState
	{
		EMOVE,	//移動
		ESTOP,	//停止
		EJUMP,	//ジャンプ
		ECRY,   //泣く
	};

	EState State(); //状態を取得する

	CCharacter();
	CTexture* Texture();

	void Texture(CTexture* pTexture,
		int left, int right, int bottom, int top);

	virtual void Render();

	void Move();

protected:
	float mVx;		//X軸速度
	float mVy;		//Y軸速度
	ETag mTag;		//識別子
	EState mState;
	bool mEnabled;

private:
	CTexture* mpTexture;
	int mLeft, mRight, mBottom, mTop;

};