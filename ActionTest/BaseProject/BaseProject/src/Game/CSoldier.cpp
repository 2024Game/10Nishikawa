#include "CSoldier.h"
#include "CColliderCapsule.h"
#include "CColliderSphere.h"
#include "CSword.h"
#include "Maths.h"

// アニメーションのパス
#define ANIM_PATH "Character\\TestPlayer\\Anims\\"
#define BODY_HEIGHT 16.0f	// 本体のコライダーの高さ
#define BODY_RADIUS 3.0f	// 本体のコライダーの幅
#define MOVE_SPEED 20.0f		// 移動速度
#define RUN_SPEED 50.0f		// 移動速度
#define JUMP_SPEED 1.5f		// ジャンプ速度
#define GRAVITY 0.0625f		// 重力加速度

#define GAUGE_OFFSET_Y 15.0f
#define DEATH_WAIT_TIME 3.0f

#define LOOKAT_SPEED 90.0f
#define BATTLE_IDLE_TIME_MIN 0.1f
#define BATTLE_IDLE_TIME_MAX 0.5f
#define ATTACK2_DIST 50.0f			// 駆け寄ってくる距離
#define ATTACK_RANGE 30.0f			// 攻撃を行う距離
#define ATTACK2_PROB 75				// 2段目攻撃を行う確率（パーセント）
#define ATTACKX_PROB 50				// X段目攻撃を行う確率（パーセント）

#define ATTACK1_START_FRAME 25.0f	// 斬り攻撃1の開始フレーム
#define ATTACK1_END_FRAME 55.0f		// 斬り攻撃1の終了フレーム
#define ATTACK2_START_FRAME 35.0f	// 斬り攻撃2の開始フレーム
#define ATTACK2_END_FRAME 100.0f	// 斬り攻撃2の終了フレーム
#define ATTACKX_START_FRAME 35.0f	// 斬り攻撃Xの開始フレーム
#define ATTACKX_END_FRAME 210.0f	// 斬り攻撃Xの終了フレーム


// 剣のオフセット座標
#define SWORD_OFFSET_POS CVector(0.0f, 7.2f, 3.5f)
// 剣のオフセット向き
#define SWORD_OFFSET_ROT CVector(20.0f, 0.0f, -30.0f)

#define ATTACKX_SWORD_OFFSET_ROT CVector(-20.0f, 0.0f, -25.0f)
// ダッシュ時の剣のオフセット向き
#define DASH_SWORD_OFFSET_ROT CVector(20.0f, 0.0f, -70.0f)

#define KICK_START_FRAME 26.0f	// 蹴り攻撃の開始フレーム
#define KICK_END_FRAME 40.0f		// 蹴り攻撃の終了フレーム
#define KICK_COL_RADIUS 7.5f		// 蹴り攻撃のコライダーの半径
// 蹴り攻撃のコライダーのオフセット座標
#define KICK_COL_OFFSET_POS CVector(0.0f, 4.0f, 2.5f)

// 先行入力のコライダーの半径
#define TA_COL_RADIUS 25.0f
// 先行入力のコライダーのオフセット座標
#define TA_COL_OFFSET_POS CVector(0.0f, 4.0f, 5.0f)

