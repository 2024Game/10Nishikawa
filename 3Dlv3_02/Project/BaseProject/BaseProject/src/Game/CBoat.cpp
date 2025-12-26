#include "CBoat.h"
#include "Maths.h"
#include "CBoatManager.h"

CBoat::CBoat(const std::string& boatTypeName)
	: mIsInvincibility(false)
	, mInvincibilityTime(0.0f)
	, mBoatTypeName(boatTypeName)
	, mIdletime(0.0f)
	, mTargetPos(CVector(0.0f, 0.0f, 0.0f))
	, mScore(0.0f)
{
	if (mBoatTypeName != "Jellyfish")
	{
		// CBoatManagerのリストに自分を追加
		CBoatManager::Instance()->Add(this);
	}
}

CBoat::~CBoat()
{
	// CBoatManagerのリストから自分を削除
	CBoatManager::Instance()->Remove(this);
}

const std::string& CBoat::GetBoatTypeName() const
{
	return mBoatTypeName;
}

void CBoat::TakeDamage(float damage, CObjectBase* causer)
{
	mInvincibilityTime = 3.0f;
	if (mIsInvincibility) return;

	// ベースクラスのダメージ処理を呼び出す
	CEnemy::TakeDamage(damage, causer);

	// 死亡していなければ、
	if (!IsDeath())
	{
		// 仰け反り状態へ移行
		// ChangeState((int)EState::eHit);

		// 移動を停止
		// mMoveSpeed = CVector::zero;
	}
	if (mHp <= 0)
	{
		mpPlayer->AddScore(mScore);
	}
}

void CBoat::Death()
{
	// 死亡状態に切り替え
	ChangeState((int)EState::eDeath);

}

void CBoat::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// ベースの衝突処理を呼び出す
	CEnemy::Collision(self, other, hit);
}

void CBoat::Update()
{
	if (mInvincibilityTime > 0.0f)
	{
		mInvincibilityTime -= Times::DeltaTime();
	}
	if (mInvincibilityTime < 0.0f)
	{
		mInvincibilityTime = 0.0f;
	}

	mIsInvincibility = (mInvincibilityTime == 0.0f) ? false : true;

	// 状態に合わせて、更新処理を切り替える
	switch ((EState)mState)
	{
		// 待機状態
	case EState::eIdle:		UpdateIdle();	break;
		// 移動
	case EState::eMove:		UpdateMove();	break;
		// 仰け反り
	case EState::eHit:		UpdateHit();	break;
		// 死亡状態
	case EState::eDeath:	UpdateDeath();	break;
	}

	// 敵のベースクラスの更新
	CEnemy::Update();
}

void CBoat::Render()
{
	CEnemy::Render();
}

float CBoat::GetScore()
{
	return mScore;
}

void CBoat::LookAtTargetPos()
{
}

void CBoat::ChangeState(int state)
{
}

void CBoat::UpdateIdle()
{
}

void CBoat::UpdateMove()
{
}

void CBoat::UpdateHit()
{
}

void CBoat::UpdateDeath()
{
}






