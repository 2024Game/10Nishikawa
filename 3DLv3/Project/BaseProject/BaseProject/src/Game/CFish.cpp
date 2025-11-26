#include "CFish.h"
#include "Maths.h"
#include "CFishManager.h"

CFish::CFish(const std::string& fishTypeName)
	: mIsInvincibility(false)
	, mInvincibilityTime(3.0f)
	, mFishTypeName(fishTypeName)
	, mIdletime(0.0f)
	, mTargetPos(CVector(0.0f, 0.0f, 0.0f))
	, mScore(0.0f)
{
	// CFishManagerのリストに自分を追加
	CFishManager::Instance()->Add(this);
}

CFish::~CFish()
{
	// CFishManagerのリストから自分を削除
	CFishManager::Instance()->Remove(this);
}

const std::string& CFish::GetFishTypeName() const
{
	return mFishTypeName;
}

void CFish::TakeDamage(float damage, CObjectBase* causer)
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

void CFish::Death()
{
	// 死亡状態に切り替え
	ChangeState((int)EState::eDeath);

}

void CFish::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// ベースの衝突処理を呼び出す
	CEnemy::Collision(self, other, hit);
}

void CFish::Update()
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

float CFish::GetScore()
{
	return mScore;
}

void CFish::LookAtTargetPos()
{
}

void CFish::ChangeState(int state)
{
}

void CFish::UpdateIdle()
{
}

void CFish::UpdateMove()
{
}

void CFish::UpdateHit()
{
}

void CFish::UpdateDeath()
{
}