// 敵のアニメーションデータのテーブル
const std::vector<CEnemy::AnimData> ANIM_DATA =
{
	{ "",						true,	0.0f,	1.0f	},	// Tポーズ
	{ ANIM_PATH"idle.x",		true,	153.0f,	1.0f	},	// 待機
	{ ANIM_PATH"idle2.x",		true,	153.0f,	1.0f	},	// 待機(戦闘中)
	{ ANIM_PATH"walk.x",		true,	82.0f,	1.5f	},	// 歩行
	{ ANIM_PATH"run.x",			true,	39.0f,	1.5f	},	// ダッシュ
	{ ANIM_PATH"GSSlash1.x",	false,	77.0f,	1.25f	},	// 斬り攻撃
	{ ANIM_PATH"GSSlash2.x",	false,	110.0f,	1.50f	},	// 斬り攻撃
	{ ANIM_PATH"GSSlash.x",		false,	212.0f,	1.75f	},	// 斬りかかり攻撃
	{ ANIM_PATH"kick.x",		false,	74.0f,	1.75f	},	// 蹴り攻撃
	{ ANIM_PATH"jump_start.x",	false,	25.0f,	1.0f	},	// ジャンプ開始
	{ ANIM_PATH"jump.x",		true,	1.0f,	1.0f	},	// ジャンプ中
	{ ANIM_PATH"jump_end.x",	false,	26.0f,	1.0f	},	// ジャンプ終了
	{ ANIM_PATH"avoidR.x",		true,	189.0f,	2.5f	},	// 回避:右
	{ ANIM_PATH"avoidL.x",		true,	189.0f,	2.5f	},	// 回避:左
	{ ANIM_PATH"hit.x",			false,	44.0f,	1.0f	},	// 仰け反り
	{ "",						true,	0.0f,	1.0f	},	// Tポーズ
};

// コンストラクタ
CSoldier::CSoldier(CPlayer* player)
	: mpRideObject(nullptr)
	, mIsPlayedSlashSE(false)
	, mIsSpawnedSlashEffect(false)
	, mpSword(nullptr)
	, mNextAttack(false)
	, mIsBattle(true)
	, mBattleIdletime(0.0f)
	, mpBattleTarget(nullptr)
{
	mMaxHp = 100;
	mHp = mMaxHp;
	mpBattleTarget = player;

	// ゲージのオフセット位置を設定
	mGaugeOffsetPos = CVector(0.0f, GAUGE_OFFSET_Y, 0.0f);

	// 敵を初期化
	InitEnemy("Soldier", &ANIM_DATA);

	// 最初は待機アニメーションを再生
	ChangeAnimation((int)EAnimType::eIdle);

	// 本体のコライダーを作成
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::eEnemy,
		CVector(0.0f, BODY_RADIUS, 0.0f),
		CVector(0.0f, BODY_HEIGHT - BODY_RADIUS, 0.0f),
		BODY_RADIUS
	);
	//mpBodyCol->SetEnable(true);
	mpBodyCol->SetCollisionTags({ ETag::eField, ETag::eRideableObject, ETag::ePlayer, ETag::eEnemy });
	mpBodyCol->SetCollisionLayers({ ELayer::eField, ELayer::ePlayer, ELayer::eEnemy, ELayer::eAttackCol, ELayer::eTypeAheadCol });

	mpSlashSE = CResourceManager::Get<CSound>("SlashSound");

	// 敵の剣を作成
	mpSword = new CSword
	(
		this,
		ETag::eEnemy,
		{ ETag::ePlayer },	// プレイヤーのタグが設定されたコライダーと衝突
		{ ELayer::ePlayer }	// プレイヤーのレイヤーが設定されたコライダーと衝突
	);

	mpSword->Scale(2.0f, 1.25f, 1.25f);

	// 右手のフレームを取得し、
	// 剣にプレイヤーの右手の行列をアタッチ
	CModelXFrame* frame = mpModel->FinedFrame("Armature_mixamorig_RightHand");
	mpSword->SetAttachMtx(&frame->CombinedMatrix());
	mpSword->Position(SWORD_OFFSET_POS);
	mpSword->Rotation(SWORD_OFFSET_ROT);

	// 蹴り攻撃用のコライダーを作成
	mpKickCol = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		KICK_COL_RADIUS
	);
	// 敵の本体のコライダーとのみヒットするように設定
	mpKickCol->SetCollisionTags({ ETag::ePlayer });
	mpKickCol->SetCollisionLayers({ ELayer::ePlayer });
	// 敵の正面にズラす
	mpKickCol->Position(KICK_COL_OFFSET_POS);
	// 攻撃コライダーは最初はオフにしておく
	mpKickCol->SetEnable(false);

	// 先行入力(Type Ahead)用のコライダーを作成
	mpTACol = new CColliderSphere
	(
		this, ELayer::eTypeAheadCol,
		TA_COL_RADIUS
	);
	// プレイヤーの本体のコライダーとのみヒットするように設定
	mpTACol->SetCollisionTags({ ETag::ePlayer });
	mpTACol->SetCollisionLayers({ ELayer::ePlayer });
	// プレイヤーの正面にズラす
	mpTACol->Position(TA_COL_OFFSET_POS);
	// 先行入力コライダーは最初はオフにしておく
	mpTACol->SetEnable(false);
}

