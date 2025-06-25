#include "CXPlayer.h"

void CXPlayer::Update()
{
	if (!isAttack)
	{
		//printf("攻撃中では有りません\n");
		//Y軸+ 左へ2度回転
		if (mInput.Key('A'))
		{
			mRotation = mRotation + CVector(0, 2, 0);
			//printf("左を向いてるはず\n");
		}
		//Y軸+ 右へ2度回転
		if (mInput.Key('D'))
		{
			mRotation = mRotation - CVector(0, 2, 0);
			//printf("右を向いてるはず\n");
		}
		//Wキー入力で前進
		if (mInput.Key('W'))
		{
			ChangeAnimation(1, true, 60);
			//Z軸方向の値を回転させ移動させる
			mPosition = mPosition + CVector(0.0f, 0.0f, 0.1f) * mMatrixRotate;
		}
		else if (!mInput.Key('W') && AnimationIndex() == 1)
		{
			ChangeAnimation(0, true, 60);
		}

		if (mInput.Key(VK_SPACE))
		{
			if (isAttack)
				return;
			isAttack = true;
			ChangeAnimation(3, false, 30);
		}
	}
	else if (isAttack)
	{
		//printf("攻撃中です\n");
	}
	if (IsAnimationFinished())
	{
		if (AnimationIndex() == 3)
		{
			ChangeAnimation(4, false, 30);
		}
		else if (AnimationIndex() == 4)
		{
			ChangeAnimation(0, true, 60);
			isAttack = false;
		}
	}
	CXCharacter::Update();
}
