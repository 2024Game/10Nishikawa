//プレイヤークラスのインクルード
#include "CPlayer.h"
#include "CInput.h"
#include "CCamera.h"
#include "CBullet.h"
#include "CBlood.h"
#include "CFlamethrower.h"
#include "CSlash.h"
#include "Maths.h"
#include "CGreatSword.h"
#include "CColliderCapsule.h"
#include "CColliderSphere.h"
#include "CTargetUI.h"
#include "CPlayerUI3D.h"
#include "CEnemy.h"
#include "CEnemyManager.h"

// アニメーションのパス
#define ANIM_PATH "Character\\TestPlayer\\Anims\\"
#define BODY_HEIGHT 16.0f	// 本体のコライダーの高さ
#define BODY_RADIUS 3.0f	// 本体のコライダーの幅
#define MOVE_SPEED 24.0f	// 移動速度 
#define RUN_SPEED 50.0f		// ダッシュ移動速度
#define JUMP_SPEED 1.5f		// ジャンプ速度
#define GRAVITY 0.0625f		// 重力加速度

#define MOTION_BLUR_TIME 3.5f	// モーションブラーを掛ける時間
#define MOTION_BLUR_WIDTH 0.8f	// モーションブラーの幅
#define MOTION_BLUR_COUNT 4		// モーションブラーの反復回数

#define LOCKON_DISTANCE 300.0f
#define INDICATOR_OFFSET_Y 5.0f

//#define ATTACK_START_FRAME 26.0f	// 斬り攻撃の開始フレーム
//#define ATTACK_END_FRAME 50.0f	// 斬り攻撃の終了フレーム

#define ATTACK1_START_FRAME 25.0f	// 斬り攻撃1の開始フレーム
#define ATTACK1_END_FRAME 55.0f		// 斬り攻撃1の終了フレーム
#define ATTACK2_START_FRAME 35.0f	// 斬り攻撃2の開始フレーム
#define ATTACK2_END_FRAME 100.0f	// 斬り攻撃2の終了フレーム
#define ATTACKX_START_FRAME 35.0f	// 斬り攻撃Xの開始フレーム
#define ATTACKX_END_FRAME 210.0f	// 斬り攻撃Xの終了フレーム
#define DEATH_END_FRAME 110.0f		// 死亡の終了フレーム

#define SLIDEATT_START_FRAME 60.0f	// スライド斬り攻撃の開始フレーム
#define SLIDEATT_END_FRAME 100.0f	// スライド斬り攻撃の終了フレーム

// 剣のオフセット座標
#define SWORD_OFFSET_POS CVector(0.0f, 7.2f, 3.5f)
// 剣のオフセット向き
#define SWORD_OFFSET_ROT CVector(20.0f, 0.0f, -30.0f)

#define ATTACKX_SWORD_OFFSET_ROT CVector(-20.0f, 0.0f, -25.0f)

// ダッシュ時の剣のオフセット向き
#define DASH_SWORD_OFFSET_ROT CVector(0.0f, 0.0f, 0.0f)

#define KICK_START_FRAME 26.0f		// 蹴り攻撃の開始フレーム
#define KICK_END_FRAME 40.0f		// 蹴り攻撃の終了フレーム
#define KICK_COL_RADIUS 7.5f		// 蹴り攻撃のコライダーの半径
// 蹴り攻撃のコライダーのオフセット座標
#define KICK_COL_OFFSET_POS CVector(0.0f, 4.0f, 5.0f)

// 先行入力のコライダーの半径
#define TA_COL_RADIUS 27.5f
// 先行入力のコライダーのオフセット座標
#define TA_COL_OFFSET_POS CVector(0.0f, 4.0f, 2.75f)

// プレイヤーのインスタンス
CPlayer* CPlayer::spInstance = nullptr;

// プレイヤーのアニメーションデータのテーブル
const CPlayer::AnimData CPlayer::ANIM_DATA[] =
{
	{ "",						true,	0.0f,	1.0f	},	// Tポーズ
	{ ANIM_PATH"idle.x",		true,	121.0f,	1.0f	},	// 待機
	{ ANIM_PATH"walk.x",		true,	82.0f,	2.25f	},	// 歩行
	{ ANIM_PATH"run.x",			true,	36.0f,	1.0f	},	// ダッシュ
	{ ANIM_PATH"GSSlash1.x",	false,	77.0f,	1.25f	},	// 斬り攻撃
	{ ANIM_PATH"GSSlash2.x",	false,	110.0f,	1.50f	},	// 斬り攻撃
	{ ANIM_PATH"GSSlash.x",		false,	212.0f,	1.75f	},	// 斬りかかり攻撃
	{ ANIM_PATH"kick.x",		false,	74.0f,	1.75f	},	// 蹴り攻撃
	{ ANIM_PATH"SlideAttack.x",	false,	128.0f,	4.0f	},	// スライド斬り攻撃
	{ ANIM_PATH"jump_start.x",	false,	25.0f,	1.0f	},	// ジャンプ開始
	{ ANIM_PATH"jump.x",		true,	1.0f,	1.0f	},	// ジャンプ中
	{ ANIM_PATH"jump_end.x",	false,	26.0f,	1.0f	},	// ジャンプ終了
	{ ANIM_PATH"avoidR.x",		false,	58.0f,	1.5f	},	// 回避:右
	{ ANIM_PATH"avoidL.x",		false,	58.0f,	1.5f	},	// 回避:左
	{ ANIM_PATH"avoidB.x",		true,	78.0f,	1.5f	},	// 回避:後ろ98
	{ ANIM_PATH"hit.x",			false,	44.0f,	1.0f	},	// 仰け反り
	{ ANIM_PATH"death.x",		false,	182.0f,	1.0f	},	// 死亡
	{ ANIM_PATH"victory.x",		true,	271.0f,	1.0f	},	// 勝利
};
// ToDo 連続攻撃が予約されたときは前のアニメーションの
// 終わりのフレームを短くしたほうがいいかも

