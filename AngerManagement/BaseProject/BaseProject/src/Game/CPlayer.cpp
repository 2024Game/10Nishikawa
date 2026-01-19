//プレイヤークラスのインクルード
#include "CPlayer.h"
#include "CInput.h"
#include "CCamera.h"
#include "CBullet.h"
#include "CFlamethrower.h"
#include "CSlash.h"
#include "Maths.h"
#include "CGreatSword.h"
#include "CColliderCapsule.h"
#include "CColliderSphere.h"
#include "CTargetUI.h"
#include "CEnemy.h"
#include "CEnemyManager.h"

// アニメーションのパス
#define ANIM_PATH "Character\\Player\\Anims\\"
#define BODY_HEIGHT 16.0f	// 本体のコライダーの高さ
#define BODY_RADIUS 3.0f	// 本体のコライダーの幅
#define MOVE_SPEED 0.3f		// 移動速度
#define RUN_SPEED 0.9f		// 移動速度
#define JUMP_SPEED 1.5f		// ジャンプ速度
#define GRAVITY 0.0625f		// 重力加速度

#define MOTION_BLUR_TIME 3.0f	// モーションブラーを掛ける時間
#define MOTION_BLUR_WIDTH 1.0f	// モーションブラーの幅
#define MOTION_BLUR_COUNT 5		// モーションブラーの反復回数

#define LOCKON_DISTANCE 300.0f

//#define ATTACK_START_FRAME 26.0f	// 斬り攻撃の開始フレーム
//#define ATTACK_END_FRAME 50.0f		// 斬り攻撃の終了フレーム

