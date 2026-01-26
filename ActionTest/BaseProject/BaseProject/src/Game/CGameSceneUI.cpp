#include "CGameSceneUI.h"
#include "CText.h"
#include "CInput.h"
#include "CImage.h"
#include <Maths.h>

CGameSceneUI::CGameSceneUI(CPlayer* player)
	: mpHpGaugeImg(nullptr)
	, mpHpWhiteImg(nullptr)
	, mpStGaugeImg(nullptr)
	, mpStWhiteImg(nullptr)
	, mHpMaxPoint(player->GetMaxHp())
	, mHpCurrPoint(mHpMaxPoint)
	, mStMaxPoint(player->GetMaxSt())
	, mStCurrPoint(mStMaxPoint)
	, mHpPercent(1.0f)
	, mStPercent(1.0f)
	, mGaugeSize(CVector2(500.0f, 30.0f))
	, mHpGaugePos(CVector2(25.0f, 25.0f))
	, mStGaugePos(CVector2(25.0f, 50.0f))
	, mpPlayer(player)
	, mSkillGaugeSize(CVector2(80.0f, 80.0f))
	, mpS1IconImg(nullptr)
	, mpS1WhiteImg(nullptr)
	, mS1MaxPoint(player->GetMaxS1())
	, mS1CurrPoint(mS1MaxPoint)
	, mS1Percent(1.0f)
	, mS1GaugePos(CVector2((WINDOW_WIDTH / 2) - 110.0f, WINDOW_HEIGHT - 105.0f))
{
	mpText = new CText
	(
		nullptr, 24,
		CVector2(WINDOW_WIDTH - 150.0f, WINDOW_HEIGHT - 150.0f),
		CVector2(100.0f, 100.0f),
		CColor::white
	);

	mpText->SetTextAlign(ETextAlignH::eCenter, ETextAlignV::eMiddle);

	// HPゲージ
	// ゲージのイメージを読み込み
	mpHpGaugeImg = new CImage
	(
		"UI\\gauge.png",
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mpHpGaugeImg->SetSize(mGaugeSize);
	mpHpGaugeImg->SetPos(mHpGaugePos);

	// 白イメージを読み込み
	mpHpWhiteImg = new CImage
	(
		"UI\\white.png",
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);

	// STゲージ
	// ゲージのイメージを読み込み
	mpStGaugeImg = new CImage
	(
		"UI\\gauge.png",
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mpStGaugeImg->SetSize(mGaugeSize);
	mpStGaugeImg->SetPos(mStGaugePos);

	// 白イメージを読み込み
	mpStWhiteImg = new CImage
	(
		"UI\\white.png",
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);


	// S1ゲージ
	// ゲージのイメージを読み込み
	mpS1IconImg = new CImage
	(
		"UI\\KickIcon.png",
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mpS1IconImg->SetSize(mSkillGaugeSize);
	mpS1IconImg->SetPos(mS1GaugePos);

	// 白イメージを読み込み
	mpS1WhiteImg = new CImage
	(
		"UI\\white.png",
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);
}

CGameSceneUI::~CGameSceneUI()
{
	// 読み込んだイメージを削除
	SAFE_DELETE(mpHpGaugeImg);
	SAFE_DELETE(mpHpWhiteImg);

	SAFE_DELETE(mpStGaugeImg);
	SAFE_DELETE(mpStWhiteImg);

	SAFE_DELETE(mpS1IconImg);
	SAFE_DELETE(mpS1WhiteImg);
}

// HP最大値を設定
void CGameSceneUI::SetHpMaxPoint(float point)
{
	mHpMaxPoint = point;
	ApplyPoint(0);
}

// HP現在値を設定
void CGameSceneUI::SetHpCurrPoint(float point)
{
	mHpCurrPoint = point;
	ApplyPoint(0);
}

// ポイント残量の割合を設定（0.0～1.0）
void CGameSceneUI::SetHpPercent(float per)
{
	mHpPercent = Math::Clamp01(per);
}



// ST最大値を設定
void CGameSceneUI::SetStMaxPoint(float point)
{
	mStMaxPoint = point;
	ApplyPoint(1);
}

// ST現在値を設定
void CGameSceneUI::SetStCurrPoint(float point)
{
	mStCurrPoint = point;
	ApplyPoint(1);
}

// ポイント残量の割合を設定（0.0～1.0）
void CGameSceneUI::SetStPercent(float per)
{
	mStPercent = Math::Clamp01(per);
}



// S1最大値を設定
void CGameSceneUI::SetS1MaxPoint(float point)
{
	mS1MaxPoint = point;
	ApplyPoint(2);
}

// S1現在値を設定
void CGameSceneUI::SetS1CurrPoint(float point)
{
	mS1CurrPoint = point;
	ApplyPoint(2);
}

// ポイント残量の割合を設定（0.0～1.0）
void CGameSceneUI::SetS1Percent(float per)
{
	mS1Percent = Math::Clamp01(per);
}


void CGameSceneUI::Update()
{
	SetHpCurrPoint(mpPlayer->GetHp());
	mpHpGaugeImg->Update();
	mpHpWhiteImg->Update();

	SetStCurrPoint(mpPlayer->GetSt());
	mpStGaugeImg->Update();
	mpStWhiteImg->Update();

	SetS1CurrPoint(mpPlayer->GetS1());
	mpS1IconImg->Update();
	mpS1WhiteImg->Update();
	
	Render();
}

void CGameSceneUI::Render()
{
	// ゲージ背景を描画
	mpHpWhiteImg->SetPos(mHpGaugePos);
	mpHpWhiteImg->SetSize(mGaugeSize);
	mpHpWhiteImg->SetColor(CColor::black);
	mpHpWhiteImg->SetAlpha(0.5f);
	mpHpWhiteImg->Render();

	mpStWhiteImg->SetPos(mStGaugePos);
	mpStWhiteImg->SetSize(mGaugeSize);
	mpStWhiteImg->SetColor(CColor::black);
	mpStWhiteImg->SetAlpha(0.5f);
	mpStWhiteImg->Render();

	// HPゲージ
	// バーのサイズ、座標、色を
	// ポイント残量の割合に合わせて調整して、バーを描画
	// バーのサイズを調整
	CVector2 barSize = mGaugeSize;
	barSize.X(barSize.X() * mHpPercent);
	mpHpWhiteImg->SetSize(barSize);
	// バーの座標を調整
	CVector2 barPos = mGaugeSize - barSize;
	//mpHpWhiteImg->SetPos(-barPos + mHpGaugePos);<-ゲージが逆なら？
	mpHpWhiteImg->SetPos(mHpGaugePos);
	// バーの色を設定
	CColor barColor = CColor::green;
	if (mHpPercent <= 0.25f) barColor = CColor::red;
	else if (mHpPercent <= 0.5f)barColor = CColor::yellow;
	mpHpWhiteImg->SetColor(barColor);

	// STゲージ
	// バーのサイズを調整
	barSize = mGaugeSize;
	barSize.X(barSize.X() * mStPercent);
	mpStWhiteImg->SetSize(barSize);
	// バーの座標を調整
	barPos = mGaugeSize - barSize;
	//mpStWhiteImg->SetPos(-barPos + mStGaugePos);<-ゲージが逆なら？
	mpStWhiteImg->SetPos(mStGaugePos);
	// バーの色を設定
	barColor = CColor::skyBlue;
	mpStWhiteImg->SetColor(barColor);

	// バーを描画
	mpHpWhiteImg->Render();
	mpStWhiteImg->Render();
	// ゲージ本体を描画
	mpHpGaugeImg->Render();
	mpStGaugeImg->Render();

	// ------スキル関連------
	
	// スキル1ゲージ
	
	// ゲージ背景を描画
	mpS1IconImg->Render();

	// フィルターの色を設定
	mpS1WhiteImg->SetPos(mS1GaugePos);
	mpS1WhiteImg->SetSize(mSkillGaugeSize);
	mpS1WhiteImg->SetColor(CColor::black);
	if (mS1Percent < 1.0f)
	{
		// フィルターを描画
		mpS1WhiteImg->SetAlpha(0.5f);
		mpS1WhiteImg->Render();
	}
	else if (mS1Percent >= 1.0f)
	{
		// フィルターを描画
		mpS1WhiteImg->SetAlpha(0.0f);
		mpS1WhiteImg->Render();
	}

	// バーのサイズ、座標、色を
	// ポイント残量の割合に合わせて調整して、バーを描画
	// バーのサイズを調整
	barSize = mSkillGaugeSize;
	barSize.Y(barSize.Y() * mS1Percent);
	mpS1WhiteImg->SetSize(barSize);
	// バーの座標を調整
	barPos = mSkillGaugeSize - barSize;
	mpS1WhiteImg->SetPos(barPos + mS1GaugePos);
	// バーの色を設定
	if (mS1Percent < 1.0f)
	{
		barColor = CColor::gray;
		mpS1WhiteImg->SetColor(barColor); // SetColorはRGBAを管理し、Aも書き換えるので注意
		mpS1WhiteImg->SetAlpha(0.5f);
	}
	else if(mS1Percent >= 1.0f)
	{
		barColor = CColor::white;
		mpS1WhiteImg->SetColor(barColor);
		mpS1WhiteImg->SetAlpha(0.0f);
	}
	// バーを描画
	mpS1WhiteImg->Render();
}

// ポイント残量を反映
void CGameSceneUI::ApplyPoint(int num)
{
	switch (num)
	{
	case 0:
		// HP最大値が不正値でなければ
		if (mHpMaxPoint > 0)
		{
			// 現在値が最大値の何パーセントか求める
			mHpPercent = Math::Clamp01((float)mHpCurrPoint / mHpMaxPoint);
		}
		// 不正値ならば、100%固定
		else
		{
			mHpPercent = 1.0f;
		}
		break;
	case 1:
		// ST最大値が不正値でなければ
		if (mStMaxPoint > 0)
		{
			// 現在値が最大値の何パーセントか求める
			mStPercent = Math::Clamp01((float)mStCurrPoint / mStMaxPoint);
		}
		// 不正値ならば、100%固定
		else
		{
			mStPercent = 1.0f;
		}
		break;
	case 2:
		// S1最大値が不正値でなければ
		if (mS1MaxPoint > 0)
		{
			// 現在値が最大値の何パーセントか求める
			mS1Percent = Math::Clamp01((float)mS1CurrPoint / mS1MaxPoint);
		}
		// 不正値ならば、100%固定
		else
		{
			mS1Percent = 1.0f;
		}
		break;
	default:
		break;
	}
	
}