#pragma once
#include "CUIBase.h"

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
};