// デストラクタ
CSoldier::~CSoldier()
{
	// コライダーを削除
	SAFE_DELETE(mpBodyCol);
	SAFE_DELETE(mpKickCol);
	SAFE_DELETE(mpTACol);

	// 剣が存在したら、
	if (mpSword != nullptr)
	{
		// 持ち主を解除してから、削除
		mpSword->SetOwner(nullptr);
		mpSword->Kill();
	}
}

// 攻撃中か
bool CSoldier::IsAttacking() const
{
	// 斬り1攻撃中
	if (mState == (int)EState::eAttack1) return true;
	// 斬り2攻撃中
	if (mState == (int)EState::eAttack2) return true;
	// 斬りX攻撃中
	if (mState == (int)EState::eAttackX) return true;
	// 蹴り攻撃攻撃中
	if (mState == (int)EState::eKick) return true;

	// 攻撃中でない
	return false;
}

// 攻撃開始
void CSoldier::AttackStart()
{
	// 先行入力コライダーをオフにする
	mpTACol->SetEnable(false);

	// ベースクラスの攻撃開始処理を呼び出し
	CEnemy::AttackStart();

	// 斬り攻撃中であれば、剣のコライダーをオンにする
	if (mState == (int)EState::eAttack1 || mState == (int)EState::eAttack2 || mState == (int)EState::eAttackX)
	{
		mpSword->SetEnableCol(true);
	}
	// 蹴り攻撃中であれば、蹴り攻撃用のコライダーをオンにする
	else if (mState == (int)EState::eKick)
	{
		mpKickCol->SetEnable(true);
	}
}

// 攻撃終了
void CSoldier::AttackEnd()
{
	// ベースクラスの攻撃終了処理を呼び出し
	CEnemy::AttackEnd();

	// 攻撃コライダーをオフ
	mpSword->SetEnableCol(false);
	mpKickCol->SetEnable(false);
}

// ダメージを受ける
void CSoldier::TakeDamage(int damage, CObjectBase* causer)
{
	// ベースクラスのダメージ処理を呼び出す
	CEnemy::TakeDamage(damage, causer);

	// 死亡していなければ、
	if (!IsDeath())
	{
		// 仰け反り状態へ移行
		ChangeState((int)EState::eHit);

		// 攻撃を加えた相手を戦闘相手に設定
		mpBattleTarget = causer;

		// 攻撃を加えた相手の方向へ向く
		LookAtBattleTarget(true);

		// 戦闘状態へ切り替え
		mIsBattle = true;

		// 移動を停止
		mMoveSpeed = CVector::zero;
	}
}

// 死亡
void CSoldier::Death()
{
	// 死亡状態に切り替え
	ChangeState((int)EState::eDeath);
}

