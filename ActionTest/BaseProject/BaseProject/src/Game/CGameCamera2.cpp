#include "CGameCamera2.h"
#include "CInput.h"
#include "Maths.h"

// カメラの回転速度
#define ROTATE_SPEED 0.05f
// カメラの上下回転の範囲
#define ROTATE_RANGE_X 45.0f

// ロックオン時のカメラのX軸の角度
#define LOCKON_ROT_X 20.0f
// ロックオン時のカメラの回転速度
#define LOCKON_SPEED 6.0f

// コンストラクタ
CGameCamera2::CGameCamera2(const CVector& eye, const CVector& center, bool isMainCamera)
	: CCamera(eye, center, isMainCamera)
	, mFollowDefaultEyeVec(CVector::forward)
	, mRotateAngle(CVector::zero)
{
}

// デストラクタ
CGameCamera2::~CGameCamera2()
{
}

void CGameCamera2::SetFollowTargetTf(CTransform* target)
{
	mFollowTargetTf = target;
	if (mFollowTargetTf != nullptr)
	{
		mFollowDefaultEyeVec = mTargetEye - mAt;
		mFollowOffsetPos = mAt - mFollowTargetTf->Position();
	}
}

void CGameCamera2::LookAt(const CVector& eye, const CVector& at, const CVector& up, bool updateTargetEye)
{
	CCamera::LookAt(eye, at, up, updateTargetEye);
	if (mFollowTargetTf != nullptr)
	{
		if (updateTargetEye)
		{
			mFollowDefaultEyeVec = mTargetEye - mAt;
		}
		mFollowOffsetPos = mAt - mFollowTargetTf->Position();
	}
}

CVector CGameCamera2::GetRotateAngle()
{
	return mRotateAngle;
}

// 後更新
void CGameCamera2::LateUpdate()
{
	// ロックオンしていない
	if (mpLockOnTarget == nullptr)
	{
		// マウスの移動量に合わせて、カメラの回転角度を変更
		CVector2 delta = CInput::GetDeltaMousePos();
		float x = Math::Clamp(mRotateAngle.X() + delta.Y() * ROTATE_SPEED, -ROTATE_RANGE_X, ROTATE_RANGE_X);
		float y = Math::Repeat(mRotateAngle.Y() + delta.X() * ROTATE_SPEED, 360.0f);
		mRotateAngle.X(x);
		mRotateAngle.Y(y);
	}
	// ロックオンしている
	else
	{
		CVector targetRotAng = mRotateAngle;

		targetRotAng.X(LOCKON_ROT_X);

		// Y軸の回転角度は、自身の座標から対象の座標までのベクトルから求める
		CVector selfPos = mFollowTargetTf != nullptr ? mFollowTargetTf->Position() : Position();
		CVector targetPos = mpLockOnTarget->Position() + mpLockOnTarget->GetLockOnOffsetPos();
		targetRotAng.Y(CVector::AngleY(targetPos - selfPos));

		mRotateAngle = CVector::LerpAngle
		(
			mRotateAngle, targetRotAng,
			LOCKON_SPEED * Times::DeltaTime()
		);
	}

	// 追従するターゲットが設定されていれば、
	if (mFollowTargetTf != nullptr)
	{
		// 回転値を求めて、注視点から視点までのベクトルを回転させることで、
		// 視点の位置を更新する
		CQuaternion rot = CQuaternion(mRotateAngle);
		mAt = mFollowTargetTf->Position() + mFollowOffsetPos;
		mTargetEye = mAt + rot * mFollowDefaultEyeVec;
		mEye = mTargetEye;
	}

#ifdef _DEBUG
	CDebugPrint::Print("Angle:%f, %f, %f\n", mRotateAngle.X(), mRotateAngle.Y(), mRotateAngle.Z());
#endif // _DEBUG

	// 設定されているコライダーと衝突する場合は、
	// カメラの位置を押し出す
	ApplyCollision();

	// 視点、注視点、上ベクトルから各行列を更新
	LookAt(mEye, mAt, mUp, false);

	// カメラのベースの更新処理
	//CCamera::Update();
}
