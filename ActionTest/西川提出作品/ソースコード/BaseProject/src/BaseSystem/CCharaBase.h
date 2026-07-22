#pragma once
#include "CObjectBase.h"

/// <summary>
/// キャラクターのベースクラス
/// </summary>
class CCharaBase : public CObjectBase
{
public:
	// コンストラクタ
	CCharaBase(ETag tag, ETaskPriority prio, int sortOrder = 0,
		ETaskPauseType pause = ETaskPauseType::eGame);
	// デストラクタ
	virtual ~CCharaBase();

	// 最大HPを取得
	float GetMaxHp() const;
	// 現在HPを取得
	float GetHp() const;

	// 最大スタミナを取得
	float GetMaxSt() const;
	// 現在スタミナを取得
	float GetSt() const;

	// ダメージを受ける
	virtual void TakeDamage(float damage, CObjectBase* causer);

	// キックを受ける
	virtual void TakeKick(CObjectBase* causer);

	// スタミナを回復する
	void GainStamina(float amount);
	// スタミナを消費する
	void UseStamina(float amount);

	// 死亡
	virtual void Death();
	// 死んでいるかどうか
	bool IsDeath() const;

protected:
	float mMaxHp;	// 最大HP
	float mHp;	// 現在HP
	float mMaxSt;	// 最大スタミナ
	float mSt;	// 現在スタミナ
};