// 衝突処理
void CSoldier::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// ベースの衝突処理を呼び出す
	CEnemy::Collision(self, other, hit);

	// 先行入力用コライダーがヒットした
	if (other->Layer() == ELayer::eTypeAheadCol)
	{
		if (mState == (int)EState::eIdle)
		{
			SelectAvoid();
		}
	}

	// 剣のコライダーが衝突した
	if (self == mpSword->Collider())
	{
		CCharaBase* hitChara = dynamic_cast<CCharaBase*>(other->Owner());
		if (hitChara != nullptr && !IsAttackHitObj(hitChara))
		{
			AddAttackHitObj(hitChara);
			// 状態に合わせて、更新処理を切り替える
			switch (mState)
			{
				// 斬り攻撃1
			case (int)EState::eAttack1:		hitChara->TakeDamage(4, this);	break;
				// 斬り攻撃2
			case (int)EState::eAttack2:		hitChara->TakeDamage(6, this);	break;
				// 斬り攻撃X
			case (int)EState::eAttackX:		hitChara->TakeDamage(5, this);	break;
			}
		}
	}
	// 蹴り攻撃のコライダーが衝突した
	else if (self == mpKickCol)
	{
		CCharaBase* hitChara = dynamic_cast<CCharaBase*>(other->Owner());
		if (hitChara != nullptr && !IsAttackHitObj(hitChara))
		{
			AddAttackHitObj(hitChara);
			hitChara->TakeDamage(1, this);
		}
	}
}

// 戦闘相手の方へ向く
void CSoldier::LookAtBattleTarget(bool immediate)
{
	// 戦闘相手がいなければ、処理しない
	if (mpBattleTarget == nullptr) return;

	// 戦闘相手までの方向ベクトルを求める
	CVector targetPos = mpBattleTarget->Position();
	CVector vec = targetPos - Position();
	vec.Y(0.0f);
	vec.Normalize();
	// すぐに戦闘相手の方向へ向く
	if (immediate)
	{
		Rotation(CQuaternion::LookRotation(vec));
	}
	// 徐々に戦闘相手の方向へ向く
	else
	{
		CVector forward = CVector::Slerp
		(
			VectorZ(), vec,
			LOOKAT_SPEED * Times::DeltaTime()
		);
		Rotation(CQuaternion::LookRotation(forward));
	}
}

// 針を発射
void CSoldier::ShotNeedle()
{
	/*
	CVector pos = Position() + Rotation() * ATTACK2_NEEDLE_OFFSET_POS;
	CVector forward = VectorZ();
	float startAngle = -ATTACK2_NEEDLE_SHOT_ANGLE;
	float endAngle = ATTACK2_NEEDLE_SHOT_ANGLE;
	for (int i = 0; i < ATTACK2_NEEDLE_SHOT_DIR_COUNT; i++)
	{
		float alpha = (float)i / (ATTACK2_NEEDLE_SHOT_DIR_COUNT - 1);
		float angle = Math::Lerp(startAngle, endAngle, alpha);
		CVector dir = CQuaternion(0.0f, angle, 0.0f) * forward;

		//CSoldierNeedle* needle = new CSoldierNeedle(ATTACK2_NEEDLE_SPEED, ATTACK2_NEEDLE_DIST);
		//needle->Position(pos);
		//needle->Rotation(CQuaternion::LookRotation(dir));
	}
	*/
}

// 状態切り替え
void CSoldier::ChangeState(int state)
{
	if (mState == state) return;

	// 攻撃中に他に状態に変わる時は、
	// 攻撃終了処理を呼び出しておく
	if (IsAttacking())
	{
		AttackEnd();
	}

	// 状態切り替え
	CEnemy::ChangeState(state);
}

