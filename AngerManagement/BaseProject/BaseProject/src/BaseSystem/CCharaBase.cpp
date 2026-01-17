#include "CCharaBase.h"

// コンストラクタ
CCharaBase::CCharaBase(ETag tag, ETaskPriority prio, int sortOrder, ETaskPauseType pause)
	: CObjectBase(tag, prio, sortOrder, pause)
	, mMaxHp(100.0f)
	, mHp(mMaxHp)
{
}

// デストラクタ
CCharaBase::~CCharaBase()
{
}

// 最大HPを取得
float CCharaBase::GetMaxHp() const
{
	return mMaxHp;
}

// 現在HPを取得
float CCharaBase::GetHp() const
{
	return mHp;
}

float CCharaBase::GetMaxSt() const
{
	return mMaxSt;
}

float CCharaBase::GetSt() const
{
	return mSt;
}

// ダメージを受ける
void CCharaBase::TakeDamage(float damage, CObjectBase* causer)
{
	// 既に死亡していたら、ダメージを受けない
	if (IsDeath()) return;

	// 受けたダメージが現在HP以上なら
	if (damage >= mHp)
	{
		// HPを0にして、死亡
		mHp = 0;
		Death();
	}
	// 現在HPの方が多い場合は、ダメージ分減らす
	else
	{
		mHp -= damage;
	}
}

void CCharaBase::GainStamina(float amount)
{
	// 回復したスタミナが最大値から現在値を引いた値以上なら
	if (amount >= mMaxSt - mSt)
	{
		// スタミナをMaxにする
		mSt = mMaxSt;
	}
	else
	{
		mSt += amount;
	}
}

void CCharaBase::UseStamina(float amount)
{
	// 消費したスタミナが現在値以上なら
	if (amount >= mSt)
	{
		// スタミナを0にする
		mSt = 0;
	}
	// 現在値の方が多い場合は、値分減らす
	else
	{
		mSt -= amount;
	}
}

// 死亡
void CCharaBase::Death()
{
}

// 死んでいるかどうか
bool CCharaBase::IsDeath() const
{
	// 現在HPが0ならば、死亡
	return mHp <= 0;
}
