//プレイヤークラスのインクルード
#include "CPlayer.h"
#include "CInput.h"
#include "CCamera.h"
#include "Maths.h"
#include "CColliderCapsule.h"
#include "CColliderSphere.h"
#include "CBarrel.h"

#define BODY_HEIGHT 15.0f	// 本体のコライダーの高さ
#define BODY_RADIUS 2.5f	// 本体のコライダーの幅
#define MOVE_SPEED  0.5f	// 移動速度

#define BARREL_OFFSET_POS CVector(0.0f, 1.5f, -8.5f)

// プレイヤーのインスタンス
CPlayer* CPlayer::spInstance = nullptr;

// コンストラクタ
CPlayer::CPlayer()
	: CCharaBase(ETag::ePlayer, ETaskPriority::ePlayer)
	, mState(EState::eMovable)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mMoveSpeedY(0.0f)
	,mpModel(nullptr)
	,mpBodyCol(nullptr)
{
	mMaxHp = 100000;
	mHp = mMaxHp;

	//インスタンスの設定
	spInstance = this;

	// モデルデータ取得
	mpModel = CResourceManager::Get<CModel>("Player");

	// 本体のコライダーを作成
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::ePlayer,
		CVector(0.0f, BODY_RADIUS, BODY_RADIUS - 7.0f),
		CVector(0.0f, BODY_RADIUS, BODY_HEIGHT - BODY_RADIUS - 7.0f),
		BODY_RADIUS
	);
	mpBodyCol->SetCollisionTags({ ETag::eField, ETag::eRideableObject, ETag::eEnemy });
	mpBodyCol->SetCollisionLayers({ ELayer::eField, ELayer::eEnemy, ELayer::eAttackCol });
}

CPlayer::~CPlayer()
{
	// コライダーを削除
	SAFE_DELETE(mpBodyCol);
}

CPlayer* CPlayer::Instance()
{
	return spInstance;
}

// 状態を切り替え
void CPlayer::ChangeState(EState state)
{
	if (mState == state) return;

	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}

// 待機
void CPlayer::UpdateIdle()
{
	
}

// 仰け反り
void CPlayer::UpdateHit()
{
	switch (mStateStep)
	{
		case 0:
			mStateStep++;
			break;
		case 1:
			// 待機状態へ移行
			ChangeState(EState::eIdle);
			break;
	}
}

// オブジェクト削除を伝える
void CPlayer::DeleteObject(CObjectBase* obj)
{
	
}

// キーの入力情報から移動ベクトルを求める
CVector CPlayer::CalcMoveVec() const
{
	CVector move = CVector::zero;

	// キーの入力ベクトルを取得
	CVector input = CVector::zero;
	if (CInput::Key('W'))		input.Y(-1.0f);
	else if (CInput::Key('S'))	input.Y(1.0f);
	if (CInput::Key('A'))		input.X(-1.0f);
	else if (CInput::Key('D'))	input.X(1.0f);

	// 入力ベクトルの長さで入力されているか判定
	if (input.LengthSqr() > 0.0f)
	{
		// 上方向ベクトル
		CVector up = CVector::up;
		// カメラの向きに合わせた移動ベクトルに変換
		CCamera* mainCamera = CCamera::MainCamera();
		CVector camForward = mainCamera->VectorZ();
		camForward.Y(0.0f);
		camForward.Normalize();
		// カメラの正面方向ベクトルと上方向ベクトルの外積から
		// 横方向の移動ベクトルを求める
		CVector moveSide = CVector::Cross(up, camForward);
		// 横方向の移動ベクトルと上方向ベクトルの外積から
		// 正面方向の移動ベクトルを求める
		CVector moveForward = CVector::Cross(moveSide, up);

		// 求めた各方向の移動ベクトルから、
		// 最終的なプレイヤーの移動ベクトルを求める
		move = moveForward * input.Y() + moveSide * input.X();
		move.Normalize();
	}

	return move;
}

// 移動の更新処理
void CPlayer::UpdateMove()
{
	mMoveSpeed = CVector::zero;

	// プレイヤーの移動ベクトルを求める
	CVector move = CalcMoveVec();
	// 求めた移動ベクトルの長さで入力されているか判定
	if (move.LengthSqr() > 0.0f)
	{
		mMoveSpeed += move * MOVE_SPEED;
	}
}

// 更新
void CPlayer::Update()
{
	// 状態に合わせて、更新処理を切り替える
	switch (mState)
	{
		// 待機状態
		case EState::eIdle:			UpdateIdle();		break;
		// 仰け反り
		case EState::eHit:			UpdateHit();		break;
	}

	// 待機中とは、移動処理を行う
	if (mState == EState::eMovable)
	{
		UpdateMove();
	}

	// 移動
	Position(Position() + mMoveSpeed);

	// ホイールクリックで弾丸発射
	if (CInput::PushKey(VK_MBUTTON) && mState == EState::eMovable)
	{
		ChangeState(EState::eIdle);
		DropBarrel();
	}

	// プレイヤーを移動方向へ向ける
	CVector current = VectorZ();
	CVector target = mMoveSpeed;
	target.Y(0.0f);
	target.Normalize();
	CVector forward = CVector::Slerp(current, target, 0.125f);
	Rotation(CQuaternion::LookRotation(forward));

	CVector pos = Position();
	CDebugPrint::Print("PlayerHP:%d / %d\n", mHp, mMaxHp);
	CDebugPrint::Print("PlayerPos:%.2f, %.2f, %.2f\n", pos.X(), pos.Y(), pos.Z());
	CDebugPrint::Print("PlayerState:%d\n", mState);

	CDebugPrint::Print("FPS:%f\n", Times::FPS());
}

// 樽を発射
void CPlayer::DropBarrel()
{
	CVector pos = Position() + Rotation() * BARREL_OFFSET_POS;
	CVector under = -VectorY();
	CVector dir = CQuaternion(0.0f, 0.0f, 0.0f) * under;
	CBarrel* barrel = new CBarrel(3, 50, this, mpCamera);
	barrel->Position(pos);
	barrel->Rotation(CQuaternion::LookRotation(dir));

	// カメラの追従を樽に移す
	mpCamera->SetFollowTargetTf(barrel);
	mpCamera->SetFollowTargetOffset(CVector(0.0f, 0.0f, 0.0f));

	// 移動を停止
	mMoveSpeed = CVector::zero;
}

// ダメージを受ける
void CPlayer::TakeDamage(int damage, CObjectBase* causer)
{
	// ベースクラスのダメージ処理を呼び出す
	CCharaBase::TakeDamage(damage, causer);

	// 死亡していなければ、
	if (!IsDeath())
	{
		// 仰け反り状態へ移行
		ChangeState(EState::eHit);

		// 移動を停止
		mMoveSpeed = CVector::zero;
	}
}

// 衝突処理
void CPlayer::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 本体のコライダーの衝突判定
	if (self == mpBodyCol)
	{
		// フィールドとの衝突
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
		// 敵と衝突した場合
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

// 描画
void CPlayer::Render()
{
	mpModel->Render(Matrix());
}

void CPlayer::SetCamera(CGameCamera2* camera)
{
	mpCamera = camera;
}

void CPlayer::SetState(int stateNum)
{
	if (stateNum == 0)
	{
		ChangeState(EState::eIdle);
	}
	else if (stateNum == 1)
	{
		ChangeState(EState::eMovable);
	}
}