// 待機状態の更新処理
void CSoldier::UpdateIdle()
{
	// 通常時の待機
	if (!mIsBattle)
	{
		ChangeAnimation((int)EAnimType::eIdle);
	}
	// 戦闘時の待機
	else
	{
		ChangeAnimation((int)EAnimType::eIdleBattle);
		// 徐々に戦闘相手の方向へ向く
		LookAtBattleTarget();

		// ステップごとに処理を切り替え
		switch (mStateStep)
		{
			// ステップ0：待機時間をランダムで決定
		case 0:
			// 待機時間が決まってなければ
			if (mBattleIdletime <= 0.0f)
			{
				// ランダムで待機時間を決定
				mBattleIdletime = Math::Rand
				(
					BATTLE_IDLE_TIME_MIN,
					BATTLE_IDLE_TIME_MAX
				);
			}
			mStateStep++;
			break;
			// ステップ1：待機時間の経過待ち
		case 1:
			// 戦闘時の待機時間待ち
			if (mElapsedTime < mBattleIdletime)
			{
				mElapsedTime += Times::DeltaTime();
			}
			// 待機時間が経過した
			else
			{
				// 次の状態（デフォルトは追跡状態）
				EState nextState = EState::eChase;

				// 戦闘相手までの距離を求める
				CVector targetPos = mpBattleTarget->Position();
				CVector vec = targetPos - Position();
				vec.Y(0.0f);
				float dist = vec.Length();
				// 戦闘相手までの距離が離れていたら、
				if (dist >= ATTACK2_DIST)
				{
					/*
					// 一定確率で、針攻撃に変更
					int rand = Math::Rand(0, 99);
					if (rand < ATTACK2_PROB)
					{
						// 歩行アニメーションを再生
						ChangeAnimation((int)EAnimType::eRun);

						// 残り距離が移動距離より大きい場合は、移動距離分移動
						CVector dir = vec.Normalized();
						float moveDist = RUN_SPEED * Times::DeltaTime();
						if (dist > moveDist)
						{
							mMoveSpeed = dir * moveDist;
						}
						// 残り距離の方が小さい場合は、
						// 残り距離分移動して、待機状態へ移行
						else
						{
							mMoveSpeed = dir * dist;
							ChangeState((int)EState::eIdle);
						}
					}
					*/
				}

				// 次の状態へ移行
				ChangeState((int)nextState);

				// 戦闘待機時間を初期化
				mBattleIdletime = 0.0f;
			}
			break;
		}
	}
}

// 追いかける時の更新処理
void CSoldier::UpdateChase()
{
	mMoveSpeed = CVector::zero;

	// 現在地と目的地を取得
	CVector pos = Position();
	CVector targetPos = mpBattleTarget->Position();
	targetPos.Y(pos.Y());
	// 現在地から目的地までのベクトルを求める
	CVector vec = targetPos - pos;
	// 攻撃範囲内であれば
	float dist = vec.Length();
	if (dist <= ATTACK_RANGE)
	{
		// 攻撃状態へ移行
		ChangeState((int)EState::eAttack1);
	}
	// 攻撃範囲外
	else if (dist >= ATTACK2_DIST)
	{
		// 歩行アニメーションを再生
		ChangeAnimation((int)EAnimType::eRun);

		// 残り距離が移動距離より大きい場合は、移動距離分移動
		CVector dir = vec.Normalized();
		float moveDist = RUN_SPEED * Times::DeltaTime();
		if (dist > moveDist)
		{
			mMoveSpeed = dir * moveDist;
		}
		// 残り距離の方が小さい場合は、
		// 残り距離分移動して、待機状態へ移行
		else
		{
			mMoveSpeed = dir * dist;
			ChangeState((int)EState::eIdle);
		}
	}
	else
	{
		// 歩行アニメーションを再生
		ChangeAnimation((int)EAnimType::eWalk);

		// 残り距離が移動距離より大きい場合は、移動距離分移動
		CVector dir = vec.Normalized();
		float moveDist = MOVE_SPEED * Times::DeltaTime();
		if (dist > moveDist)
		{
			mMoveSpeed = dir * moveDist;
		}
		// 残り距離の方が小さい場合は、
		// 残り距離分移動して、待機状態へ移行
		else
		{
			mMoveSpeed = dir * dist;
			ChangeState((int)EState::eIdle);
		}
	}

	// 徐々に戦闘相手の方向へ向く
	LookAtBattleTarget();
}

