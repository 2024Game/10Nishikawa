#include "CTuna.h"
#include "CColliderCapsule.h"
#include "Maths.h"
#include <iostream>

#define ANIM_PATH "Character\\Enemy\\Tuna\\Anim\\"
#define BODY_HEIGHT 36.0f
#define BODY_RADIUS 4.5f
#define BODY_COL_OFFSET 19.0f
#define GAUGE_OFFSET_Y 8.0f

#define MOVE_SPEED 12.0f
#define LOOKAT_SPEED 0.5f
#define IDLE_TIME_MIN 5.0f
#define IDLE_TIME_MAX 15.0f

#define DEATH_WAIT_TIME 2.0f

CTuna::CTuna()
	:CFish("Tuna")
{
	// 敵のアニメーションデータのテーブル
	const std::vector<CEnemy::AnimData> ANIM_DATA =
	{
		{ "",					true,	0.0f,	1.0f},	// Tポーズ
		{ ANIM_PATH"Tuna.x",	true,	31.0f,	1.0f},	// 待機
	};

	// ゲージのオフセット位置を設定
	mGaugeOffsetPos = CVector(0.0f, GAUGE_OFFSET_Y, 0.0f);

	// 敵を初期化
	InitEnemy("Tuna", &ANIM_DATA);

	// 最初は待機アニメーションを再生
	ChangeAnimation((int)EAnimType::eIdle);

	// 本体のコライダーを作成
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::eEnemy,
		CVector(0.0f, 0.0f, BODY_RADIUS - BODY_COL_OFFSET),
		CVector(0.0f, 0.0f, BODY_HEIGHT - BODY_RADIUS - BODY_COL_OFFSET),
		BODY_RADIUS
	);

	// フィールドと、プレイヤーの攻撃コライダーとヒットするように設定
	mpBodyCol->SetCollisionTags({ ETag::ePlayer });
	mpBodyCol->SetCollisionLayers({ ELayer::ePlayer, ELayer::eAttackCol });
	mMaxHp = 2;
	mHp = mMaxHp;
}

CTuna::~CTuna()
{
}

void CTuna::LookAtTargetPos()
{
	// 目的地までの方向ベクトルを求める
	CVector targetPos = mTargetPos;
	CVector vec = targetPos - Position();

	// Y成分を無視して水平面上の方向ベクトルにする
	vec.Y(0.0f);
	vec.Normalize();

	// 徐々に目的地の方向へ向く
	CVector forward = CVector::Slerp
	(
		VectorZ(), vec,
		LOOKAT_SPEED * Times::DeltaTime()
	);
	Rotation(CQuaternion::LookRotation(forward));
}

void CTuna::ChangeState(int state)
{
	// 状態切り替え
	CEnemy::ChangeState(state);
}

void CTuna::UpdateIdle()
{
	// ステップごとに処理を切り替え
	switch (mStateStep)
	{
		// ステップ0：待機時間をランダムで決定
	case 0:
		// 待機時間が決まってなければ
		if (mIdletime <= 0.0f)
		{
			// ランダムで待機時間を決定
			mIdletime = Math::Rand
			(
				IDLE_TIME_MIN,
				IDLE_TIME_MAX
			);
		}
		mStateStep++;
		break;
		// ステップ1：待機時間の経過待ち
	case 1:
		// 戦闘時の待機時間待ち
		if (mElapsedTime < mIdletime)
		{
			mElapsedTime += Times::DeltaTime();
		}
		// 待機時間が経過した
		else
		{
			// 次の状態（デフォルトは移動状態）
			EState nextState = EState::eMove;

			// 目的地までの距離を求める
			CVector targetPos = mTargetPos;
			CVector vec = targetPos - Position();

			float dist = vec.Length();

			// ランダムな座標を設定
			float randX = Math::Rand(-500.0f, 500.0f);
			float randY = Math::Rand(-175.0f, -125.0f);
			float randZ = Math::Rand(-500.0f, 500.0f);
			mTargetPos = CVector(randX, randY, randZ);

			// 次の状態へ移行
			ChangeState((int)nextState);

			// 戦闘待機時間を初期化
			mIdletime = 0.0f;
		}
		break;
	}
}

void CTuna::UpdateMove()
{
	LookAtTargetPos();

	mMoveSpeed = CVector::zero;

	// 現在地と目的地を取得
	CVector pos = Position();
	CVector targetPos = mTargetPos;

	// 現在地から目的地までのベクトルを求める
	CVector vec = targetPos - pos;

	// 残り距離が移動距離より大きい場合は、移動距離分移動
	CVector dir = vec.Normalized();
	float moveDist = MOVE_SPEED * Times::DeltaTime();
	float dist = vec.Length();

	//CDebugPrint::Print("FishDist:%.2f\n", dist);

	if (dist > moveDist)
	{
		mMoveSpeed = dir * moveDist;
	}
	// 残り距離の方が小さい場合は、
	// 残り距離分移動して、待機状態へ移行
	else
	{
		CDebugPrint::Print("EState::eIdle\n");
		mMoveSpeed = dir * dist;
		ChangeState((int)EState::eIdle);
		mMoveSpeed = CVector::zero;
	}
}

void CTuna::UpdateHit()
{

}

void CTuna::UpdateDeath()
{
	// ステップごとに処理を分ける
	switch (mStateStep)
	{
		// ステップ0：死亡アニメーション再生
	case 0:
		mStateStep++;
		break;
		// ステップ1：アニメーション終了待ち
	case 1:
		mStateStep++;
		break;
		// ステップ2：死亡後の待ち
	case 2:
		if (mElapsedTime < DEATH_WAIT_TIME)
		{
			mElapsedTime += Times::DeltaTime();
		}
		// 待ち時間が終了したら、削除
		else
		{
			Kill();
		}
		break;
	}
}