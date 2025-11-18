#pragma once
#include "CUIBase.h"

class CText;
class CImage;

class CGameSceneUI : public CUIBase
{
public:
	// コンストラクタ
	CGameSceneUI();
	// デストラクタ
	~CGameSceneUI();

private:
	CText* mpText;			// 文字列描画用
	CImage* mpGaugeImg;		// ゲージのイメージ
	CImage* mpWhiteImg;		// 白イメージ
	CVector2 mGaugeSize;	// ゲージのイメージのサイズ
	int mMaxPoint;			// 最大値
	int mCurrPoint;			// 現在値
	float mPercent;			// ポイント残量の割合
};