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
	// ポイント残量の割合を設定（0.0～1.0）
	void SetHpPercent(float per);

	// ST最大値を設定
	void SetStMaxPoint(float point);
	// ST現在値を設定
	void SetStCurrPoint(float point);
	// ポイント残量の割合を設定（0.0～1.0）
	void SetStPercent(float per);

	// S1最大値を設定
	void SetS1MaxPoint(float point);
	// S1現在値を設定
	void SetS1CurrPoint(float point);
	// ポイント残量の割合を設定（0.0～1.0）
	void SetS1Percent(float per);

	// 更新
	void Update() override;

	// 描画
	void Render();

private:
	// ポイント残量を反映
	void ApplyPoint(int num);

	CText* mpText;				// 文字列描画用
	CPlayer* mpPlayer;

	CVector2 mGaugeSize;		// ゲージのイメージのサイズ

	CImage* mpHpGaugeImg;		// ゲージのイメージ
	CImage* mpHpWhiteImg;		// 白イメージ
	CVector2 mHpGaugePos;		// ゲージのイメージの位置
	float mHpMaxPoint;			// HP最大値
	float mHpCurrPoint;			// HP現在値
	float mHpPercent;			// HPポイント残量の割合

	CImage* mpStGaugeImg;		// ゲージのイメージ
	CImage* mpStWhiteImg;		// 白イメージ
	CVector2 mStGaugePos;		// ゲージのイメージの位置
	float mStMaxPoint;			// ST最大値
	float mStCurrPoint;			// ST現在値
	float mStPercent;			// STポイント残量の割合

	// スキル関連
	CVector2 mSkillGaugeSize;		// ゲージのイメージのサイズ

	CImage* mpS1IconImg;		// ゲージのイメージ
	CImage* mpS1WhiteImg;		// 白イメージ
	CVector2 mS1GaugePos;		// ゲージのイメージの位置
	float mS1MaxPoint;			// スキル1最大値(クールタイム)
	float mS1CurrPoint;			// スキル1現在値
	float mS1Percent;			// ポイント残量の割合
};