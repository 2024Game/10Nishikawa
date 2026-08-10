#include "CTargetUI.h"
#include "CImage.h"
#include "Maths.h"

#define DIST_MIN 10.0f
#define SCALE_MIN 0.05f
#define SCALE_MAX 0.25f

CTargetUI::CTargetUI()
	:mpTargetImg(nullptr)
	, mImageBaseSize(0.0f,0.0f)
	, mTargetPos(0.0f, 0.0f, 0.0f)
{
	mpTargetImg = new CImage
	(
		"UI\\target.png",
		ETaskPriority::eUI, 0,
		ETaskPauseType::eGame,
		false, false
	);

	mImageBaseSize = mpTargetImg->GetSize();

	mpTargetImg->SetAlpha(0.6f);

	SetShow(false);
}

CTargetUI::~CTargetUI()
{
	SAFE_DELETE(mpTargetImg)
}

void CTargetUI::SetTargetPos(const CVector& pos)
{
	mTargetPos = pos;
}

void CTargetUI::Update()
{
}

void CTargetUI::Render()
{
	CCamera* camera = CCamera::CurrentCamera();
	if (camera == nullptr) return;

	// カメラの後ろ側なら、描画しない
	CVector spos = camera->WorldToScreenPos(mTargetPos);
	if (spos.Z() < 0.0f) return;

	// カメラから離れるとスケール値が小さくなる
	CVector cameraPos = camera->Position();
	float dist = CVector::Distance(cameraPos, mTargetPos);
	float scale = Math::Clamp(DIST_MIN / dist, SCALE_MIN, SCALE_MAX);

	CVector2 size = mImageBaseSize * scale;
	mpTargetImg->SetPos(spos);
	mpTargetImg->SetSize(size);
	mpTargetImg->SetCenter(size * 0.5f);
	mpTargetImg->Render();
}