// コンストラクタ
CPlayer::CPlayer(CSaveManager* SaveManager)
	: CXCharacter(ETag::ePlayer, ETaskPriority::ePlayer)
	, mState(EState::eReserve)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mMoveSpeedY(0.0f)
	, mIsGrounded(false)
	, mIsGravity(true)
	, mpRideObject(nullptr)
	, mIsPlayedSlashSE(false)
	, mIsSpawnedSlashEffect(false)
	, mMotionBlurRemainTime(0.0f)
	, mpGreatSword(nullptr)
	, mNextAttack(false)
	, mAttackMag(0.0f)
	, mAttackCost1(25.0f)
	, mAvoidCost(20.0f)
	, mpSaveManager(SaveManager)
	, mIsLockOn(false)
	, mpLockOnTarget(nullptr)
	, mpTargetUI(nullptr)
	, mpIndicator(nullptr)
	, mpHeadFrame(nullptr)
	, mInTypeAhead(false)
	, mInJustAction(false)
	, mS1RecastTime(10.0f)
	, mS1CastTime(mS1RecastTime)
	, mIsSA(false)

	, mAttHitCount(0)
	, mJustAvoidCount(0)
	, mRushHitCount(0)
	, mKickHitCount(0)
	, mJustKickHitCount(0)
{
	mMaxHp = mpSaveManager->data.maxHp;
	mHp = mpSaveManager->data.hp;
	mMaxSt = 150.0f + (mpSaveManager->data.stLv * 5);
	mSt = mMaxSt;
	mAttackMag = 1.0f + (mpSaveManager->data.attackLv * 0.05f);
	mStRegeneMag = 1.0f + (mpSaveManager->data.stRegeneLv * 0.025f);
	//インスタンスの設定
	spInstance = this;

	// モデルデータ取得
	CModelX* model = CResourceManager::Get<CModelX>("Player");

	// テーブル内のアニメーションデータを読み込み
	int size = ARRAY_SIZE(ANIM_DATA);
	for (int i = 0; i < size; i++)
	{
		const AnimData& data = ANIM_DATA[i];
		if (data.path.empty()) continue;
		model->AddAnimationSet(data.path.c_str());
	}
	// CXCharacterの初期化
	Init(model);

	// 最初は待機アニメーションを再生
	ChangeAnimation(EAnimType::eIdle);

	// 本体のコライダーを作成
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::ePlayer,
		CVector(0.0f, BODY_RADIUS, 0.0f),
		CVector(0.0f, BODY_HEIGHT - BODY_RADIUS, 0.0f),
		BODY_RADIUS
	);
	mpBodyCol->SetCollisionTags({ ETag::eField, ETag::eRideableObject, ETag::eEnemy });
	SetInvincible(false);

	mpSlashSE = CResourceManager::Get<CSound>("SlashSound");

	/*
	mpFlamethrower = new CFlamethrower
	(
		this, nullptr,
		CVector(0.0f, 14.0f, -1.0f),
		CQuaternion(0.0f, 90.0f, 0.0f).Matrix()
	);
	*/
	
	// プレイヤーの剣を作成
	mpGreatSword = new CGreatSword
	(
		this,
		ETag::ePlayer,
		{ ETag::eEnemy },	// 敵のタグが設定されたコライダーと衝突
		{ ELayer::eEnemy }	// 敵のレイヤーが設定されたコライダーと衝突
	);

	mpGreatSword->Scale(1.25f, 1.1f, 1.1f);

	// 右手のフレームを取得し、
	// 剣にプレイヤーの右手の行列をアタッチ
	CModelXFrame* frame = mpModel->FinedFrame("Armature_mixamorig_RightHand");
	mpGreatSword->SetAttachMtx(&frame->CombinedMatrix());
	mpGreatSword->Position(SWORD_OFFSET_POS);
	mpGreatSword->Rotation(SWORD_OFFSET_ROT);

	// 蹴り攻撃用のコライダーを作成
	mpKickCol = new CColliderSphere
	(
		this, ELayer::eAttackCol,
		KICK_COL_RADIUS
	);
	// 敵の本体のコライダーとのみヒットするように設定
	mpKickCol->SetCollisionTags({ ETag::eEnemy });
	mpKickCol->SetCollisionLayers({ ELayer::eEnemy });
	// プレイヤーの正面にズラす
	mpKickCol->Position(KICK_COL_OFFSET_POS);
	// 攻撃コライダーは最初はオフにしておく
	mpKickCol->SetEnable(false);


	// 先行入力(Type Ahead)用のコライダーを作成
	mpTACol = new CColliderSphere
	(
		this, ELayer::eTypeAheadCol,
		TA_COL_RADIUS
	);
	// 敵の本体のコライダーとのみヒットするように設定
	mpTACol->SetCollisionTags({ ETag::eEnemy });
	mpTACol->SetCollisionLayers({ ELayer::eEnemy });
	// プレイヤーの正面にズラす
	mpTACol->Position(TA_COL_OFFSET_POS);
	// 先行入力コライダーは最初はオフにしておく
	mpTACol->SetEnable(false);

	// ロックオンUIを作成
	mpTargetUI = new CTargetUI();

	mpHeadFrame = mpModel->FinedFrame("Armature_mixamorig_Head");
	// インジケーターを作成
	mpIndicator = new CPlayerUI3D(this);
	// インジケーターのオフセット位置を設定
	mIndicatorOffsetPos = CVector(0.0f, INDICATOR_OFFSET_Y, 0.0f);
}

CPlayer::~CPlayer()
{
	// コライダーを削除
	SAFE_DELETE(mpBodyCol);
	SAFE_DELETE(mpKickCol);
	SAFE_DELETE(mpTACol);

	// 剣が存在したら、
	if (mpGreatSword != nullptr)
	{
		// 持ち主を解除してから、削除
		mpGreatSword->SetOwner(nullptr);
		mpGreatSword->Kill();
	}

	mpTargetUI->Kill();

	// インジケーターが存在したら、一緒に削除する
	if (mpIndicator != nullptr)
	{
		mpIndicator->SetOwner(nullptr);
		mpIndicator->Kill();
	}
}

void CPlayer::TutorialInit()
{
	mMaxHp = 100.0f;
	mHp = mMaxHp;
	mMaxSt = 150.0f;
	mSt = mMaxSt;
	mAttackMag = 1.0f;
	mStRegeneMag = 1.0f;
}

CPlayer* CPlayer::Instance()
{
	return spInstance;
}

// アニメーション切り替え
void CPlayer::ChangeAnimation(EAnimType type, bool restart)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength, restart);
	if (
		type == EAnimType::eAttack1 ||
		type == EAnimType::eAttack2 ||
		type == EAnimType::eAttackX)
	{
		CXCharacter::SetAnimationSpeed(data.speed * 1.0f);
	}
	else
	{
		CXCharacter::SetAnimationSpeed(data.speed);
	}
}

