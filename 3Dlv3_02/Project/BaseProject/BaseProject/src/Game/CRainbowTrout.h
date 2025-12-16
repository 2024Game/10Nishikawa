#pragma once
#include "CBoat.h"

class CRainbowTrout : public CBoat
{
public:
	// コンストラクタ
	CRainbowTrout(CPlayer* player);
	// デストラクタ
	~CRainbowTrout();

	void Init();

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

	// 目的地の方へ向く
	void LookAtTargetPos() override;

	// 状態切り替え
	void ChangeState(int state) override;

	// 待機状態の更新処理
	void UpdateIdle() override;
	// 回遊時の更新処理
	void UpdateMove() override;
	// 仰け反り状態の更新処理
	void UpdateHit() override;
	// 死亡状態の更新処理
	void UpdateDeath() override;
};