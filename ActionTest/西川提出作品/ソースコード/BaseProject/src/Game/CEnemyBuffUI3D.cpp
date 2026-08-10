#include "CEnemyBuffUI3D.h"
#include "CCamera.h"
#include <glut.h>
#include "CImage3D.h"
#include "Maths.h"

#define WORLD_UNIT_PER_PIXEL 32.0f

// コンストラクタ
CEnemyBuffUI3D::CEnemyBuffUI3D(CObjectBase* owner)
	: CObjectBase(ETag::eUI, ETaskPriority::eUI3D, 0, ETaskPauseType::eGame)
	, mpOwner(owner)
	, mpIconImg(nullptr)
	, mpFilterImg(nullptr)
	, mMaxPoint(100)
	, mCurrPoint(mMaxPoint)
	, mPercent(1.0f)
{
	// ゲージのイメージを読み込み
	mpIconImg = new CImage3D
	(
		"UI\\GuardBreak.png",
		ETag::eUI,
		ETaskPriority::eUI3D, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mpIconImg->SetWorldUnitPerPixel(WORLD_UNIT_PER_PIXEL);
	//mpIconImg->SetDepthMask(true); <- trueにすると描画の順番が変になった
	mpIconImg->SetDepthMask(false);
	mIconSize = mpIconImg->GetSize() * 1.8f;

	// 白イメージを読み込み
	mpFilterImg = new CImage3D
	(
		"UI\\white.png",
		ETag::eUI,
		ETaskPriority::eUI3D, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mpFilterImg->SetWorldUnitPerPixel(WORLD_UNIT_PER_PIXEL);
}

// デストラクタ
CEnemyBuffUI3D::~CEnemyBuffUI3D()
{
	// 読み込んだイメージを削除
	SAFE_DELETE(mpIconImg);
	SAFE_DELETE(mpFilterImg);

	// 持ち主に削除されたことを伝える
	if (mpOwner != nullptr)
	{
		mpOwner->DeleteObject(this);
	}
}

// 持ち主を設定
void CEnemyBuffUI3D::SetOwner(CObjectBase* owner)
{
	mpOwner = owner;
}

// ポイント残量を反映
void CEnemyBuffUI3D::ApplyPoint()
{
	// 最大値が不正値でなければ
	if (mMaxPoint > 0)
	{
		// 最大値-現在値(つまり経過した分)が最大値の何パーセントか求める
		mPercent = 1.0f - (Math::Clamp01((float)mCurrPoint / mMaxPoint));
	}
	// 不正値ならば、0%固定
	else
	{
		mPercent = 0.0f;
	}
}

// 最大値を設定
void CEnemyBuffUI3D::SetMaxPoint(float point)
{
	mMaxPoint = point;
	ApplyPoint();
}

// 現在値を設定
void CEnemyBuffUI3D::SetCurrPoint(float point)
{
	mCurrPoint = point;
	ApplyPoint();
}

// ポイント残量の割合を設定（0.0～1.0）
void CEnemyBuffUI3D::SetPercent(float per)
{
	mPercent = Math::Clamp01(per);
}

// 更新
void CEnemyBuffUI3D::Update()
{
	mpIconImg->Update();
	mpFilterImg->Update();
}

// 描画
void CEnemyBuffUI3D::Render()
{
	if (mCurrPoint <= 0) return;
	// 行列の保存
	glPushMatrix();

	// 常にカメラの方向を向かせる
	CCamera* cam = CCamera::CurrentCamera();
	CMatrix m = cam->GetViewMatrix().Inverse();
	m.Position(CVector::zero);
	glMultMatrixf((m * Matrix()).M());

	// アイコン本体を描画
	mpIconImg->Render();
	mpIconImg->SetSize(mIconSize);

	// バーのサイズ、座標、色を
	// ポイント残量の割合に合わせて調整して、バーを描画
	// バーのサイズを調整
	CVector2 barSize = mIconSize;
	barSize.Y(barSize.Y() * mPercent);
	mpFilterImg->SetSize(barSize);
	// バーの座標を調整
	CVector2 barPos = mIconSize - barSize;
	mpFilterImg->SetOffsetPos(barPos);
	// バーの色を設定
	mBarColor = CColor::black;
	mpFilterImg->SetColor(mBarColor);
	mpFilterImg->SetAlpha(0.5f);
	// バーを描画
	mpFilterImg->Render();

	// 行列を戻す
	glPopMatrix();
}