// パンチ攻撃時の更新処理
void CSoldier::UpdateAttack1()
{
	// ステップごとに処理を分ける
	switch (mStateStep)
	{
		// ステップ0：攻撃アニメーション再生
	case 0:
		ChangeAnimation((int)EAnimType::eAttack1, true);
		mStateStep++;
		break;
		// ステップ1：攻撃開始
	case 1:
		// 攻撃を開始するまで、徐々に戦闘相手の方向へ向く
		LookAtBattleTarget();

		// 攻撃開始フレームまで経過したか
		if (GetAnimationFrame() >= ATTACK1_START_FRAME)
		{
			// 斬撃SEを再生
			mpSlashSE->Play();
			// 攻撃開始処理を呼び出す
			AttackStart();
			mStateStep++;
		}
		break;
		// ステップ2：攻撃終了
	case 2:
		// 攻撃終了フレームまで経過したか
		if (GetAnimationFrame() >= ATTACK1_END_FRAME)
		{
			// 攻撃終了処理を呼び出す
			AttackEnd();

			// 一定確率で、連続攻撃を予約
			int rand = Math::Rand(0, 99);
			if (rand < ATTACK2_PROB) mNextAttack = true;
			mStateStep++;
		}
		break;
		// ステップ3：攻撃アニメーション終了待ち
	case 3:
		// アニメーション終了したら、待機状態へ戻す
		if (IsAnimationFinished())
		{
			if (!mNextAttack)
			{
				// 待機状態へ移行
				ChangeState((int)EState::eIdle);
				ChangeAnimation((int)EAnimType::eIdle);
			}
			else
			{
				// 攻撃2段目へ移行
				ChangeState((int)EState::eAttack2);
				mNextAttack = false;
				CObjectBase::AttackStart();
			}
		}
		break;
	}
}

// 斬り攻撃2の更新処理
void CSoldier::UpdateAttack2()
{
	// ステップごとに処理を分ける
	switch (mStateStep)
	{
		// ステップ0：攻撃アニメーション再生
	case 0:
		ChangeAnimation((int)EAnimType::eAttack2, true);
		mStateStep++;
		break;
		// ステップ1：攻撃開始
	case 1:
		// 攻撃を開始するまで、徐々に戦闘相手の方向へ向く
		LookAtBattleTarget();

		// 攻撃開始フレームまで経過したか
		if (GetAnimationFrame() >= ATTACK2_START_FRAME)
		{
			// 斬撃SEを再生
			mpSlashSE->Play();
			// 攻撃開始処理を呼び出す
			AttackStart();
			mStateStep++;
		}
		break;
		// ステップ2：攻撃終了
	case 2:
		// 攻撃終了フレームまで経過したか
		if (GetAnimationFrame() >= ATTACK2_END_FRAME)
		{
			// 攻撃終了処理を呼び出す
			AttackEnd();

			// 一定確率で、連続攻撃を予約
			int rand = Math::Rand(0, 99);
			if (rand < ATTACKX_PROB) mNextAttack = true;
			mStateStep++;
		}
		break;
		// ステップ3：攻撃アニメーション終了待ち
	case 3:
		// アニメーション終了したら、待機状態へ戻す
		if (IsAnimationFinished())
		{
			if (!mNextAttack)
			{
				// 待機状態へ移行
				ChangeState((int)EState::eIdle);
				ChangeAnimation((int)EAnimType::eIdle);
			}
			else
			{
				// 攻撃X段目へ移行
				ChangeState((int)EState::eAttackX);
				mNextAttack = false;
				CObjectBase::AttackStart();
			}
		}
		break;
	}
}