// 状態を切り替え
void CPlayer::ChangeState(EState state)
{
	if (mState == state) return;

	// 攻撃中に他に状態に変わる時は、
	// 攻撃終了処理を呼び出しておく
	if (IsAttacking())
	{
		AttackEnd();
	}

	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}

void CPlayer::UpdateReserve()
{

}

// 待機
void CPlayer::UpdateIdle()
{
	if (mHp > 0.0f)
	{
		CCharaBase::GainStamina(10.0f * Times::DeltaTime() * mStRegeneMag);
	}

	// 接地していれば、
	if (mIsGrounded)
	{
		AvoidJudge();
		// 左クリックで斬撃攻撃へ移行
		if (CInput::PushKey(VK_LBUTTON) && mSt >= mAttackCost1)
		{
			CCharaBase::UseStamina(mAttackCost1);

			// 先行入力コライダーをオンにする
			mpTACol->SetEnable(true);

			mMoveSpeed = CVector::zero;
			ChangeState(EState::eAttack1);
		}
		// Cでキック攻撃へ移行
		else if (CInput::PushKey('C'))
		{
			if (mS1CastTime < mS1RecastTime) return;
			mS1CastTime = 0.0f;

			// 先行入力コライダーをオンにする
			mpTACol->SetEnable(true);

			mMoveSpeed = CVector::zero;
			ChangeState(EState::eKick);
		}
		// SPACEキーでジャンプ開始へ移行
		else if (CInput::PushKey(VK_SPACE))
		{
			ChangeState(EState::eJumpStart);
		}
	}
}

// 斬り攻撃
void CPlayer::UpdateAttack1()
{
	switch (mStateStep)
	{
		case 0:
		{
			LookCameraForward();

			// 攻撃アニメーションを開始
			ChangeAnimation(EAnimType::eAttack1, true);
			// 斬撃SEの再生済みフラグを初期化
			mIsPlayedSlashSE = false;
			// 斬撃エフェクトの生成済みフラグを初期化
			mIsSpawnedSlashEffect = false;

			mAttackVec = VectorZ();

			mStateStep++;
			break;
		}
			
		case 1:
			AvoidJudge();

			if (GetAnimationFrame() >= ATTACK1_START_FRAME)
			{
				// 斬撃SEを再生
				mpSlashSE->Play();
				// 攻撃開始
				AttackStart();

				mInAttack = true;

				mStateStep++;
			}

			// 左クリックで連続攻撃を予約
			if (CInput::PushKey(VK_LBUTTON))
			{
				mNextAttack = true;
			}

			break;
		case 2:
			if (GetAnimationFrame() >= ATTACK1_END_FRAME)
			{
				// 攻撃終了
				AttackEnd();

				mInAttack = false;
				mMoveSpeed = CVector::zero;
				mStateStep++;
			}

			// 左クリックで連続攻撃を予約
			if (CInput::PushKey(VK_LBUTTON))
			{
				mNextAttack = true;
			}

			if (mInAttack)
			{
				// 1秒あたりの移動速度
				mMoveSpeed = mAttackVec * 5.0f * Times::DeltaTime();
			}
			break;
		case 3:
			AvoidJudge();
			// 攻撃アニメーションが終了したら、
			if (IsAnimationFinished())
			{
				if (!mNextAttack)
				{
					// 待機状態へ移行
					ChangeState(EState::eIdle);
					ChangeAnimation(EAnimType::eIdle);
				}
				else
				{
					mNextAttack = false;
					CObjectBase::AttackStart();
					if (mSt >= mAttackCost1)
					{
						CCharaBase::UseStamina(mAttackCost1);
						// 攻撃2段目へ移行
						ChangeState(EState::eAttack2);
					}
					else
					{
						// 待機状態へ移行
						ChangeState(EState::eIdle);
					}
				}
			}
			break;
	}
}

void CPlayer::UpdateAttack2()
{
	switch (mStateStep)
	{
	case 0:
	{
		LookCameraForward();

		// 先行入力コライダーをオンにする
		mpTACol->SetEnable(true);
		// 攻撃アニメーションを開始
		ChangeAnimation(EAnimType::eAttack2, true);
		// 斬撃SEの再生済みフラグを初期化
		mIsPlayedSlashSE = false;
		// 斬撃エフェクトの生成済みフラグを初期化
		mIsSpawnedSlashEffect = false;

		mAttackVec = VectorZ();

		mStateStep++;
		break;
	}
		
	case 1:
		AvoidJudge();
		if (GetAnimationFrame() >= ATTACK2_START_FRAME)
		{
			// 斬撃SEを再生
			mpSlashSE->Play();
			// 攻撃開始
			AttackStart();

			mInAttack = true;

			// 左クリックで連続攻撃を予約
			if (CInput::PushKey(VK_LBUTTON))
			{
				mNextAttack = true;
			}

			mStateStep++;
		}

		// 左クリックで連続攻撃を予約
		if (CInput::PushKey(VK_LBUTTON))
		{
			mNextAttack = true;
		}

		break;
	case 2:
		if (GetAnimationFrame() >= ATTACK2_END_FRAME)
		{
			// 攻撃終了
			AttackEnd();

			mInAttack = false;
			mMoveSpeed = CVector::zero;
			mStateStep++;
		}

		// 左クリックで連続攻撃を予約
		if (CInput::PushKey(VK_LBUTTON))
		{
			mNextAttack = true;
		}

		if (mInAttack)
		{
			// 1秒あたりの移動速度
			mMoveSpeed = mAttackVec * 10.0f * Times::DeltaTime();
		}
		break;
	case 3:
		AvoidJudge();
		// 攻撃アニメーションが終了したら、
		if (IsAnimationFinished())
		{
			if (!mNextAttack)
			{
				// 待機状態へ移行
				ChangeState(EState::eIdle);
				ChangeAnimation(EAnimType::eIdle);
			}
			else
			{
				mNextAttack = false;
				CObjectBase::AttackStart();
				if (mSt >= mAttackCost1)
				{
					CCharaBase::UseStamina(mAttackCost1);
					// 攻撃X段目へ移行
					ChangeState(EState::eAttackX);
				}
				else
				{
					// 待機状態へ移行
					ChangeState(EState::eIdle);
				}
			}

		}
		break;
	}
}

