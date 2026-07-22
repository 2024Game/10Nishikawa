#pragma once
#include "CItem.h"
#include "CModel.h"

// 低級回復薬のクラス
class CLowHealP : public CItem
{
public:
	// コンストラクタ
	CLowHealP();
	// デストラクタ
	~CLowHealP();

	// 描画
	void Render() override;

private:
	CModel* mpModel;	// 低級回復薬のモデル
};