#pragma once
#include "CEnemy.h"
#include <iostream>

// 基底クラス
class CFish : public CEnemy
{
public:
    virtual void Show() const = 0;

    // コンストラクタ
    CFish(const std::string& fishTypeName);
    // デストラクタ
    virtual ~CFish();

	const std::string& GetFishTypeName() const;

    // ダメージを受ける
    void TakeDamage(int damage, CObjectBase* causer) override;
    // 死亡
    void Death() override;
    // 衝突処理
    void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

    // 更新
    void Update() override;

protected:
	
	// プレイヤーの状態
	enum class EState
	{
		eIdle,		// 待機状態
		eMove,		// 追いかける
		eHit,		// 仰け反り
		eDeath,		// 死亡
	};

	// 目的地の方へ向く
	virtual void LookAtTargetPos();

	// 状態切り替え
	virtual void ChangeState(int state) override;

	// 待機状態の更新処理
	virtual void UpdateIdle();
	// 回遊時の更新処理
	virtual void UpdateMove();
	// 仰け反り状態の更新処理
	virtual void UpdateHit();
	// 死亡状態の更新処理
	virtual void UpdateDeath();

	std::string mFishTypeName;		// 魚の種類名
	bool mIsInvincibility;			// 無敵状態か
	float mInvincibilityTime;
	float mIdletime;				// 待機状態時の待機時間
	CVector mTargetPos;				// 目的地
};