void CPlayer::UpdateAttackX()
{
	switch (mStateStep)
	{
	case 0:
	{
		LookCameraForward();

		// 先行入力コライダーをオンにする
		mpTACol->SetEnable(true);

		mpGreatSword->Rotation(ATTACKX_SWORD_OFFSET_ROT);
		// 攻撃アニメーションを開始
		ChangeAnimation(EAnimType::eAttackX, true);
		// 斬撃SEの再生済みフラグを初期化
		mIsPlayedSlashSE = false;
		// 斬撃エフェクトの生成済みフラグを初期化
		mIsSpawnedSlashEffect = false;

		mAttackVec = VectorZ();

		mStateStep++;
		break;
	}
		
	case 1:
		AvoidJudge();
		if (GetAnimationFrame() >= ATTACKX_START_FRAME)
		{
			// 斬撃SEを再生
			mpSlashSE->Play();
			// 攻撃開始
			AttackStart();

			// 左クリックで連続攻撃を予約
			if (CInput::PushKey(VK_LBUTTON))
			{
				//mNextAttack = true;
			}

			mStateStep++;
		}
		break;
	case 2:
		if (GetAnimationFrame() >= ATTACKX_START_FRAME + 85.0f)
		{
			CObjectBase::AttackStart();
			mInAttack = true;
			mStateStep++;
		}
		break;
	case 3:
		if (GetAnimationFrame() >= ATTACKX_END_FRAME)
		{
			// 攻撃終了
			AttackEnd();

			mInAttack = false;
			mMoveSpeed = CVector::zero;
			mStateStep++;
		}

		if (mInAttack)
		{
			// 1秒あたりの移動速度
			mMoveSpeed = mAttackVec * 30.0f * Times::DeltaTime();
		}
		break;
	case 4:
		AvoidJudge();
		// 攻撃アニメーションが終了したら、
		if (IsAnimationFinished())
		{
			if (!mNextAttack)
			{
				mpGreatSword->Rotation(SWORD_OFFSET_ROT);
				// 待機状態へ移行
				ChangeState(EState::eIdle);
				ChangeAnimation(EAnimType::eIdle);
			}
			else
			{
				// 待機状態へ移行
				ChangeState(EState::eAttack2);
			}

		}
		break;
	}
}

// 蹴り攻撃
void CPlayer::UpdateKick()
{
	switch (mStateStep)
	{
		case 0:
			// 攻撃アニメーションを開始
			ChangeAnimation(EAnimType::eKick, true);
			if (mInTypeAhead)
			{
				Times::SetTimeScale(0.25f);
				SetInvincible(true);
				mInJustAction = true;
			}

			// キック中はスーパーアーマーをオン
			mIsSA = true;

			mStateStep++;
			break;
		case 1:
			if (GetAnimationFrame() >= KICK_START_FRAME)
			{
				AttackStart();
				mStateStep++;
			}
			break;
		case 2:
			if (GetAnimationFrame() >= KICK_END_FRAME)
			{
				AttackEnd();
				mStateStep++;
			}
			break;
		case 3:
			// 攻撃アニメーションが終了したら、
			if (IsAnimationFinished())
			{
				if (mInJustAction)
				{
					Times::SetTimeScale(1.0f);
					SetInvincible(false);
					mInJustAction = false;
				}

				// スーパーアーマーをオフ
				mIsSA = false;

				// 待機状態へ移行
				ChangeState(EState::eIdle);
				ChangeAnimation(EAnimType::eIdle);
			}
			break;
	}
}

void CPlayer::UpdateSlideAttack()
{
	switch (mStateStep)
	{
	case 0:
		Times::SetTimeScale(0.15f);
		mStateStep++;
		break;
	case 1:
	{
		LockOnTarget();
		if (mpLockOnTarget)
		{
			LookCameraForward();

			// 左クリックで斬撃攻撃へ移行
			if (CInput::PushKey(VK_LBUTTON))
			{
				// モーションブラーを掛けている最中であれば、
				// 新しくモーションブラーを掛け直さない
				if (mMotionBlurRemainTime <= 0.0f)
				{
					System::SetEnableMotionBlur(true);
					mMotionBlurRemainTime = MOTION_BLUR_TIME;
				}

				mRushHitCount++;

				mStateStep++;
			}
		}
		break;
	}
	case 2:
	{
		mpGreatSword->Rotation(SWORD_OFFSET_ROT);
		// 攻撃アニメーションを開始
		ChangeAnimation(EAnimType::eSlideAtt, true);
		// 斬撃SEの再生済みフラグを初期化
		mIsPlayedSlashSE = false;
		// 斬撃エフェクトの生成済みフラグを初期化
		mIsSpawnedSlashEffect = false;

		mAttackVec = VectorZ();

		mStateStep++;
		break;
	}
	case 3:
	{
		CCamera* camera = CCamera::MainCamera();
		if (!camera || !mpLockOnTarget) break;

		// ===== カメラの向き（平面）=====
		CVector camForward = -camera->VectorZ();
		camForward.Y(0.0f);

		if (camForward.LengthSqr() < 0.0001f) break;

		camForward.Normalize();

		// 向きをカメラ方向へ
		Rotation(CQuaternion::LookRotation(camForward));


		// ===== 距離はロックオンターゲットまで =====
		CVector myPos = Position();
		CVector targetPos = mpLockOnTarget->Position();
		targetPos.Y(myPos.Y());   // 平面距離のみ

		float dist = CVector::Distance(myPos, targetPos);
		if (dist > 0.001f)
		{
			// 1フレームあたりの移動距離
			float movePerFrame = dist / (SLIDEATT_START_FRAME - 15.0f);

			// 1秒あたりの移動量
			mMoveSpeed = camForward * movePerFrame * Times::DeltaTime() * 60 * 6.00;
		}

		// ===== 攻撃開始 =====
		if (GetAnimationFrame() >= SLIDEATT_START_FRAME)
		{
			mMoveSpeed = CVector::zero;
			mpSlashSE->Play();
			AttackStart();
			mInAttack = true;
			mStateStep++;
		}
		break;
	}
	case 4:
		if (GetAnimationFrame() >= SLIDEATT_END_FRAME)
		{
			// 攻撃終了
			AttackEnd();

			mInAttack = false;
			mMoveSpeed = CVector::zero;
			mStateStep++;
		}

		if (mInAttack)
		{
			// 1秒あたりの移動速度
			mMoveSpeed = mAttackVec * 5.0f * Times::UnscaledDeltaTime();
		}
		break;
	case 5:
		// 攻撃アニメーションが終了したら、
		if (IsAnimationFinished())
		{
			Times::SetTimeScale(1.0f);
			UnLockTarget();
			SetInvincible(false);
			mpGreatSword->Rotation(SWORD_OFFSET_ROT);
			// 待機状態へ移行
			ChangeState(EState::eIdle);
			ChangeAnimation(EAnimType::eIdle);
		}
		break;
	}
}

