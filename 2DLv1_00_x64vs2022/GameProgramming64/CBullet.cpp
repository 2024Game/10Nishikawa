#include "CBullet.h"
#include "CApplication.h"

void CBullet::Update()
{
	if (mState == EState::EMOVE)
	{
		float y = Y() + H();

		if (y > 620.0f)
			y = 88.0f;

		Y(y);

	}


}

bool CBullet::Collision(CRectangle* rect)
{
	if (CRectangle::Collision(rect))
	{
		mState = EState::ESTOP;
		return true;
	}
	
	return false;
}

CBullet::CBullet()
{
	mState = EState::ESTOP;
}

CBullet::CBullet(float x, float y, float w, float h, float l, float r, float b, float t, CTexture* pt)
{
	Set(x, y, w, h);
	Texture(pt, l, r, b, t);
	mState = EState::EMOVE;
	mTag = ETag::EBULLET;
}

void CBullet::Collision()
{
	CApplication::CharacterManager()->Collision(this);
}

void CBullet::Collision(CCharacter* m, CCharacter* o)
{
	switch (o->Tag())
	{
	case ETag::EBULLET:
		break;
	default:
		if (CRectangle::Collision(o))
		{
			mState = EState::ESTOP;
			mEnabled = false;
		}
	}
}
