#pragma once
#include <iostream>
#include "CEnemy.h"

// 基底クラス
class CFish : public CEnemy
{
public:
    virtual void Show() const = 0;

    // コンストラクタ
    CFish();
    // デストラクタ
    ~CFish();

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

	// 戦闘相手の方へ向く
	void LookAtTargetPos();

	// 状態切り替え
	void ChangeState(int state) override;

	// 待機状態の更新処理
	void UpdateIdle();
	// 回遊時の更新処理
	void UpdateMove();
	// 仰け反り状態の更新処理
	void UpdateHit();
	// 死亡状態の更新処理
	void UpdateDeath();

	bool mIsBattle;					// 戦闘状態か
	float mIdletime;			// 待機状態時の待機時間
	CVector mTargetPos;				// 目的地
};



// 派生クラス
class CRainbowTrout : public CFish
{
public:
	// コンストラクタ
	CRainbowTrout();
	// デストラクタ
	~CRainbowTrout();
    void Show() const override
    {
        std::cout << "ニジマス\n";
    }

private:
	// アニメーションの種類
	enum class EAnimType
	{
		None = -1,

		eTPose,		// Tポーズ
		eIdle,		// 待機
		eHit,		// 仰け反り
		eDeath,		// 死亡

		Num
	};
};

class Tuna : public CFish
{
public:
    void Show() const override
    {
        std::cout << "マグロ\n";
    }
};

class Shrimp : public CFish
{
public:
    void Show() const override
    {
        std::cout << "エビ\n";
    }
};

class Octopus : public CFish
{
public:
    void Show() const override
    {
        std::cout << "タコ\n";
    }
};