// ジャンプ開始
void CPlayer::UpdateJumpStart()
{
	ChangeAnimation(EAnimType::eJumpStart);
	ChangeState(EState::eJump);

	mMoveSpeedY += JUMP_SPEED;
	mIsGrounded = false;
}

// ジャンプ中
void CPlayer::UpdateJump()
{
	if (mMoveSpeedY <= 0.0f)
	{
		ChangeAnimation(EAnimType::eJumpEnd);
		ChangeState(EState::eJumpEnd);
	}
}

// ジャンプ終了
void CPlayer::UpdateJumpEnd()
{
	// ジャンプアニメーションが終了かつ、
	// 地面に接地したら、待機状態へ戻す
	if (IsAnimationFinished() && mIsGrounded)
	{
		ChangeState(EState::eIdle);
	}
}

void CPlayer::UpdateAvoidR()
{
	switch (mStateStep)
	{
	case 0:
		// 回避アニメーションを開始
		ChangeAnimation(EAnimType::eAvoidR, true);
		mStateStep++;
		break;
	case 1:
		if (GetAnimationFrame() >= 20.0f && !mAvoidMoving)
		{
			SetInvincible(true);
			mAvoidMoving = true;
			mStateStep++;
		}
		break;
	case 2:
		if (mAvoidMoving)
		{
			// 1秒あたりの移動速度
			mMoveSpeed = mAvoidVec * 150.0f * Times::DeltaTime();

			if (GetAnimationFrame() >= 50.0f)
			{
				mAvoidMoving = false;
				mMoveSpeed = CVector::zero;
				mStateStep++;
			}
		}
		break;
	case 3:
		// 回避アニメーションが終了したら
		if (IsAnimationFinished())
		{
			if (!mInJustAction)
			{
				SetInvincible(false);
				mIsGravity = true;
				Times::SetTimeScale(1.0f);
				// 待機状態へ移行
				ChangeState(EState::eIdle);
				ChangeAnimation(EAnimType::eIdle);
			}
			else
			{
				mIsGravity = true;
				Times::SetTimeScale(0.1f);
				// スライド斬り攻撃状態へ移行
				ChangeState(EState::eSlideAtt);
			}
			mInJustAction = false;
		}
		break;
	}
}

void CPlayer::UpdateAvoidL()
{
	switch (mStateStep)
	{
	case 0:
		// 回避アニメーションを開始
		ChangeAnimation(EAnimType::eAvoidL, true);
		mStateStep++;
		break;
	case 1:
		if (GetAnimationFrame() >= 20.0f && !mAvoidMoving)
		{
			SetInvincible(true);
			mAvoidMoving = true;
			mStateStep++;
		}
		break;
	case 2:
		if (mAvoidMoving)
		{
			// 1秒あたりの移動速度
			mMoveSpeed = mAvoidVec * 150.0f * Times::DeltaTime();

			if (GetAnimationFrame() >= 50.0f)
			{
				mAvoidMoving = false;
				mMoveSpeed = CVector::zero;
				mStateStep++;
			}
		}
		break;
	case 3:
		// 回避アニメーションが終了したら
		if (IsAnimationFinished())
		{
			if (!mInJustAction)
			{
				SetInvincible(false);
				mIsGravity = true;
				Times::SetTimeScale(1.0f);
				// 待機状態へ移行
				ChangeState(EState::eIdle);
				ChangeAnimation(EAnimType::eIdle);
			}
			else
			{
				mIsGravity = true;
				Times::SetTimeScale(0.1f);
				// スライド斬り攻撃状態へ移行
				ChangeState(EState::eSlideAtt);
			}
			mInJustAction = false;
		}
		break;
	}
}

void CPlayer::UpdateAvoidB()
{
	{
		switch (mStateStep)
		{
		case 0:
			// 回避アニメーションを開始
			ChangeAnimation(EAnimType::eAvoidB, true);
			mStateStep++;
			break;
		case 1:
			if (GetAnimationFrame() >= 12.0f && !mAvoidMoving)
			{
				SetInvincible(true);
				mAvoidMoving = true;
				mStateStep++;
			}
			break;
		case 2:
			if (mAvoidMoving)
			{
				// 1秒あたりの移動速度
				mMoveSpeed = mAvoidVec * 125.0f * Times::DeltaTime();

				if (GetAnimationFrame() >= 52.0f)
				{
					mAvoidMoving = false;
					mMoveSpeed = CVector::zero;
					mStateStep++;
				}
			}
			break;
		case 3:
			// 回避アニメーションが終了したら
			if (IsAnimationFinished())
			{
				if (!mInJustAction)
				{
					SetInvincible(false);
					mIsGravity = true;
					Times::SetTimeScale(1.0f);
					// 待機状態へ移行
					ChangeState(EState::eIdle);
					ChangeAnimation(EAnimType::eIdle);
				}
				else
				{
					mIsGravity = true;
					Times::SetTimeScale(0.1f);
					// スライド斬り攻撃状態へ移行
					ChangeState(EState::eSlideAtt);
				}
				mInJustAction = false;
			}
			break;
		}
	}
}

// 仰け反り
void CPlayer::UpdateHit()
{
	if (!mIsGravity) mIsGravity = true;
	switch (mStateStep)
	{
		case 0:
			// 先行入力コライダーは最初はオフにしておく
			mpTACol->SetEnable(false);
			// 仰け反りアニメーションを開始
			ChangeAnimation(EAnimType::eHit, true);
			mStateStep++;
			break;
		case 1:
			// 仰け反りアニメーションが終了したら
			if (IsAnimationFinished())
			{
				// 待機状態へ移行
				ChangeState(EState::eIdle);
				ChangeAnimation(EAnimType::eIdle);
			}
			break;
	}
}

void CPlayer::UpdateDeath()
{
	switch (mStateStep)
	{
	case 0:
		// 死亡アニメーションを開始
		ChangeAnimation(EAnimType::eDeath, true);
		mDeathVec = -VectorZ();
		mToDeath = true;
		mDeathTimer = 0.0f;
		SetInvincible(true);
		mStateStep++;
		break;
	case 1:
		if (GetAnimationFrame() >= DEATH_END_FRAME)
		{
			mToDeath = false;
			mMoveSpeed = CVector::zero;
			mStateStep++;
		}

		if (mToDeath)
		{
			mDeathTimer += Times::DeltaTime();

			// 1秒あたりの移動速度
			mMoveSpeed = mDeathVec * 20.0f * Times::DeltaTime();
		}
		break;
	case 2:
		// 死亡アニメーションが終了したら、
		if (IsAnimationFinished())
		{
			mStateStep++;
		}
		break;
	case 3:
		
		break;
	}
}

