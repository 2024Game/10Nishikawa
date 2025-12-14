#pragma once
#include "CUIBase.h"
#include "CPlayer.h"

class CText;
class CImage;

class CGameSceneUI : public CUIBase
{
public:
	// コンストラクタ
	CGameSceneUI(CPlayer* player);
	// デストラクタ
	~CGameSceneUI();

	// 最大値を設定
	void SetMaxPoint(float point);
	// 現在値を設定
	void SetCurrPoint(float point);
	// ポイント残量の割合を設定（0.0～1.0）
	void SetPercent(float per);

	// 更新
	void Update() override;
	// 描画
	void Render();

private:
	// ポイント残量を反映
	void ApplyPoint();

	void SetDepthInfo();

	CText* mpText;			// 文字列描画用
	CImage* mpGaugeImg;		// ゲージのイメージ
	CImage* mpWhiteImg;		// 白イメージ
	CVector2 mGaugeSize;	// ゲージのイメージのサイズ
	CVector2 mGaugePos;		// ゲージのイメージの位置
	float mMaxPoint;		// 最大値
	float mCurrPoint;		// 現在値
	float mPercent;			// ポイント残量の割合

	CPlayer* mpPlayer;
};