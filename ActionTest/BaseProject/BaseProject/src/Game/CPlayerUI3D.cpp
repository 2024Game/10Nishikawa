#include "CPlayerUI3D.h"
#include "CCamera.h"
#include <glut.h>
#include "CImage3D.h"
#include "Maths.h"

#define WORLD_UNIT_PER_PIXEL 32.0f

CPlayerUI3D::CPlayerUI3D(CObjectBase* owner)
	: CObjectBase(ETag::eUI, ETaskPriority::eUI3D, 0, ETaskPauseType::eGame)
	, mpOwner(owner)
	, mpWhiteImg(nullptr)
	, mIsShow(false)
{
	// 白イメージを読み込み
	mpWhiteImg = new CImage3D
	(
		"UI\\Danger.png",
		ETag::eUI,
		ETaskPriority::eUI3D, 0,
		ETaskPauseType::eGame,
		false, false
	);
	mpWhiteImg->SetWorldUnitPerPixel(WORLD_UNIT_PER_PIXEL);
	mpWhiteImg->SetDepthMask(true);
	mImageSize = mpWhiteImg->GetSize() * 0.1f;
}

CPlayerUI3D::~CPlayerUI3D()
{
	// 読み込んだイメージを削除
	SAFE_DELETE(mpWhiteImg);

	// 持ち主に削除されたことを伝える
	if (mpOwner != nullptr)
	{
		mpOwner->DeleteObject(this);
	}
}

void CPlayerUI3D::SetOwner(CObjectBase* owner)
{
	mpOwner = owner;
}

void CPlayerUI3D::SetShow(bool show)
{
	mIsShow = show;
}

void CPlayerUI3D::Update()
{
	if (!mIsShow) return;

	mpWhiteImg->Update();
}

void CPlayerUI3D::Render()
{
	if (!mIsShow) return;

	// 行列の保存
	glPushMatrix();

	// 常にカメラの方向を向かせる
	CCamera* cam = CCamera::CurrentCamera();
	CMatrix m = cam->GetViewMatrix().Inverse();
	m.Position(CVector::zero);
	glMultMatrixf((m * Matrix()).M());

	// インジケーターを描画
	mpWhiteImg->SetOffsetPos(CVector2::zero);
	mpWhiteImg->SetSize(mImageSize);
	mpWhiteImg->SetColor(CColor::white);
	mpWhiteImg->Render();

	// 行列を戻す
	glPopMatrix();
}