void CPlayer::UpdateVictory()
{
	switch (mStateStep)
	{
	case 0:
		mMoveSpeed = CVector::zero;
		// 勝利アニメーションを再生
		ChangeAnimation(EAnimType::eVictory);
		mStateStep++;
		break;
	case 1:
		mStateStep++;
		break;
	case 2:
		break;
	}
}

void CPlayer::AvoidJudge()
{
	// 右クリックで回避へ移行
	if (CInput::PushKey(VK_RBUTTON) && CInput::Key('D') && mSt >= mAvoidCost)
	{
		mpTACol->SetEnable(false);
		// ジャストタイムか
		if (mInTypeAhead)
		{
			Times::SetTimeScale(0.25f);
			SetInvincible(true);
			mInJustAction = true;

			// チュートリアル用
			mJustAvoidCount++;
		}
		mNextAttack = false;
		CCharaBase::UseStamina(mAvoidCost);
		mMoveSpeed = CVector::zero;
		// プレイヤーの移動ベクトルを求める
		mAvoidVec = CalcMoveVec();
		ChangeState(EState::eAvoidR);
		mIsGravity = false;
	}
	else if (CInput::PushKey(VK_RBUTTON) && CInput::Key('A') && mSt >= mAvoidCost)
	{
		mpTACol->SetEnable(false);
		// ジャストタイムか
		if (mInTypeAhead)
		{
			Times::SetTimeScale(0.2f);
			SetInvincible(true);
			mInJustAction = true;

			// チュートリアル用
			mJustAvoidCount++;
		}
		mNextAttack = false;
		CCharaBase::UseStamina(mAvoidCost);
		mMoveSpeed = CVector::zero;
		// プレイヤーの移動ベクトルを求める
		mAvoidVec = CalcMoveVec();
		ChangeState(EState::eAvoidL);
		mIsGravity = false;
	}
	else if (CInput::PushKey(VK_RBUTTON) && mSt >= mAvoidCost)
	{
		mpTACol->SetEnable(false);
		// ジャストタイムか
		if (mInTypeAhead)
		{
			Times::SetTimeScale(0.2f);
			SetInvincible(true);
			mInJustAction = true;

			// チュートリアル用
			mJustAvoidCount++;
		}
		mNextAttack = false;
		CCharaBase::UseStamina(mAvoidCost);
		mMoveSpeed = CVector::zero;
		// プレイヤーの移動ベクトルを求める
		mAvoidVec = -VectorZ();
		ChangeState(EState::eAvoidB);
		mIsGravity = false;
	}
}

// オブジェクト削除を伝える
void CPlayer::DeleteObject(CObjectBase* obj)
{
	// 剣が先に削除されたら、剣のポインタを初期化
	if (mpGreatSword == obj)
	{
		mpGreatSword = nullptr;
	}
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
		// 上方向ベクトル(設置している場合は、地面の法線)
		CVector up = mIsGrounded ? mGroundNormal : CVector::up;
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
	if (move.LengthSqr() > 0.0f && CInput::Key(VK_LSHIFT))
	{
		mpGreatSword->Rotation(DASH_SWORD_OFFSET_ROT);
		mMoveSpeed += move * RUN_SPEED * Times::DeltaTime();

		// 待機状態であれば、歩行アニメーションに切り替え
		if (mState == EState::eIdle)
		{
			ChangeAnimation(EAnimType::eRun);
		}
	}
	else if (move.LengthSqr() > 0.0f)
	{
		mMoveSpeed += move * MOVE_SPEED * Times::DeltaTime();

		// 待機状態であれば、歩行アニメーションに切り替え
		if (mState == EState::eIdle)
		{
			ChangeAnimation(EAnimType::eWalk);
		}
	}
	// 移動キーを入力していない
	else
	{
		// 待機状態であれば、待機アニメーションに切り替え
		if (mState == EState::eIdle)
		{
			mpGreatSword->Rotation(SWORD_OFFSET_ROT);
			ChangeAnimation(EAnimType::eIdle);
		}
	}
}

// モーションブラーの更新処理
void CPlayer::UpdateMotionBlur()
{
	// モーションブラーの残り時間が残っていなければ、処理しない
	if (mMotionBlurRemainTime <= 0.0f) return;
	// 現在のカメラを取得し、存在しなければ処理しない
	CCamera* currentCamera = CCamera::CurrentCamera();
	if (currentCamera == nullptr) return;

	// カメラの向きと反対方向へブラーを掛けるため、
	// 反転したカメラの向きを取得
	CVector camDir = -currentCamera->VectorZ();

	// 残り時間から経過時間の割合を取得（経過時間の割合 = 1 - 残り時間の割合）
	float percent = 1.0f - mMotionBlurRemainTime / MOTION_BLUR_TIME;
	// ブラーの幅をサインカーブで経過時間に合わせて増減させる
	float ratio = sinf(M_PI * percent);
	float width = MOTION_BLUR_WIDTH * ratio;

	// モーションブラーのパラメータを設定
	System::SetMotionBlur(camDir, width, MOTION_BLUR_COUNT);

	// 残り時間を経過時間分減少させる
	mMotionBlurRemainTime -= Times::UnscaledDeltaTime();
	// 残り時間がなくなれば、
	if (mMotionBlurRemainTime <= 0.0f)
	{
		// モーションブラーをオフにする
		System::SetEnableMotionBlur(false);
		mMotionBlurRemainTime = 0.0f;
	}
}

void CPlayer::LookCameraForward()
{
	// カメラの向きを取得（Y軸のみ）
	CCamera* camera = CCamera::MainCamera();
	if (camera)
	{
		CVector camForward = -camera->VectorZ();
		camForward.Y(0.0f);          // Y成分を無視（水平回転のみ）

		if (camForward.LengthSqr() > 0.0001f)
		{
			camForward.Normalize();
			Rotation(CQuaternion::LookRotation(camForward));
		}
	}
}

// ロックオン関連
void CPlayer::LockOnTarget()
{
	CEnemy* target = CEnemyManager::Instance()->GetLockOnTarget(LOCKON_DISTANCE);
	if (target == nullptr) return;

	mpLockOnTarget = target;
	CCamera::CurrentCamera()->SetLockOnTarget(mpLockOnTarget);
	mpTargetUI->SetShow(true);
	mIsLockOn = true;
}

