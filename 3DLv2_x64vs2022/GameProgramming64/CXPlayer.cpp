#include "CXPlayer.h"
#include "CActionCamera.h"

CXPlayer::CXPlayer()
	: mColSphereHead(this, nullptr,CVector(0.0f, 5.0f, -3.0f), 0.5f)
	, mColSphereBody(this, nullptr, CVector(), 0.5f)
	, mColSphereSword(this, nullptr,CVector(-10.0f, 10.0f, 50.0f), 0.3f)
{
}

void CXPlayer::Update()
{
	if (!isAttack)
	{
		/*
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
		*/

		if (mInput.Key(VK_SPACE))
		{
			if (isAttack)
				return;
			isAttack = true;
			ChangeAnimation(3, false, 30);
			// 移動中に攻撃をするとアニメーションがストップするので
			// 攻撃したら処理を抜けています
			return;
		}

		// 課題30ーーーーーーーーーーーーーーーーーーーーーーーーーー

		//カメラの前方
		CVector cameraZ = CActionCamera::Instance()->VectorZ();
		//カメラの左方向
		CVector cameraX = CActionCamera::Instance()->VectorX();
		//キャラクタの前方
		CVector charZ = mMatrixRotate.VectorZ();
		//XZ平面にして正規化
		cameraZ.Y(0.0f); cameraZ = cameraZ.Normalize();
		cameraX.Y(0.0f); cameraX = cameraX.Normalize();
		charZ.Y(0.0f); charZ = charZ.Normalize();
		//移動方向の設定
		CVector move;
		if (mInput.Key('A'))
		{
			move = move + cameraX;
		}
		if (mInput.Key('D'))
		{
			move = move - cameraX;
		}
		if (mInput.Key('W'))
		{
			move = move + cameraZ;
		}
		if (mInput.Key('S'))
		{
			move = move - cameraZ;
		}
		
		//移動あり
		if (move.Length() > 0.0f)
		{
			//遊び
			const float MARGIN = 0.06f;
			//正規化
			move = move.Normalize();
			//自分の向きと向かせたい向きで外積
			float cross = charZ.Cross(move).Y();
			//自分の向きと向かせたい向きで内積
			float dot = charZ.Dot(move);
			//外積がプラスは左回転
			if (cross > MARGIN)
			{
				mRotation.Y(mRotation.Y() + 5.0f);
			}
			//外積がマイナスは右回転
			else if (cross < -MARGIN)
			{
				mRotation.Y(mRotation.Y() - 5.0f);
			}
			//前後の向きが同じとき内積は1.0
			else if (dot < 1.0f - MARGIN)
			{
				mRotation.Y(mRotation.Y() - 5.0f);
			}
			//移動方向へ移動
			mPosition = mPosition + move * 0.1f;
			ChangeAnimation(1, true, 60);
		}
		// ここ少し変えました
		else if (move.Length() == 0.0f && AnimationIndex() == 1)
		{
			ChangeAnimation(0, true, 60);
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

void CXPlayer::Init(CModelX* model)
{
	CXCharacter::Init(model);
	//合成行列の設定
	//頭
	mColSphereHead.Matrix(&mpCombinedMatrix[12]);
	//体
	mColSphereBody.Matrix(&mpCombinedMatrix[9]);
	//剣
	mColSphereSword.Matrix(&mpCombinedMatrix[22]);
	mColSphereSword.SetTag(1);
}