#define ATTACK1_START_FRAME 25.0f	// 斬り攻撃1の開始フレーム
#define ATTACK1_END_FRAME 55.0f		// 斬り攻撃1の終了フレーム
#define ATTACK2_START_FRAME 35.0f	// 斬り攻撃2の開始フレーム
#define ATTACK2_END_FRAME 100.0f	// 斬り攻撃2の終了フレーム
#define ATTACKX_START_FRAME 35.0f	// 斬り攻撃Xの開始フレーム
#define ATTACKX_END_FRAME 210.0f	// 斬り攻撃Xの終了フレーム
#define DEATH_END_FRAME 110.0f		// 死亡の終了フレーム



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
	{ ANIM_PATH"idle.x",		true,	115.0f,	1.0f	},	// 待機
	{ ANIM_PATH"walk.x",		true,	82.0f,	1.5f	},	// 歩行
	{ ANIM_PATH"run.x",			true,	48.0f,	1.5f	},	// ダッシュ
	{ "",						true,	0.0f,	1.0f	},	// 斬り攻撃
	{ "",						true,	0.0f,	1.0f	},	// 斬り攻撃
	{ "",						true,	0.0f,	1.0f	},	// 斬りかかり攻撃
	{ "",						true,	0.0f,	1.0f	},	// 蹴り攻撃
	{ "",						true,	0.0f,	1.0f	},	// ジャンプ開始
	{ "",						true,	0.0f,	1.0f	},	// ジャンプ中
	{ "",						true,	0.0f,	1.0f	},	// ジャンプ終了
	{ "",						true,	0.0f,	1.0f	},	// 回避:右
	{ "",						true,	0.0f,	1.0f	},	// 回避:左
	{ "",						true,	0.0f,	1.0f	},	// 仰け反り
	{ "",						true,	0.0f,	1.0f	},	// 死亡
	{ "",						true,	0.0f,	1.0f	},	// 勝利
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
	, mA1StCost(25.0f)
	, mAvoidStCost(20.0f)
	, mpSaveManager(SaveManager)
	, mIsLockOn(false)
	, mpLockOnTarget(nullptr)
	, mpTargetUI(nullptr)
{
	mMaxHp = mpSaveManager->data.maxHp;
	mHp = mpSaveManager->data.hp;
	mMaxSt = 100.0f + (mpSaveManager->data.stLv * 5);
	mSt = mMaxSt;
	mAttackMag = 1.0f + (mpSaveManager->data.attackLv * 0.05f);
	mStRegeneMag = 1.0f + (mpSaveManager->data.stRegeneLv * 0.01f);
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
	mpBodyCol->SetCollisionLayers({ ELayer::eField, ELayer::eEnemy, ELayer::eAttackCol });

	mpSlashSE = CResourceManager::Get<CSound>("SlashSound");

	mpFlamethrower = new CFlamethrower
	(
		this, nullptr,
		CVector(0.0f, 14.0f, -1.0f),
		CQuaternion(0.0f, 90.0f, 0.0f).Matrix()
	);

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
		CCharaBase::GainStamina(7.5 * Times::DeltaTime() * mStRegeneMag);
	}

	// 接地していれば、
	if (mIsGrounded)
	{
		// 左クリックで斬撃攻撃へ移行
		if (CInput::PushKey(VK_LBUTTON) && mSt >= mA1StCost)
		{
			CCharaBase::UseStamina(mA1StCost);

			// 先行入力コライダーをオンにする
			mpTACol->SetEnable(true);

			mMoveSpeed = CVector::zero;
			ChangeState(EState::eAttack1);
		}
		// Cでキック攻撃へ移行
		else if (CInput::PushKey('C'))
		{
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
		// 右クリックで回避へ移行
		else if (CInput::PushKey(VK_RBUTTON) && CInput::Key('D') && mSt >= mAvoidStCost)
		{
			CCharaBase::UseStamina(mAvoidStCost);
			mMoveSpeed = CVector::zero;
			// プレイヤーの移動ベクトルを求める
			mAvoidVec = CalcMoveVec();
			ChangeState(EState::eAvoidR);
			mIsGravity = false;
		}
		else if (CInput::PushKey(VK_RBUTTON) && CInput::Key('A') && mSt >= mAvoidStCost)
		{
			CCharaBase::UseStamina(mAvoidStCost);
			mMoveSpeed = CVector::zero;
			// プレイヤーの移動ベクトルを求める
			mAvoidVec = CalcMoveVec();
			ChangeState(EState::eAvoidL);
			mIsGravity = false;
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
					if (mSt >= mA1StCost)
					{
						CCharaBase::UseStamina(mA1StCost);
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
				if (mSt >= mA1StCost)
				{
					CCharaBase::UseStamina(mA1StCost);
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
			mpBodyCol->SetCollisionLayers({ ELayer::eField, ELayer::eEnemy });
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
			mpBodyCol->SetCollisionLayers({ ELayer::eField, ELayer::eEnemy, ELayer::eAttackCol });
			mIsGravity = true;
			// 待機状態へ移行
			ChangeState(EState::eIdle);
			ChangeAnimation(EAnimType::eIdle);
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
			mpBodyCol->SetCollisionLayers({ ELayer::eField, ELayer::eEnemy });
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
			mpBodyCol->SetCollisionLayers({ ELayer::eField, ELayer::eEnemy, ELayer::eAttackCol });
			mIsGravity = true;
			// 待機状態へ移行
			ChangeState(EState::eIdle);
			ChangeAnimation(EAnimType::eIdle);
		}
		break;
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
		mpBodyCol->SetCollisionLayers({ ELayer::eField, ELayer::eEnemy });
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
	if (CInput::PushKey(VK_RBUTTON) && CInput::Key('D') && mSt >= mAvoidStCost)
	{
		mNextAttack = false;
		CCharaBase::UseStamina(mAvoidStCost);
		mMoveSpeed = CVector::zero;
		// プレイヤーの移動ベクトルを求める
		mAvoidVec = CalcMoveVec();
		ChangeState(EState::eAvoidR);
		mIsGravity = false;
	}
	else if (CInput::PushKey(VK_RBUTTON) && CInput::Key('A') && mSt >= mAvoidStCost)
	{
		mNextAttack = false;
		CCharaBase::UseStamina(mAvoidStCost);
		mMoveSpeed = CVector::zero;
		// プレイヤーの移動ベクトルを求める
		mAvoidVec = CalcMoveVec();
		ChangeState(EState::eAvoidL);
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
		mMoveSpeed += move * RUN_SPEED;

		// 待機状態であれば、歩行アニメーションに切り替え
		if (mState == EState::eIdle)
		{
			ChangeAnimation(EAnimType::eRun);
		}
	}
	else if (move.LengthSqr() > 0.0f)
	{
		mMoveSpeed += move * MOVE_SPEED;

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
	mMotionBlurRemainTime -= Times::DeltaTime();
	// 残り時間がなくなれば、
	if (mMotionBlurRemainTime <= 0.0f)
	{
		// モーションブラーをオフにする
		System::SetEnableMotionBlur(false);
		mMotionBlurRemainTime = 0.0f;
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
		case EState::eReserve:		UpdateReserve();	break;
		// 待機状態
		case EState::eIdle:			UpdateIdle();		break;
		// 斬り攻撃1
		case EState::eAttack1:		UpdateAttack1();	break;
		// 斬り攻撃2
		case EState::eAttack2:		UpdateAttack2();	break;
		// 斬り攻撃X
		case EState::eAttackX:		UpdateAttackX();	break;
		// 蹴り攻撃
		case EState::eKick:			UpdateKick();		break;
		// ジャンプ開始
		case EState::eJumpStart:	UpdateJumpStart();	break;
		// ジャンプ中
		case EState::eJump:			UpdateJump();		break;
		// ジャンプ終了
		case EState::eJumpEnd:		UpdateJumpEnd();	break;
		// 回避:右
		case EState::eAvoidR:		UpdateAvoidR();		break;
		// 回避:左
		case EState::eAvoidL:		UpdateAvoidL();		break;
		// 仰け反り
		case EState::eHit:			UpdateHit();		break;
		// 死亡
		case EState::eDeath:		UpdateDeath();		break;
		// 勝利
		case EState::eVictory:		UpdateVictory();	break;
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

	// プレイヤーを移動方向へ向ける
	CVector current = VectorZ();
	CVector target = moveSpeed;
	target.Y(0.0f);
	target.Normalize();
	CVector forward = CVector::Slerp(current, target, 0.125f);
	Rotation(CQuaternion::LookRotation(forward));

	// ホイールクリックで弾丸発射
	if (CInput::PushKey(VK_MBUTTON))
	{
		// 弾丸を生成
		new CBullet
		(
			// 発射位置
			Position() + CVector(0.0f, 10.0f, 0.0f) + VectorZ() * 5.0f,
			VectorZ(),	// 発射方向
			1000.0f,	// 移動距離
			1000.0f		// 飛距離
		);
	}

	// 「E」キーで炎の発射をオンオフする
	if (CInput::PushKey('E'))
	{
		if (!mpFlamethrower->IsThrowing())
		{
			mpFlamethrower->Start();
		}
		else
		{
			mpFlamethrower->Stop();
		}
	}

	// 「P」キーを押したら、ゲームを終了
	if (CInput::PushKey('P'))
	{
		System::ExitGame();
	}

	// 「B」キーを押したら、モーションブラー開始
	if (CInput::PushKey('B'))
	{
		// モーションブラーを掛けている最中であれば、
		// 新しくモーションブラーを掛け直さない
		if (mMotionBlurRemainTime <= 0.0f)
		{
			System::SetEnableMotionBlur(true);
			mMotionBlurRemainTime = MOTION_BLUR_TIME;
		}
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

#ifdef _DEBUG
	CDebugPrint::Print("PlayerHP:%f / %f\n", mHp, mMaxHp);
	CDebugPrint::Print("PlayerST:%f / %f\n", mSt, mMaxSt);
	CVector pos = Position();
	CDebugPrint::Print("PlayerPos:%.2f, %.2f, %.2f\n", pos.X(), pos.Y(), pos.Z());
	CDebugPrint::Print("FPS:%f\n", Times::FPS());
	CDebugPrint::Print("PlayerGrounded:%s\n", mIsGrounded ? "true" : "false");
	CDebugPrint::Print("PlayerState:%d\n", mState);
#endif // _DEBUG
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
	if (mState == EState::eAttack1 || mState == EState::eAttack2 || mState == EState::eAttackX)
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
		// 仰け反り状態へ移行
		ChangeState(EState::eHit);

		// 攻撃を加えた相手の方向へ向く
		CVector targetPos = causer->Position();
		CVector vec = targetPos - Position();
		vec.Y(0.0f);
		Rotation(CQuaternion::LookRotation(vec.Normalized()));

		// 移動を停止
		mMoveSpeed = CVector::zero;
	}
	else
	{
		// 移動を停止
		mMoveSpeed = CVector::zero;
		// 死亡状態へ移行
		ChangeState(EState::eDeath);
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
	// 剣のコライダーが衝突した
	else if (self == mpGreatSword->Collider())
	{
		CCharaBase* hitChara = dynamic_cast<CCharaBase*>(other->Owner());
		if (hitChara != nullptr && !IsAttackHitObj(hitChara))
		{
			AddAttackHitObj(hitChara);
			// 状態に合わせて、更新処理を切り替える
			switch (mState)
			{
				// 斬り攻撃1
			case EState::eAttack1:
				hitChara->TakeDamage(4.0f * mAttackMag, this);	break;
				// 斬り攻撃2
			case EState::eAttack2:
				hitChara->TakeDamage(6.0f * mAttackMag, this);	break;
				// 斬り攻撃X
			case EState::eAttackX:
				hitChara->TakeDamage(5.0f * mAttackMag, this);	break;
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

// 描画
void CPlayer::Render()
{
	CXCharacter::Render();
}
