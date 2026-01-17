#include "CItem.h"

#define GRAVITY 0.0625f

// コンストラクタ
CItem::CItem()
	: CObjectBase(ETag::eItem, ETaskPriority::eItem, 0, ETaskPauseType::eGame)
	, mpAttachMtx(nullptr)
	, mpCollider(nullptr)
	, mIsGravity(true)
	, mMoveSpeedY(0.0f)
{
	// 最初はコライダーオン
	SetEnableCol(true);
}

// デストラクタ
CItem::~CItem()
{
	// コライダーを削除
	SAFE_DELETE(mpCollider);
}

// アイテムをアタッチする行列を設定
void CItem::SetAttachMtx(const CMatrix* mtx)
{
	mpAttachMtx = mtx;
}

// 行列を取得
CMatrix CItem::Matrix() const
{
	CMatrix m = CTransform::Matrix();
	// くっつける行列が設定されていれば
	if (mpAttachMtx != nullptr)
	{
		// その行列にくっつける
		m = m * mAttachMtx;
	}
	return m;
}

// 衝突処理
void CItem::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 本体が衝突した
	if (self == mpCollider)
	{
		// フィールドと衝突した
		if (other->Layer() == ELayer::eField)
		{
			// 坂道で滑らないように、押し戻しベクトルのXとZの値を0にする
			CVector adjust = hit.adjust;
			adjust.X(0.0f);
			adjust.Z(0.0f);

			Position(Position() + adjust * hit.weight);

			// 衝突した地面が床か天井かを内積で判定
			CVector normal = hit.adjust.Normalized();
			float dot = CVector::Dot(normal, CVector::up);
			// 内積の結果がプラスであれば、床と衝突した
			if (dot >= 0.0f)
			{
				// 落下などで床に上から衝突した時（下移動）のみ
				// 上下の移動速度を0にする
				if (mMoveSpeedY < 0.0f)
				{
					mMoveSpeedY = 0.0f;
				}

				// 接地した
				mIsGrounded = true;
				// 接地した地面の法線を記憶しておく
				//mGroundNormal = hit.adjust.Normalized();
			}
			// 内積の結果がマイナスであれば、天井と衝突した
			else
			{
				// ジャンプなどで天井に下から衝突した時（上移動）のみ
				// 上下の移動速度を0にする
				if (mMoveSpeedY > 0.0f)
				{
					mMoveSpeedY = 0.0f;
				}
			}
		}
		// プレイヤーと衝突した場合
		else if (other->Layer() == ELayer::ePlayer)
		{
			// 横方向にのみ押し戻すため、
			// 押し戻しベクトルのYの値を0にする
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);
			Position(Position() + adjust * hit.weight);
		}
		// プレイヤーと衝突した場合
		else if (other->Layer() == ELayer::eEnemy)
		{
			// 横方向にのみ押し戻すため、
			// 押し戻しベクトルのYの値を0にする
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);
			Position(Position() + adjust * hit.weight);
		}
	}
}

// コライダーを取得
CCollider* CItem::Collider() const
{
	return mpCollider;
}

void CItem::Update()
{
	if (mIsGravity)
	{
		// 重力
		mMoveSpeedY -= GRAVITY;
	}

	// 移動
	CVector moveSpeed = CVector(0.0f, mMoveSpeedY, 0.0f);
	Position(Position() + moveSpeed);

	mIsGrounded = false;
}

// 行列を更新
void CItem::UpdateMtx()
{
	// くっつける行列が設定されていれば、
	if (mpAttachMtx != nullptr)
	{
		// くっつける行列をメンバ変数にコピー
		mAttachMtx = *mpAttachMtx;
	}
}