void CSoldier::UpdateAttackX()
{
	switch (mStateStep)
	{
	case 0:
		// 先行入力コライダーをオンにする
		mpTACol->SetEnable(true);

		mpSword->Rotation(ATTACKX_SWORD_OFFSET_ROT);
		// 攻撃アニメーションを開始
		ChangeAnimation((int)EAnimType::eAttackX, true);
		// 斬撃SEの再生済みフラグを初期化
		mIsPlayedSlashSE = false;
		// 斬撃エフェクトの生成済みフラグを初期化
		mIsSpawnedSlashEffect = false;

		mAttackVec = VectorZ();

		mStateStep++;
		break;
	case 1:
		if (GetAnimationFrame() >= ATTACKX_START_FRAME)
		{
			// 斬撃SEを再生
			mpSlashSE->Play();
			// 攻撃開始
			AttackStart();

			mStateStep++;
		}
		break;
	case 2:
		if (GetAnimationFrame() >= ATTACKX_START_FRAME + 85.0f)
		{
			CObjectBase::AttackStart();
			mInAttack = true;
			mAttackTimer = 0.0f;
			mStateStep++;
		}
		break;
	case 3:
		if (GetAnimationFrame() >= ATTACKX_END_FRAME)
		{
			// 攻撃終了
			AttackEnd();

			mInAttack = false;

			mStateStep++;
		}

		if (mInAttack)
		{
			mAttackTimer += Times::DeltaTime();

			// 1秒あたりの移動速度
			CVector move = mAttackVec * 30.0f * Times::DeltaTime();
			Position(Position() + move);
		}

		break;
	case 4:
		// 攻撃アニメーションが終了したら、
		if (IsAnimationFinished())
		{
			mpSword->Rotation(SWORD_OFFSET_ROT);
			// 待機状態へ移行
			ChangeState((int)EState::eIdle);
			ChangeAnimation((int)EAnimType::eIdle);
		}
		break;
	}
}

void CSoldier::SelectAvoid()
{
	if (!mpBattleTarget) return;

	// 自分の前方向
	CVector myForward = VectorZ();
	myForward.Y(0.0f);
	myForward.Normalize();

	// 自分 → 敵 の方向
	CVector toTarget = mpBattleTarget->Position() - Position();
	toTarget.Y(0.0f);
	toTarget.Normalize();

	// 外積で左右判定
	// Yが + → 敵は右側
	// Yが - → 敵は左側
	float crossY = CVector::Cross(myForward, toTarget).Y();

	if (crossY >= 0.0f)
	{
		// 敵が右 → 左に回避
		mAvoidVec = -CVector::Cross(myForward, CVector::up);
		mAvoidVec.Normalize();
		ChangeState((int)EState::eAvoidR);
		mIsGravity = false;
		mpBodyCol->SetCollisionLayers({ ELayer::eField, ELayer::eEnemy });
	}
	else if (crossY < 0.0f)
	{
		// 敵が左 → 右に回避
		mAvoidVec = CVector::Cross(myForward, CVector::up);
		mAvoidVec.Normalize();
		ChangeState((int)EState::eAvoidR);
		mIsGravity = false;
		mpBodyCol->SetCollisionLayers({ ELayer::eField, ELayer::eEnemy });
	}
}

void CSoldier::UpdateAvoidR()
{
	switch (mStateStep)
	{
	case 0:
		// 回避アニメーションを開始
		ChangeAnimation((int)EAnimType::eAvoidR, true);

		mStateStep++;
		break;
	case 1:
		if (GetAnimationFrame() >= 60.0f && !mAvoidMoving)
		{
			mAvoidMoving = true;
			mAvoidTimer = 0.0f;
		}

		if (mAvoidMoving)
		{
			mAvoidTimer += Times::DeltaTime();

			// 1秒あたりの移動速度
			CVector move = mAvoidVec * 300.0f * Times::DeltaTime();
			Position(Position() + move);

			if (mAvoidTimer >= mAvoidDuration)
			{
				mAvoidMoving = false;
				mStateStep++;
			}
		}
		break;
	case 2:
		// 回避アニメーションが終了したら
		if (IsAnimationFinished())
		{
			mpBodyCol->SetCollisionLayers({ ELayer::eField, ELayer::ePlayer, ELayer::eEnemy, ELayer::eAttackCol, ELayer::eTypeAheadCol });
			mIsGravity = true;
			// 待機状態へ移行
			ChangeState((int)EState::eIdle);
			ChangeAnimation((int)EAnimType::eIdle);
		}
		break;
	}
}