void CPlayer::UnLockTarget()
{
	mpLockOnTarget = nullptr;
	CCamera::CurrentCamera()->SetLockOnTarget(nullptr);
	mpTargetUI->SetShow(false);
	mIsLockOn = false;
}

void CPlayer::UpdateLockOn()
{
	if (CInput::PullKey('T'))
	{
		if (!mIsLockOn) LockOnTarget();
		else UnLockTarget();
	}

	if (mIsLockOn)
	{
		// ロックオン対象が存在しなくなれば解除する
		if (!CEnemyManager::Instance()->Exist(mpLockOnTarget))
		{
			UnLockTarget();
			return;
		}
		CCamera* camera = CCamera::CurrentCamera();
		CVector cameraPos = camera->Position();
		CVector targetPos = mpLockOnTarget->Position() + mpLockOnTarget->GetLockOnOffsetPos();

		// ロックオン範囲外に出たらロックオン解除
		float dist = CVector::Distance(cameraPos, targetPos);
		if (dist > LOCKON_DISTANCE)
		{
			UnLockTarget();
			return;
		}

		// ロックオンUIに対象の座標を設定
		mpTargetUI->SetTargetPos(targetPos);
	}
	else
	{
		UnLockTarget();
	}
}

// 更新
void CPlayer::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;

	// スキル
	if (mS1CastTime < mS1RecastTime)
	{
		float capa = mS1RecastTime - mS1CastTime;
		float amount = 1 * Times::DeltaTime();
		if (capa >= amount)
		{
			mS1CastTime += amount;
		}
		else
		{
			mS1CastTime = mS1RecastTime;
		}
	}
	

	/*
	CModelXFrame* frame = mpModel->FinedFrame("Armature_mixamorig_RightHand");
	//mpSword->SetAttachMtx(&frame->CombinedMatrix());

	CMatrix SetAttachM = frame->CombinedMatrix();
	
	mpBodyCol->Set
	(
		this, ELayer::ePlayer,
		CVector(0.0f, BODY_RADIUS, 0.0f),
		CVector(frame->CombinedMatrix().Position().X(),
			frame->CombinedMatrix().Position().Y(),
			frame->CombinedMatrix().Position().Z()),
		BODY_RADIUS
	);
	*/

	// 状態に合わせて、更新処理を切り替える
	switch (mState)
	{
		// 戦闘準備状態
		case EState::eReserve:		UpdateReserve();		break;
		// 待機状態
		case EState::eIdle:			UpdateIdle();			break;
		// 斬り攻撃1
		case EState::eAttack1:		UpdateAttack1();		break;
		// 斬り攻撃2
		case EState::eAttack2:		UpdateAttack2();		break;
		// 斬り攻撃X
		case EState::eAttackX:		UpdateAttackX();		break;
		// 蹴り攻撃
		case EState::eKick:			UpdateKick();			break;
		// スライド斬り攻撃
		case EState::eSlideAtt:		UpdateSlideAttack();	break;
		// ジャンプ開始
		case EState::eJumpStart:	UpdateJumpStart();		break;
		// ジャンプ中
		case EState::eJump:			UpdateJump();			break;
		// ジャンプ終了
		case EState::eJumpEnd:		UpdateJumpEnd();		break;
		// 回避:右
		case EState::eAvoidR:		UpdateAvoidR();			break;
		// 回避:左
		case EState::eAvoidL:		UpdateAvoidL();			break;
		// 回避:後ろ
		case EState::eAvoidB:		UpdateAvoidB();			break;
		// 仰け反り
		case EState::eHit:			UpdateHit();			break;
		// 死亡
		case EState::eDeath:		UpdateDeath();			break;
		// 勝利
		case EState::eVictory:		UpdateVictory();		break;
	}

	// 待機中とジャンプ中は、移動処理を行う
	if (mState == EState::eIdle
		|| mState == EState::eJumpStart
		|| mState == EState::eJump
		|| mState == EState::eJumpEnd)
	{
		UpdateMove();
	}

	if (mIsGravity)
	{
		mMoveSpeedY -= GRAVITY;
	}
	CVector moveSpeed = mMoveSpeed + CVector(0.0f, mMoveSpeedY, 0.0f);

	// 移動
	Position(Position() + moveSpeed);

	if (
		mState != EState::eDeath &&
		mState != EState::eAvoidR &&
		mState != EState::eAvoidL &&
		mState != EState::eAvoidB
		)
	{
		// プレイヤーを移動方向へ向ける
		CVector current = VectorZ();
		CVector target = moveSpeed;
		target.Y(0.0f);
		target.Normalize();
		CVector forward = CVector::Slerp(current, target, 0.125f);
		Rotation(CQuaternion::LookRotation(forward));
	}

	// モーションブラーの更新処理
	UpdateMotionBlur();

	UpdateLockOn();

	// キャラクターの更新
	CXCharacter::Update();

	// 武器の行列を更新
	mpGreatSword->UpdateMtx();

	mIsGrounded = false;

	if (Position().Y() < -100.0f)
	{
		Position(0.0f, 10.0f, 100.0f);
	}

	// インジケーターを更新
	CMatrix mtx = mpHeadFrame->CombinedMatrix();
	mpIndicator->Position(mtx.Position() + mIndicatorOffsetPos);

#ifdef _DEBUG
	CDebugPrint::Print("PlayerHP:%f / %f\n", mHp, mMaxHp);
	CDebugPrint::Print("PlayerST:%f / %f\n", mSt, mMaxSt);
	CVector pos = Position();
	CDebugPrint::Print("PlayerPos:%.2f, %.2f, %.2f\n", pos.X(), pos.Y(), pos.Z());
	CDebugPrint::Print("FPS:%f\n", Times::FPS());
	CDebugPrint::Print("PlayerGrounded:%s\n", mIsGrounded ? "true" : "false");
	CDebugPrint::Print("PlayerState:%d\n", mState);
	CDebugPrint::Print("PlayerInTACol:%s\n", mInTypeAhead ? "true" : "false");
#endif // _DEBUG
}

void CPlayer::LastUpdate()
{
	mpIndicator->SetShow(false);
	mInTypeAhead = false;
}

