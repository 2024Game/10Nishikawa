#include "CXPlayer.h"

void CXPlayer::Update()
{
	//Y軸+ 左へ2度回転
	if (mInput.Key('A'))
	{
		mRotation = mRotation + CVector(0, 2, 0);
		//printf("左を向いてるはず\n");
	}
	//Y軸+ 右へ2度回転
	if (mInput.Key('D'))
	{
		mRotation = mRotation - CVector(0,2,0);
		//printf("右を向いてるはず\n");
	}
	//Wキー入力で前進
	if (mInput.Key('W'))
	{
		ChangeAnimation(1, true, 60);
		//Z軸方向の値を回転させ移動させる
		mPosition = mPosition + CVector(0.0f, 0.0f, 0.1f) * mMatrixRotate;
	}
	else if (!mInput.Key('W'))
	{
		ChangeAnimation(0, true, 60);
	}
	CXCharacter::Update();
}