void CSoldier::UpdateAvoidL()
{
	switch (mStateStep)
	{
	case 0:
		// 回避アニメーションを開始
		ChangeAnimation((int)EAnimType::eAvoidL, true);

		mStateStep++;
		break;
	case 1:
		if (GetAnimationFrame() >= 60.0f && !mAvoidMoving)
		{
			mAvoidMoving = true;
			mAvoidTimer = 0.0f;
		}

		if (mAvoidMoving)
		{
			mAvoidTimer += Times::DeltaTime();

			// 1秒あたりの移動速度
			CVector move = mAvoidVec * 300.0f * Times::DeltaTime();
			Position(Position() + move);

			if (mAvoidTimer >= mAvoidDuration)
			{
				mAvoidMoving = false;
				mStateStep++;
			}
		}
		break;
	case 2:
		// 回避アニメーションが終了したら
		if (IsAnimationFinished())
		{
			mpBodyCol->SetCollisionLayers({ ELayer::eField, ELayer::ePlayer, ELayer::eEnemy, ELayer::eAttackCol, ELayer::eTypeAheadCol });
			mIsGravity = true;
			// 待機状態へ移行
			ChangeState((int)EState::eIdle);
			ChangeAnimation((int)EAnimType::eIdle);
		}
		break;
	}
}

// 仰け反り状態の更新処理
void CSoldier::UpdateHit()
{
	// ステップごとに処理を分ける
	switch (mStateStep)
	{
		// ステップ0：仰け反りアニメーション再生
	case 0:
		ChangeAnimation((int)EAnimType::eHit, true);
		mStateStep++;
		break;
		// ステップ1：アニメーション終了待ち
	case 1:
		// 仰け反りアニメーションが終了したら、
		// 待機状態へ戻す
		if (IsAnimationFinished())
		{
			ChangeState((int)EState::eIdle);
		}
		break;
	}
}

// 死亡状態の更新処理
void CSoldier::UpdateDeath()
{
	// ステップごとに処理を分ける
	switch (mStateStep)
	{
		// ステップ0：死亡アニメーション再生
	case 0:
		mMoveSpeed = CVector::zero;
		ChangeAnimation((int)EAnimType::eDeath, true);
		mStateStep++;
		break;
		// ステップ1：アニメーション終了待ち
	case 1:
		// 死亡アニメーションが終了したら、削除
		if (IsAnimationFinished())
		{
			mStateStep++;
		}
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

// 更新
void CSoldier::Update()
{
	// 状態に合わせて、更新処理を切り替える
	switch ((EState)mState)
	{
		// 待機状態
	case EState::eIdle:		UpdateIdle();	break;
		// 追いかける
	case EState::eChase:	UpdateChase();	break;
		// 斬り攻撃1
	case EState::eAttack1:	UpdateAttack1(); break;
		// 斬り攻撃2
	case EState::eAttack2:	UpdateAttack2(); break;
		// 斬り攻撃X
	case EState::eAttackX:	UpdateAttackX(); break;
		// 回避:右
	case EState::eAvoidR:	UpdateAvoidR();	break;
		// 回避:左
	case EState::eAvoidL:	UpdateAvoidL();	break;
		// 仰け反り
	case EState::eHit:		UpdateHit();	break;
		// 死亡状態
	case EState::eDeath:	UpdateDeath();	break;
	}

	// 敵のベースクラスの更新
	CEnemy::Update();

	// 武器の行列を更新
	mpSword->UpdateMtx();

	// 戦闘相手までの距離をデバッグ表示
	if (mpBattleTarget != nullptr)
	{
		CVector targetPos = mpBattleTarget->Position();
		CVector pos = Position();
		targetPos.Y(pos.Y());
		float dist = CVector::Distance(targetPos, pos);
		CDebugPrint::Print("Dist:%.2f\n", dist);
	}
	CDebugPrint::Print("EnemyState:%d\n", mState);
	CDebugPrint::Print("EnemyAnimType:%d\n", mAnimType);
}