void CPlayer::SetInBattle(int state)
{
	if (state == 0)
	{
		// 待機状態へ移行
		ChangeState(EState::eIdle);
	}
	if (state == 2)
	{
		// 勝利状態へ移行
		ChangeState(EState::eVictory);
	}
}

// 攻撃中か
bool CPlayer::IsAttacking() const
{
	// 斬り攻撃中
	if (mState == EState::eAttack1) return true;
	// 蹴り攻撃中
	if (mState == EState::eKick) return true;

	// 攻撃中でない
	return false;
}

// 攻撃開始
void CPlayer::AttackStart()
{
	// 先行入力コライダーをオフにする
	mpTACol->SetEnable(false);

	// ベースクラスの攻撃開始処理を呼び出し
	CXCharacter::AttackStart();

	// 斬り攻撃中であれば、剣のコライダーをオンにする
	if (mState == EState::eAttack1
		|| mState == EState::eAttack2
		|| mState == EState::eAttackX
		|| mState == EState::eSlideAtt)
	{
		mpGreatSword->SetEnableCol(true);
	}
	// 蹴り攻撃中であれば、蹴り攻撃用のコライダーをオンにする
	else if (mState == EState::eKick)
	{
		mpKickCol->SetEnable(true);
	}
}

// 攻撃終了
void CPlayer::AttackEnd()
{
	// ベースクラスの攻撃終了処理を呼び出し
	CXCharacter::AttackEnd();

	// 攻撃コライダーをオフ
	mpGreatSword->SetEnableCol(false);
	mpKickCol->SetEnable(false);
}

// ダメージを受ける
void CPlayer::TakeDamage(float damage, CObjectBase* causer)
{
	// ベースクラスのダメージ処理を呼び出す
	CXCharacter::TakeDamage(damage, causer);

	// 死亡していなければ、
	if (!IsDeath())
	{
		// スーパーアーマー状態でなければのけぞる
		if (!mIsSA)
		{
			// 仰け反り状態へ移行
			ChangeState(EState::eHit);

			// 攻撃を加えた相手の方向へ向く
			CVector targetPos = causer->Position();
			CVector vec = targetPos - Position();
			vec.Y(0.0f);
			Rotation(CQuaternion::LookRotation(vec.Normalized()));
		}
		
		// 移動を停止
		mMoveSpeed = CVector::zero;
	}
	else
	{
		// 攻撃を加えた相手の方向へ向く
		CVector targetPos = causer->Position();
		CVector vec = targetPos - Position();
		vec.Y(0.0f);
		Rotation(CQuaternion::LookRotation(vec.Normalized()));
		// 移動を停止
		mMoveSpeed = CVector::zero;
		// 死亡状態へ移行
		ChangeState(EState::eDeath);
	}
}

void CPlayer::SetInvincible(bool invincible)
{
	if (!invincible)
	{
		// 当たり判定を通常のレイヤー設定にする
		mpBodyCol->SetCollisionLayers({ ELayer::eField, ELayer::eWall, ELayer::eEnemy, ELayer::eAttackCol, ELayer::eTypeAheadCol });
	}
	else
	{
		// 当たり判定を無敵のレイヤー設定にする
		mpBodyCol->SetCollisionLayers({ ELayer::eField, ELayer::eWall, ELayer::eEnemy });
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

				// 接地した
				mIsGrounded = true;
				// 接地した地面の法線を記憶しておく
				mGroundNormal = hit.adjust.Normalized();

				if (other->Tag() == ETag::eRideableObject)
				{
					mpRideObject = other->Owner();
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
		// 壁との衝突
		else if (other->Layer() == ELayer::eWall)
		{
			// 横方向の押し戻しのため、押し戻しベクトルのYの値を0にする
			CVector adjust = hit.adjust;
			adjust.Y(0.0f);

			Position(Position() + adjust * hit.weight);
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

		// 先行入力用コライダーがヒットした
		if (other->Layer() == ELayer::eTypeAheadCol)
		{
			mpIndicator->SetShow(true);
			mInTypeAhead = true;
		}
	}
	// 剣のコライダーが衝突した
	else if (self == mpGreatSword->Collider())
	{
		CCharaBase* hitChara = dynamic_cast<CCharaBase*>(other->Owner());
		if (hitChara != nullptr && !IsAttackHitObj(hitChara))
		{
			AddAttackHitObj(hitChara);

			// チュートリアル用
			mAttHitCount++;

			// 状態に合わせて、更新処理を切り替える
			switch (mState)
			{
				// 斬り攻撃1
			case EState::eAttack1:
				{
					// 血のエフェクトを作成
					CBlood* blood = new CBlood(ETag::eFlame, hitChara, 1.0f);
					hitChara->TakeDamage(4.0f * mAttackMag, this);	break;
				}
				
				// 斬り攻撃2
			case EState::eAttack2:
				{
					// 血のエフェクトを作成
					CBlood* blood = new CBlood(ETag::eFlame, hitChara, 1.0f);
					hitChara->TakeDamage(6.0f * mAttackMag, this);	break;
				}

				// 斬り攻撃X
			case EState::eAttackX:
				{
					// 血のエフェクトを作成
					CBlood* blood = new CBlood(ETag::eFlame, hitChara, 1.0f);
					hitChara->TakeDamage(5.0f * mAttackMag, this);	break;
				}
				
				// スライド斬り攻撃
			case EState::eSlideAtt:
				{
					// 血のエフェクトを作成
					CBlood* blood = new CBlood(ETag::eFlame, hitChara, 1.5f);
					hitChara->TakeDamage(8.0f * mAttackMag, this);	break;
				}
			}
		}
	}
	// 蹴り攻撃のコライダーが衝突した
	else if (self == mpKickCol)
	{
		CCharaBase* hitChara = dynamic_cast<CCharaBase*>(other->Owner());
		if (hitChara != nullptr && !IsAttackHitObj(hitChara))
		{
			// Enemyかどうかを判定
			CEnemy* hitEnemy = dynamic_cast<CEnemy*>(hitChara);

			if (mInJustAction && hitEnemy != nullptr)
			{
				mJustKickHitCount++;
				hitEnemy->SetGuardBreak(true);
			}
			else if (hitEnemy != nullptr)
			{
				mKickHitCount++;
			}

			AddAttackHitObj(hitChara);
			hitEnemy->TakeKick(this);
		}
	}
}

// 描画
void CPlayer::Render()
{
	CXCharacter::Render();
}

float CPlayer::GetMaxS1()
{
	return mS1RecastTime;
}

float CPlayer::GetS1()
{
	return mS1CastTime;
}
