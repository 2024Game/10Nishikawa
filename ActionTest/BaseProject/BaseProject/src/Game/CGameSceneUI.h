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

	// HP最大値を設定
	void SetHpMaxPoint(float point);
	// HP現在値を設定
	void SetHpCurrPoint(float point);

	// ST最大値を設定
	void SetStMaxPoint(float point);
	// ST現在値を設定
	void SetStCurrPoint(float point);

	// ポイント残量の割合を設定（0.0～1.0）
	void SetHpPercent(float per);
	// ポイント残量の割合を設定（0.0～1.0）
	void SetStPercent(float per);

	// 更新
	void Update() override;

	// 描画
	void Render();

private:
	// ポイント残量を反映
	void ApplyPoint(int num);

	CText* mpText;				// 文字列描画用
	CImage* mpGaugeImg1;		// ゲージのイメージ
	CImage* mpWhiteImg1;		// 白イメージ
	CImage* mpGaugeImg2;		// ゲージのイメージ
	CImage* mpWhiteImg2;		// 白イメージ
	CVector2 mGaugeSize;		// ゲージのイメージのサイズ
	CVector2 mHpGaugePos;		// ゲージのイメージの位置
	CVector2 mStGaugePos;		// ゲージのイメージの位置
	float mHpMaxPoint;			// HP最大値
	float mHpCurrPoint;			// HP現在値
	float mStMaxPoint;			// ST最大値
	float mStCurrPoint;			// ST現在値
	float mHpPercent;			// HPポイント残量の割合
	float mStPercent;			// STポイント残量の割合

	CPlayer* mpPlayer;
};