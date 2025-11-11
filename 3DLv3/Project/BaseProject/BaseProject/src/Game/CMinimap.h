#pragma once
#include "CUIBase.h"
#include "CSound.h"

class CMinimap : public CUIBase
{
public:
	//コンストラクタ
	CMinimap();

	//デストラクタ
	~CMinimap();

	// ミニマップを作成
	void Render() override;

private:
	// 塗りつぶした円や点を描く
	void DrawFilledCircle(float cx, float cy, float radius, float r, float g, float b, float a);

	float mUpdateTimer = 0.0f;         // 更新までの経過時間

	struct FishMapInfo
	{
		CVector position; // 5秒ごとに更新される魚の位置
	};

	std::vector<FishMapInfo> mFishMapInfos;

	CSound* mpFishfinderSound;	// 探知機音データ
};