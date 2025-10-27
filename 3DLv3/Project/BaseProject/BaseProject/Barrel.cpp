#include "Barrel.h"
#include "CTrailEffect.h"
#include "Primitive.h"

// コンストラクタ
CBarrel::CBarrel(const CVector& pos, const CVector& dir,
	float speed, float distance)
	: CObjectBase(ETag::eBullet, ETaskPriority::eDefault, 0, ETaskPauseType::eGame)
	, mMoveSpeed(speed)
	, mFlyingDistance(distance)
	, mCurrentFlyingDistance(0.0f)
{
	Position(pos);
	Rotation(CQuaternion::LookRotation(dir, CVector::up));
}

// デストラクタ
CBarrel::~CBarrel()
{
	
}

// 更新
void CBarrel::Update()
{
	if (IsKill()) return;

	// 残り飛距離が0ならば、弾丸削除
	float remain = mFlyingDistance - mCurrentFlyingDistance;
	if (remain <= 0.0f)
	{
		Kill();
		return;
	}

	// 移動速度を計算
	// 移動速度が残りの飛距離より大きい場合は、
	// 残りの飛距離を移動速度とする
	float moveSpeed = mMoveSpeed * Times::DeltaTime();
	if (abs(moveSpeed) > remain)
	{
		moveSpeed = remain * (moveSpeed < 0.0f ? -1.0f : 1.0f);
	}

	// 弾丸を正面方向に移動
	Position(Position() + VectorZ() * moveSpeed);
	// 現在の飛距離を更新
	mCurrentFlyingDistance += abs(moveSpeed);
}

// 描画
void CBarrel::Render()
{
	Primitive::DrawSphere(Matrix(), 0.1f, mColor);
}