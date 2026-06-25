#include "CHeavyWarrior.h"
#include "CColliderCapsule.h"
#include "CColliderSphere.h"
#include "CBusterSword.h"
#include "Maths.h"
#include "CEnemyStatusLoader.h"
#include "CCollisionManager.h"

// アニメーションのパス
#define ANIM_PATH "Character\\TestPlayer\\Anims\\"
#define BODY_HEIGHT 18.0f	// 本体のコライダーの高さ
#define BODY_RADIUS 3.5f	// 本体のコライダーの幅
#define MOVE_SPEED 13.5f	// 移動速度
#define RUN_SPEED 27.0f		// ダッシュ移動速度
#define JUMP_SPEED 1.5f		// ジャンプ速度
#define GRAVITY 0.0625f		// 重力加速度

#define GAUGE_OFFSET_Y 27.0f		// HP・STゲージオフセット位置(高さ)
#define BUFF_GAUGE_OFFSET_Y 23.0f	// バフゲージオフセット位置(高さ)
#define DEATH_WAIT_TIME 3.0f

#define LOOKAT_SPEED 90.0f
#define DASH_DIST 75.0f				// 駆け寄ってくる距離
#define ATTACK_RANGE 41.5f			// 攻撃を行う距離

#define AT_GRACE_FRAME 5.0f			// 先行入力フレーム
#define ATTACK1_START_FRAME 25.0f	// 斬り攻撃1の開始フレーム
#define ATTACK1_END_FRAME 55.0f		// 斬り攻撃1の終了フレーム
#define ATTACK1B_START_FRAME 25.0f	// 斬り攻撃1Bの開始フレーム
#define ATTACK1B_END_FRAME 70.0f	// 斬り攻撃1Bの終了フレーム
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
#define DASH_SWORD_OFFSET_ROT CVector(0.0f, 0.0f, 0.0f)

#define KICK_START_FRAME 26.0f		// 蹴り攻撃の開始フレーム
#define KICK_END_FRAME 40.0f		// 蹴り攻撃の終了フレーム
#define KICK_COL_RADIUS 7.5f		// 蹴り攻撃のコライダーの半径
// 蹴り攻撃のコライダーのオフセット座標
#define KICK_COL_OFFSET_POS CVector(0.0f, 4.0f, 2.5f)

// 先行入力のコライダーの半径
#define TA_COL_RADIUS 25.0f
// 先行入力のコライダーのオフセット座標
#define TA_COL_OFFSET_POS CVector(0.0f, 4.0f, 2.75f)

// 敵のアニメーションデータのテーブル
const std::vector<CEnemy::AnimData> ANIM_DATA =
{
	{ "",						true,	0.0f,	1.0f	},	// Tポーズ
	{ ANIM_PATH"idle.x",		true,	121.0f,	1.0f	},	// 待機
	{ ANIM_PATH"idle2.x",		true,	121.0f,	1.0f	},	// 待機(戦闘中)
	{ ANIM_PATH"walk.x",		true,	82.0f,	1.0f	},	// 歩行
	{ ANIM_PATH"run.x",			true,	36.0f,	0.7f	},	// ダッシュ
	{ ANIM_PATH"GSSlash1.x",	false,	77.0f,	1.0f	},	// 斬り攻撃
	{ ANIM_PATH"GSSlashR.x",	false,	90.0f,	1.0f	},	// 斬り攻撃B
	{ ANIM_PATH"GSSlash2.x",	false,	110.0f,	1.25f	},	// 斬り攻撃
	{ ANIM_PATH"GSSlash.x",		false,	212.0f,	1.25f	},	// 斬りかかり攻撃
	{ ANIM_PATH"kick.x",		false,	74.0f,	1.75f	},	// 蹴り攻撃
	{ ANIM_PATH"jump_start.x",	false,	25.0f,	1.0f	},	// ジャンプ開始
	{ ANIM_PATH"jump.x",		true,	1.0f,	1.0f	},	// ジャンプ中
	{ ANIM_PATH"jump_end.x",	false,	26.0f,	1.0f	},	// ジャンプ終了
	{ ANIM_PATH"avoidR.x",		true,	58.0f,	1.5f	},	// 回避:右
	{ ANIM_PATH"avoidL.x",		true,	58.0f,	1.5f	},	// 回避:左
	{ ANIM_PATH"avoidB.x",		true,	98.0f,	1.5f	},	// 回避:後ろ
	{ ANIM_PATH"hit.x",			false,	44.0f,	0.90f	},	// 仰け反り
	{ ANIM_PATH"death.x",		false,	182.0f,	1.0f	},	// 死亡
	{ ANIM_PATH"victory.x",		true,	271.0f,	1.0f	},	// 勝利
};

// コンストラクタ
CHeavyWarrior::CHeavyWarrior(CPlayer* player, int enemyLevel)
	: mpRideObject(nullptr)
	, mIsPlayedSlashSE(false)
	, mIsSpawnedSlashEffect(false)
	, mpSword(nullptr)
	, mIsBattle(true)
	, mBattleIdletime(0.0f)
	, mpBattleTarget(nullptr)
	, mCan1B(false)
	, mTactics((int)ETactics::Aggressive)
	, mAttPattern((int)EAttPattern::None)
	, mIsSA(false)
{
	mpBattleTarget = player;

	// ゲージのオフセット位置を設定
	mHpGaugeOffsetPos = CVector(0.0f, GAUGE_OFFSET_Y, 0.0f);
	mStGaugeOffsetPos = CVector(0.0f, GAUGE_OFFSET_Y - 1.0f, 0.0f);
	// バフ・デバフゲージのオフセット位置を設定
	mGBBuffOffsetPos = CVector(0.0f, BUFF_GAUGE_OFFSET_Y, 0.0f);

	// 敵を初期化
	InitEnemy("Soldier", &ANIM_DATA);

	// 最初は待機アニメーションを再生
	ChangeAnimation((int)EAnimType::eIdle);

	// 本体のコライダーを作成
	mpBodyCol = new CColliderCapsule
	(
		this, ELayer::eEnemy,
		CVector(0.0f, BODY_RADIUS - 1.0f, 0.0f),
		CVector(0.0f, BODY_HEIGHT - BODY_RADIUS, 0.0f),
		BODY_RADIUS
	);
	mpBodyCol->SetCollisionTags({ ETag::eField, ETag::eRideableObject, ETag::ePlayer, ETag::eEnemy });
	SetInvincible(false);

	mpSlashSE = CResourceManager::Get<CSound>("SlashSound");

	// 敵の剣を作成
	mpSword = new CBusterSword
	(
		this,
		ETag::eEnemy,
		{ ETag::ePlayer },	// プレイヤーのタグが設定されたコライダーと衝突
		{ ELayer::ePlayer }	// プレイヤーのレイヤーが設定されたコライダーと衝突
	);

	// mpSword->Scale(1.2f, 1.0f, 1.0f);

	// 大きくして威圧感を出す
	Scale(1.5f, 1.5f, 1.5f);
	mpSword->Scale(1.0f, 0.9f, 0.9f);

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

	mLevel = enemyLevel;
	InitStatus();
	mGuardBreakTime = 12.5f;

	// ロックオンしたときの注視点の高さ
	mLockOnOffset = CVector(0.0f, 15.0f, 0.0f);
}

// デストラクタ
CHeavyWarrior::~CHeavyWarrior()
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

void CHeavyWarrior::InitStatus()
{
	// 外部CSVファイルからステータスを読み込む
	const EnemyStatusData* data =
		CEnemyStatusLoader::GetStatus("HeavyWarrior", mLevel);
	if (data != nullptr)
	{
		mMaxHp       = data->maxHp;
		mMaxSt       = data->maxSt;
		mGainSt      = data->gainSt;
		mAttackCost1 = data->attackCost1;
		mAttackCost2 = data->attackCost2;
		mAttackCost3 = data->attackCost3;
		mAvoidCost   = data->avoidCost;
		mStepMag     = data->stepMag;
		mAttackMag   = data->attackMag;
		mAtSpeedMag  = data->atSpeedMag;
		mNegTime     = data->negTime;
		mNegProb     = data->negProb;
		mCan1B       = data->can1B;
	}
	else
	{
		// CSVにデータがない場合のデフォルト値
		mMaxHp = 120.0f;
		mMaxSt = 150.0f;
		mGainSt = 10.0f;
		mAttackCost1 = 27.5f;
		mAttackCost2 = 32.5f;
		mAttackCost3 = 37.5f;
		mAvoidCost = 20.0f;
		mStepMag = 1.0f;
		mAttackMag = 1.0f;
		mAtSpeedMag = 1.0f;
		mNegTime = 0.8f;
		mNegProb = 66.6f;
	}

	mHp = mMaxHp;
	mSt = mMaxSt;
}

// 攻撃中か
bool CHeavyWarrior::IsAttacking() const
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
void CHeavyWarrior::AttackStart()
{
	// 先行入力コライダーをオフにする
	mpTACol->SetEnable(false);

	// ベースクラスの攻撃開始処理を呼び出し
	CEnemy::AttackStart();

	// 斬り攻撃中であれば、剣のコライダーをオンにする
	if (mState == (int)EState::eAttack1
		|| mState == (int)EState::eAttack1B
		|| mState == (int)EState::eAttack2
		|| mState == (int)EState::eAttackX)
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
void CHeavyWarrior::AttackEnd()
{
	// ベースクラスの攻撃終了処理を呼び出し
	CEnemy::AttackEnd();

	// 攻撃コライダーをオフ
	mpSword->SetEnableCol(false);
	mpKickCol->SetEnable(false);
}

// ダメージを受ける
void CHeavyWarrior::TakeDamage(float damage, CObjectBase* causer)
{
	float amount = (!mInGuardBreak) ? damage : damage * 2;

	// ベースクラスのダメージ処理を呼び出す
	CEnemy::TakeDamage(amount, causer);

	// 死亡していなければ、
	if (!IsDeath())
	{
		// 攻撃を加えた相手を戦闘相手に設定
		mpBattleTarget = causer;

		// スーパーアーマー状態でなければのけぞる
		if (!mIsSA)
		{
			// 仰け反り状態へ移行
			ChangeState((int)EState::eHit);

			// 攻撃を加えた相手の方向へ向く
			LookAtBattleTarget(true);
		}

		// 戦闘状態へ切り替え
		mIsBattle = true;

		// 移動を停止
		mMoveSpeed = CVector::zero;
	}
}

void CHeavyWarrior::TakeKick(CObjectBase* causer)
{
	// 死亡していなければ、
	if (!IsDeath())
	{
		// 攻撃を加えた相手を戦闘相手に設定
		mpBattleTarget = causer;

		// スーパーアーマー状態でなければのけぞる
		if (!mIsSA)
		{
			// 仰け反り状態へ移行
			ChangeState((int)EState::eHit);

			// 攻撃を加えた相手の方向へ向く
			LookAtBattleTarget(true);
		}

		// 戦闘状態へ切り替え
		mIsBattle = true;

		// 移動を停止
		mMoveSpeed = CVector::zero;
	}
}

// 死亡
void CHeavyWarrior::Death()
{
	// 死亡状態に切り替え
	ChangeState((int)EState::eDeath);
}

// 衝突処理
void CHeavyWarrior::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// ベースの衝突処理を呼び出す
	CEnemy::Collision(self, other, hit);

	// 先行入力用コライダーがヒットした
	if (other->Layer() == ELayer::eTypeAheadCol)
	{
		if (mState == (int)EState::eIdle || mState == (int)EState::eChase)
		{
			if (mSt >= mAvoidCost)
			{
				CCharaBase::UseStamina(mAvoidCost);
				SelectAvoid();
			}
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
			case (int)EState::eAttack1:		hitChara->TakeDamage(12 * mAttackMag, this);	break;
				// 斬り攻撃1B
			case (int)EState::eAttack1B:	hitChara->TakeDamage(10 * mAttackMag, this);	break;
				// 斬り攻撃2
			case (int)EState::eAttack2:		hitChara->TakeDamage(18 * mAttackMag, this);	break;
				// 斬り攻撃X
			case (int)EState::eAttackX:		hitChara->TakeDamage(15 * mAttackMag, this);	break;
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

void CHeavyWarrior::ChangeAnimation(int type, bool restart)
{
	if (mpAnimData == nullptr) return;
	if (!(0 <= type && type < mpAnimData->size())) return;
	AnimData data = (*mpAnimData)[type];
	CXCharacter::ChangeAnimation
	(
		type,
		data.loop,
		data.frameLength,
		restart
	);
	// 攻撃スピードを少しランダムにする
	float rand = Math::Rand(0.85f, 1.15f);
	switch (type)
	{
	case 5:
	case 6:
	case 7:
	case 8:
		CXCharacter::SetAnimationSpeed((data.speed * rand) * mAtSpeedMag);
		break;
	default:
		CXCharacter::SetAnimationSpeed(data.speed);
		break;
	}

	mAnimType = type;
}

void CHeavyWarrior::UpdateTactics()
{
	float hpRate = mHp / mMaxHp;

	if (hpRate >= 0.7f)
	{
		// HP 70%以上：攻撃的
		mTactics = (int)ETactics::Aggressive;
	}
	else if (hpRate >= 0.3f)
	{
		// HP 30%〜70%：バランス
		mTactics = (int)ETactics::Balanced;
	}
	else
	{
		// HP 30%未満：慎重
		mTactics = (int)ETactics::Cautious;
	}
}

void CHeavyWarrior::UpdateBattleTempo()
{
	float stRate = mSt / mMaxSt;

	// 戦術レイヤーに応じてスタミナ閾値を調整
	float highThreshold = 0.7f;		// これ以上ならHighSt
	float lowThreshold = 0.3f;		// これ以下ならLowSt

	if (mTactics == (int)ETactics::Aggressive)
	{
		// 攻撃的：スタミナが少なくてもHighStになりやすい
		highThreshold = 0.5f;
		lowThreshold = 0.2f;
	}
	else if (mTactics == (int)ETactics::Cautious)
	{
		// 慎重：スタミナが多くてもLowStになりやすい
		highThreshold = 0.8f;
		lowThreshold = 0.4f;
	}

	if (stRate >= highThreshold)
	{
		mBattleTempo = (int)EBattleTempo::HighSt;
	}
	else if (stRate >= lowThreshold)
	{
		mBattleTempo = (int)EBattleTempo::MidSt;
	}
	else
	{
		mBattleTempo = (int)EBattleTempo::LowSt;
	}
}

// 戦術×テンポの3×3マトリクスから次の行動ステートを決定
// 距離も考慮して最終的な行動を返す
void CHeavyWarrior::DecideNextAction()
{
	float dist = GetDistToTarget();
	int rand = Math::Rand(1, 100);

	// 戦術レイヤー
	switch (static_cast<ETactics>(mTactics))
	{
		// ===== 攻撃的 =====
	case ETactics::Aggressive:
		// テンポレイヤー
		switch (static_cast<EBattleTempo>(mBattleTempo))
		{
			// 攻撃的 × 高スタミナ：とにかく攻める
		case EBattleTempo::HighSt:
			// 攻撃が当たる距離にいるか
			if (dist <= ATTACK_RANGE + (mStepMag * 0.5f))
			{
				// ToDo : ここで攻撃のパターンを選択した後、
				// EAttPatternを変更してAttPattGearBoxへ
				if (rand <= 10)
				{
					mAttPattern = (int)EAttPattern::PatternA;
				}
				else if (rand <= 30)
				{
					mAttPattern = (int)EAttPattern::PatternB;
				}
				else
				{
					mAttPattern = (int)EAttPattern::PatternC;
				}
				AttPattGearBox();
				break;
			}
			// 遠ければ接近の状態へ移行
			ChangeState((int)EState::eChase);
			break;

			// 攻撃的 × 中スタミナ：攻め気は保つが少し抑えめ
		case EBattleTempo::MidSt:
			// 攻撃が当たる距離にいるか
			if (dist <= ATTACK_RANGE + (mStepMag * 0.5f))
			{
				// ToDo : ここで攻撃のパターンを選択した後、
				// EAttPatternを変更してAttPattGearBoxへ
				if (rand <= 15)
				{
					mAttPattern = (int)EAttPattern::PatternA;
				}
				else if (rand <= 30)
				{
					mAttPattern = (int)EAttPattern::PatternB;
				}
				else
				{
					mAttPattern = (int)EAttPattern::PatternC;
				}
				AttPattGearBox();
			}
			// 遠ければ接近の状態へ移行
			ChangeState((int)EState::eChase);
			break;

			// 攻撃的 × 低スタミナ：攻撃は積極的に行うが、追わない
		case EBattleTempo::LowSt:
			// 攻撃が当たる距離にいるか
			if (dist <= ATTACK_RANGE + (mStepMag * 0.5f))
			{
				// ToDo : ここで攻撃のパターンを選択した後、
				// EAttPatternを変更してAttPattGearBoxへ
				if (rand <= 15)
				{
					mAttPattern = (int)EAttPattern::PatternA;
				}
				else if (rand <= 35)
				{
					mAttPattern = (int)EAttPattern::PatternB;
				}
				else
				{
					mAttPattern = (int)EAttPattern::PatternC;
				}
				AttPattGearBox();
			}
			// 遠くても追わない
			ChangeState((int)EState::eIdle);
			break;
		}
		break;

		// ===== バランス =====
	case ETactics::Balanced:
		switch (static_cast<EBattleTempo>(mBattleTempo))
		{
			// バランス × 高スタミナ：少しスタミナを残しつつ連撃
		case EBattleTempo::HighSt:
			// 攻撃が当たる距離にいるか
			if (dist <= ATTACK_RANGE + (mStepMag * 0.5f))
			{
				// ToDo : ここで攻撃のパターンを選択した後、
				// EAttPatternを変更してAttPattGearBoxへ
				if (rand <= 10)
				{
					mAttPattern = (int)EAttPattern::PatternA;
				}
				else if (rand <= 45)
				{
					mAttPattern = (int)EAttPattern::PatternB;
				}
				else
				{
					mAttPattern = (int)EAttPattern::PatternC;
				}
				AttPattGearBox();
				break;
			}
			// 遠ければ接近の状態へ移行
			ChangeState((int)EState::eChase);
			break;

			// バランス × 中スタミナ：確実な戦い方
		case EBattleTempo::MidSt:
			// 攻撃が当たる距離にいるか
			if (dist <= ATTACK_RANGE + (mStepMag * 0.5f))
			{
				// ToDo : ここで攻撃のパターンを選択した後、
				// EAttPatternを変更してAttPattGearBoxへ
				if (rand <= 15)
				{
					mAttPattern = (int)EAttPattern::PatternA;
				}
				else if (rand <= 50)
				{
					mAttPattern = (int)EAttPattern::PatternB;
				}
				else
				{
					mAttPattern = (int)EAttPattern::PatternC;
				}
				AttPattGearBox();
				break;
			}
			// 遠ければ接近の状態へ移行
			ChangeState((int)EState::eChase);
			break;

			// バランス × 低スタミナ：下がってスタミナ回復を優先
		case EBattleTempo::LowSt:
			// 攻撃が当たる距離にいるか
			if (dist <= ATTACK_RANGE + (mStepMag * 0.5f))
			{
				// ToDo : ここで攻撃のパターンを選択した後、
				// EAttPatternを変更してAttPattGearBoxへ
				if (rand <= 20)
				{
					mAttPattern = (int)EAttPattern::PatternA;
				}
				else if (rand <= 55)
				{
					mAttPattern = (int)EAttPattern::PatternB;
				}
				else
				{
					mAttPattern = (int)EAttPattern::PatternC;
				}
				AttPattGearBox();
				break;
			}
			// 遠くても追わない
			ChangeState((int)EState::eIdle);
			break;
		}
		break;

		// ===== 慎重 (HP 30%未満) =====
	case ETactics::Cautious:
		switch (static_cast<EBattleTempo>(mBattleTempo))
		{
			// 慎重 × 高スタミナ：スタミナがあっても追わずに戦う
		case EBattleTempo::HighSt:
			// 攻撃が当たる距離にいるか
			if (dist <= ATTACK_RANGE + (mStepMag * 0.5f))
			{
				// ToDo : ここで攻撃のパターンを選択した後、
				// EAttPatternを変更してAttPattGearBoxへ
				if (rand <= 10)
				{
					mAttPattern = (int)EAttPattern::PatternA;
				}
				else if (rand <= 35)
				{
					mAttPattern = (int)EAttPattern::PatternB;
				}
				else
				{
					mAttPattern = (int)EAttPattern::PatternC;
				}
				AttPattGearBox();
				break;
			}
			// 遠くても追わない
			ChangeState((int)EState::eIdle);
			break;

			// 慎重 × 中スタミナ：距離を取りたがる
		case EBattleTempo::MidSt:
			// 攻撃が当たる距離にいるか
			if (dist <= ATTACK_RANGE + (mStepMag * 0.5f))
			{
				// ToDo : ここで攻撃のパターンを選択した後、
				// EAttPatternを変更してAttPattGearBoxへ
				if (rand <= 20)
				{
					mAttPattern = (int)EAttPattern::PatternA;
				}
				else if (rand <= 55)
				{
					mAttPattern = (int)EAttPattern::PatternB;
				}
				else
				{
					mAttPattern = (int)EAttPattern::PatternC;
				}
				AttPattGearBox();
				break;
			}
			// 遠くても追わない
			ChangeState((int)EState::eIdle);
			break;

			// 慎重 × 低スタミナ：瀕死かつ息切れ。必死にスタミナ回復
		case EBattleTempo::LowSt:
			// 攻撃が当たる距離にいるか
			if (dist <= ATTACK_RANGE + (mStepMag * 0.5f))
			{
				// ToDo : ここで攻撃のパターンを選択した後、
				// EAttPatternを変更してAttPattGearBoxへ
				if (rand <= 20)
				{
					mAttPattern = (int)EAttPattern::PatternA;
				}
				else if (rand <= 60)
				{
					mAttPattern = (int)EAttPattern::PatternB;
				}
				else
				{
					mAttPattern = (int)EAttPattern::PatternC;
				}
				AttPattGearBox();
				break;
			}
			// 遠くても追わない
			ChangeState((int)EState::eIdle);
			break;
		}
		break;
	}
}

// 戦闘相手の方へ向く
void CHeavyWarrior::LookAtBattleTarget(bool immediate)
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

float CHeavyWarrior::GetDistToTarget()
{
	// 現在地と目的地を取得
	CVector pos = Position();
	CVector targetPos = mpBattleTarget->Position();
	targetPos.Y(pos.Y());
	// 現在地から目的地までのベクトルを求める
	CVector vec = targetPos - pos;
	// 距離に換算
	float dist = vec.Length();
	return dist;
}

void CHeavyWarrior::STRegene()
{
	if (mHp > 0.0f)
	{
		CCharaBase::GainStamina(mGainSt * Times::DeltaTime());
	}
}

void CHeavyWarrior::SetInvincible(bool invincible)
{
	if (!invincible)
	{
		// 当たり判定を通常のレイヤー設定にする
		mpBodyCol->SetCollisionLayers({ ELayer::eField, ELayer::eWall, ELayer::ePlayer, ELayer::eEnemy, ELayer::eAttackCol, ELayer::eTypeAheadCol });
	}
	else
	{
		// 当たり判定を無敵のレイヤー設定にする
		mpBodyCol->SetCollisionLayers({ ELayer::eField, ELayer::eWall, ELayer::ePlayer, ELayer::eEnemy });
	}
}

// 状態切り替え
void CHeavyWarrior::ChangeState(int state)
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

void CHeavyWarrior::UpdateReserve()
{
}

// 待機状態の更新処理
void CHeavyWarrior::UpdateIdle()
{
	STRegene();
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
			// 待機時間なし
			mStateStep++;
			break;
			// ステップ1：マトリクスから次の行動を決定
		case 1:

			// 戦術レイヤーとテンポレイヤーを更新
			UpdateTactics();
			UpdateBattleTempo();

			// 3×3マトリクスで次の行動を決定
			DecideNextAction();
			break;
		}
	}
}

// 追いかける時の更新処理
void CHeavyWarrior::UpdateChase()
{
	mMoveSpeed = CVector::zero;
	STRegene();

	// 現在地と目的地を取得
	CVector pos = Position();
	CVector targetPos = mpBattleTarget->Position();
	targetPos.Y(pos.Y());
	// 現在地から目的地までのベクトルを求める
	CVector vec = targetPos - pos;
	// 攻撃範囲内であれば
	float dist = vec.Length();
	if (dist <= ATTACK_RANGE + (mStepMag * 0.5f))
	{
		// 待機状態へ移行
		ChangeState((int)EState::eIdle);
	}
	// 攻撃範囲外
	else if (dist >= DASH_DIST && mBattleTempo == (int)EBattleTempo::HighSt && mTactics != (int)ETactics::Cautious)
	{
		mpSword->Rotation(DASH_SWORD_OFFSET_ROT);
		// 走行アニメーションを再生
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
			mpSword->Rotation(SWORD_OFFSET_ROT);
			mMoveSpeed = dir * dist;
			ChangeState((int)EState::eIdle);
		}
	}
	else
	{
		mpSword->Rotation(SWORD_OFFSET_ROT);
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

// 斬り攻撃1の更新処理
void CHeavyWarrior::UpdateAttack1()
{
	// ステップごとに処理を分ける
	switch (mStateStep)
	{
	// ステップ0：攻撃アニメーション再生
	case 0:
		ChangeAnimation((int)EAnimType::eAttack1, true);
		mAttackVec = VectorZ();
		CCharaBase::UseStamina(mAttackCost1);
		// 斬り攻撃1にはスーパーアーマーを付与
		mIsSA = true;
		mStateStep++;
		break;
	case 1:
		// 先行入力フレーム開始フレームまで経過したか
		if (GetAnimationFrame() >= ATTACK1_START_FRAME - AT_GRACE_FRAME)
		{
			// 先行入力コライダーをオンにする
			mpTACol->SetEnable(true);
			mStateStep++;
		}
		break;
	// ステップ2：攻撃開始
	case 2:
		// 攻撃を開始するまで、徐々に戦闘相手の方向へ向く
		LookAtBattleTarget();

		// 攻撃開始フレームまで経過したか
		if (GetAnimationFrame() >= ATTACK1_START_FRAME)
		{
			// 斬撃SEを再生
			mpSlashSE->Play();
			// 攻撃開始処理を呼び出す
			AttackStart();

			mInAttack = true;

			mStateStep++;
		}
		break;
	// ステップ3：攻撃終了
	case 3:
		// 攻撃終了フレームまで経過したか
		if (GetAnimationFrame() >= ATTACK1_END_FRAME)
		{
			// 攻撃終了処理を呼び出す
			AttackEnd();
			mInAttack = false;
			mStateStep++;
		}

		if (mInAttack)
		{
			// 1秒あたりの移動速度
			CVector move = mAttackVec * (15.0f * mStepMag * mAtSpeedMag) * Times::DeltaTime();
			Position(Position() + move);
		}
		break;
	// ステップ4：攻撃アニメーション終了待ち
	case 4:
		// アニメーション終了したら、待機状態へ戻す
		if (IsAnimationFinished())
		{
			// スーパーアーマーを解除
			mIsSA = false;

			// 待機状態へ移行
			ChangeState((int)EState::eIdle);
			ChangeAnimation((int)EAnimType::eIdle);
			mAttStep++;
			AttPattGearBox();
		}
		break;
	}
}

// 斬り攻撃1Bの更新処理
// 斬り攻撃1Bは通常の斬り攻撃1でがら空きになる右サイドを
// 刈り取る右薙ぎ払い攻撃
void CHeavyWarrior::UpdateAttack1B()
{
	// ステップごとに処理を分ける
	switch (mStateStep)
	{
	// ステップ0：攻撃アニメーション再生
	case 0:
		mpSword->Rotation(DASH_SWORD_OFFSET_ROT);
		ChangeAnimation((int)EAnimType::eAttack1B, true);
		mAttackVec = VectorZ();
		CCharaBase::UseStamina(mAttackCost1);
		mStateStep++;
		break;
	case 1:
		// 先行入力フレーム開始フレームまで経過したか
		if (GetAnimationFrame() >= ATTACK1_START_FRAME - AT_GRACE_FRAME)
		{
			// 先行入力コライダーをオンにする
			mpTACol->SetEnable(true);
			mStateStep++;
		}
		break;
	// ステップ2：攻撃開始
	case 2:
		// 攻撃を開始するまで、徐々に戦闘相手の方向へ向く
		LookAtBattleTarget();

		// 攻撃開始フレームまで経過したか
		if (GetAnimationFrame() >= ATTACK1B_START_FRAME)
		{
			// 斬撃SEを再生
			mpSlashSE->Play();
			// 攻撃開始処理を呼び出す
			AttackStart();

			mInAttack = true;

			mStateStep++;
		}
		break;
	// ステップ3：攻撃終了
	case 3:
		// 攻撃終了フレームまで経過したか
		if (GetAnimationFrame() >= ATTACK1B_END_FRAME)
		{
			// 攻撃終了処理を呼び出す
			AttackEnd();
			mInAttack = false;
			mStateStep++;
		}

		if (mInAttack)
		{
			// 1秒あたりの移動速度
			CVector move = mAttackVec * (27.5f * mStepMag * mAtSpeedMag) * Times::DeltaTime();
			Position(Position() + move);
		}
		break;
	// ステップ4：攻撃アニメーション終了待ち
	case 4:
		// アニメーション終了したら、待機状態へ戻す
		if (IsAnimationFinished())
		{
			mpSword->Rotation(SWORD_OFFSET_ROT);
			// 待機状態へ移行
			ChangeState((int)EState::eIdle);
			ChangeAnimation((int)EAnimType::eIdle);
			mAttStep++;
			AttPattGearBox();
		}
		break;
	}
}

// 斬り攻撃2の更新処理
void CHeavyWarrior::UpdateAttack2()
{
	// ステップごとに処理を分ける
	switch (mStateStep)
	{
		// ステップ0：攻撃アニメーション再生
	case 0:
		ChangeAnimation((int)EAnimType::eAttack2, true);
		mAttackVec = VectorZ();
		CCharaBase::UseStamina(mAttackCost2);
		mStateStep++;
		break;
	case 1:
		// 先行入力フレーム開始フレームまで経過したか
		if (GetAnimationFrame() >= ATTACK1_START_FRAME - AT_GRACE_FRAME)
		{
			// 先行入力コライダーをオンにする
			mpTACol->SetEnable(true);
			mStateStep++;
		}
		break;
	// ステップ2：攻撃開始
	case 2:
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

	// ステップ3：攻撃終了
	case 3:
		// 攻撃終了フレームまで経過したか
		if (GetAnimationFrame() >= ATTACK2_END_FRAME)
		{
			// 攻撃終了処理を呼び出す
			AttackEnd();
			mInAttack = false;
			mStateStep++;
		}

		if (mInAttack)
		{
			// 1秒あたりの移動速度
			CVector move = mAttackVec * (10.0f * mStepMag * mAtSpeedMag) * Times::DeltaTime();
			Position(Position() + move);
		}
		break;

	// ステップ4：攻撃アニメーション終了待ち
	case 4:
		// アニメーション終了したら、待機状態へ戻す
		if (IsAnimationFinished())
		{
			// 待機状態へ移行
			ChangeState((int)EState::eIdle);
			ChangeAnimation((int)EAnimType::eIdle);
			mAttStep++;
			AttPattGearBox();
		}
		break;
	}
}

void CHeavyWarrior::UpdateAttackX()
{
	switch (mStateStep)
	{
	case 0:
		mpSword->Rotation(ATTACKX_SWORD_OFFSET_ROT);
		// 攻撃アニメーションを開始
		ChangeAnimation((int)EAnimType::eAttackX, true);
		// 斬撃SEの再生済みフラグを初期化
		mIsPlayedSlashSE = false;
		// 斬撃エフェクトの生成済みフラグを初期化
		mIsSpawnedSlashEffect = false;

		mAttackVec = VectorZ();
		CCharaBase::UseStamina(mAttackCost3);
		mStateStep++;
		break;
	case 1:
		// 先行入力フレーム開始フレームまで経過したか
		if (GetAnimationFrame() >= ATTACK1_START_FRAME - AT_GRACE_FRAME)
		{
			// 先行入力コライダーをオンにする
			mpTACol->SetEnable(true);
			mStateStep++;
		}
		break;
	case 2:
		// 攻撃を開始するまで、徐々に戦闘相手の方向へ向く
		LookAtBattleTarget();

		if (GetAnimationFrame() >= ATTACKX_START_FRAME)
		{
			// 斬撃SEを再生
			mpSlashSE->Play();
			// 攻撃開始
			AttackStart();

			mStateStep++;
		}
		break;

	case 3:
		if (GetAnimationFrame() >= ATTACKX_START_FRAME + 85.0f)
		{
			CObjectBase::AttackStart();
			mInAttack = true;
			mStateStep++;
		}
		break;

	case 4:
		if (GetAnimationFrame() >= ATTACKX_END_FRAME)
		{
			// 攻撃終了
			AttackEnd();

			mInAttack = false;

			mStateStep++;
		}

		if (mInAttack)
		{
			// 1秒あたりの移動速度
			CVector move = mAttackVec * (30.0f * mStepMag * mAtSpeedMag) * Times::DeltaTime();
			Position(Position() + move);
		}

		break;
	case 5:
		// 攻撃アニメーションが終了したら、
		if (IsAnimationFinished())
		{
			mpSword->Rotation(SWORD_OFFSET_ROT);

			// 待機状態へ移行
			ChangeState((int)EState::eIdle);
			ChangeAnimation((int)EAnimType::eIdle);
			mAttStep++;
			AttPattGearBox();
		}
		break;
	}
}

void CHeavyWarrior::SelectAvoid()
{
	if (!mpBattleTarget) return;

	// 自分の前方向
	CVector myForward = VectorZ();
	myForward.Y(0.0f);
	myForward.Normalize();

	// 右方向
	CVector right = CVector::Cross(myForward, CVector::up);
	right.Y(0.0f);
	right.Normalize();

	// 左方向
	CVector left = -right;

	// 後方向
	CVector back = -myForward;

	// 回避距離
	const float avoidDist = 90.0f;

	// レイ開始位置（少し浮かせる）
	CVector start = Position();
	start.Y(start.Y() + 10.0f);

	// 各方向の終了位置
	CVector rightEnd = start + right * avoidDist;
	CVector leftEnd = start + left * avoidDist;
	CVector backEnd = start + back * avoidDist;

	// 壁チェック
	bool hitRight = false;
	bool hitLeft = false;
	bool hitBack = false;

	float rightDist = avoidDist;
	float leftDist = avoidDist;
	float backDist = avoidDist;

	CHitInfo hit;

	//-----------------------------------
	// 右チェック
	//-----------------------------------
	for (auto col : CCollisionManager::Instance()->GetColList())
	{
		if (col->Layer() != ELayer::eWall) continue;

		if (CCollider::CollisionRay(col, start, rightEnd, &hit))
		{
			hitRight = true;
			rightDist = hit.dist;
			break;
		}
	}

	//-----------------------------------
	// 左チェック
	//-----------------------------------
	for (auto col : CCollisionManager::Instance()->GetColList())
	{
		if (col->Layer() != ELayer::eWall) continue;

		if (CCollider::CollisionRay(col, start, leftEnd, &hit))
		{
			hitLeft = true;
			leftDist = hit.dist;
			break;
		}
	}

	//-----------------------------------
	// 後チェック
	//-----------------------------------
	for (auto col : CCollisionManager::Instance()->GetColList())
	{
		if (col->Layer() != ELayer::eWall) continue;

		if (CCollider::CollisionRay(col, start, backEnd, &hit))
		{
			hitBack = true;
			backDist = hit.dist;
			break;
		}
	}

	//-----------------------------------
	// 一番安全な方向を選択
	//-----------------------------------
	float bestDist = -1.0f;

	enum class EAvoidType
	{
		Right,
		Left,
		Back
	};

	EAvoidType avoid = EAvoidType::Back;

	if (!hitRight && avoidDist > bestDist)
	{
		bestDist = avoidDist;
		avoid = EAvoidType::Right;
	}
	else if (rightDist > bestDist)
	{
		bestDist = rightDist;
		avoid = EAvoidType::Right;
	}

	if (!hitLeft && avoidDist > bestDist)
	{
		bestDist = avoidDist;
		avoid = EAvoidType::Left;
	}
	else if (leftDist > bestDist)
	{
		bestDist = leftDist;
		avoid = EAvoidType::Left;
	}

	if (!hitBack && avoidDist > bestDist)
	{
		bestDist = avoidDist;
		avoid = EAvoidType::Back;
	}
	else if (backDist > bestDist)
	{
		bestDist = backDist;
		avoid = EAvoidType::Back;
	}

	//-----------------------------------
	// 回避実行
	//-----------------------------------
	switch (avoid)
	{
	case EAvoidType::Right:
		mAvoidVec = right;
		ChangeState((int)EState::eAvoidR);
		break;

	case EAvoidType::Left:
		mAvoidVec = left;
		ChangeState((int)EState::eAvoidL);
		break;

	case EAvoidType::Back:
		mAvoidVec = back;
		ChangeState((int)EState::eAvoidB);
		break;
	}

	mAvoidVec.Normalize();
	mIsGravity = false;
}

void CHeavyWarrior::UpdateAvoidR()
{
	switch (mStateStep)
	{
	case 0:
		// 回避アニメーションを開始
		ChangeAnimation((int)EAnimType::eAvoidR, true);
		mStateStep++;
		break;
	case 1:
		if (GetAnimationFrame() >= 20.0f && !mAvoidMoving)
		{
			mAvoidMoving = true;
			SetInvincible(true);
			mStateStep++;
		}
		break;
	case 2:
		if (mAvoidMoving)
		{
			// 1秒あたりの移動速度
			CVector move = mAvoidVec * 150.0f * Times::DeltaTime();
			Position(Position() + move);

			if (GetAnimationFrame() >= 50.0f)
			{
				mAvoidMoving = false;
				mStateStep++;
			}
		}
		break;
	case 3:
		// 回避アニメーションが終了したら
		if (IsAnimationFinished())
		{
			SetInvincible(false);
			mIsGravity = true;
			// 待機状態へ移行
			ChangeState((int)EState::eIdle);
			ChangeAnimation((int)EAnimType::eIdle);
		}
		break;
	}
}

void CHeavyWarrior::UpdateAvoidL()
{
	switch (mStateStep)
	{
	case 0:
		// 回避アニメーションを開始
		ChangeAnimation((int)EAnimType::eAvoidL, true);
		mStateStep++;
		break;
	case 1:
		if (GetAnimationFrame() >= 20.0f && !mAvoidMoving)
		{
			mAvoidMoving = true;
			SetInvincible(true);
			mStateStep++;
		}
		break;
	case 2:
		if (mAvoidMoving)
		{
			// 1秒あたりの移動速度
			CVector move = mAvoidVec * 150.0f * Times::DeltaTime();
			Position(Position() + move);

			if (GetAnimationFrame() >= 50.0f)
			{
				mAvoidMoving = false;
				mStateStep++;
			}
		}
		break;
	case 3:
		// 回避アニメーションが終了したら
		if (IsAnimationFinished())
		{
			SetInvincible(false);
			mIsGravity = true;
			// 待機状態へ移行
			ChangeState((int)EState::eIdle);
			ChangeAnimation((int)EAnimType::eIdle);
		}
		break;
	}
}

void CHeavyWarrior::UpdateAvoidB()
{
	switch (mStateStep)
	{
	case 0:
		// 回避アニメーションを開始
		ChangeAnimation((int)EAnimType::eAvoidB, true);
		mStateStep++;
		break;
	case 1:
		if (GetAnimationFrame() >= 12.0f && !mAvoidMoving)
		{
			mAvoidMoving = true;
			SetInvincible(true);
			mStateStep++;
		}
		break;
	case 2:
		if (mAvoidMoving)
		{
			// 1秒あたりの移動速度
			CVector move = -VectorZ() * 125.0f * Times::DeltaTime();
			Position(Position() + move);

			if (GetAnimationFrame() >= 52.0f)
			{
				mAvoidMoving = false;
				mStateStep++;
			}
		}
		break;
	case 3:
		// 回避アニメーションが終了したら
		if (IsAnimationFinished())
		{
			SetInvincible(false);
			mIsGravity = true;
			// 待機状態へ移行
			ChangeState((int)EState::eIdle);
			ChangeAnimation((int)EAnimType::eIdle);
		}
		break;
	}
}

void CHeavyWarrior::UpdateNeg()
{
	STRegene();
	// 戦闘時の待機
	ChangeAnimation((int)EAnimType::eIdleBattle);
	// ステップごとに処理を切り替え
	// 何か処理をはさむかもなのでswitch制御にしています
	switch (mStateStep)
	{
		// ステップ0：待機時間
	case 0:
		// n秒間隙ができる
		if (mElapsedTime < mNegTime)
		{
			mElapsedTime += Times::DeltaTime();
		}
		// 待ち時間が終了したら、終了
		else
		{
			mStateStep++;
		}
		break;
	case 1:
		// 待機状態へ移行
		ChangeState((int)EState::eIdle);
		ChangeAnimation((int)EAnimType::eIdle);
		break;
	}
}

// 仰け反り状態の更新処理
void CHeavyWarrior::UpdateHit()
{
	if (!mIsGravity) mIsGravity = true;
	// ステップごとに処理を分ける
	switch (mStateStep)
	{
		// ステップ0：仰け反りアニメーション再生
	case 0:
		// 先行入力コライダーは最初はオフにしておく
		mpTACol->SetEnable(false);
		ChangeAnimation((int)EAnimType::eHit, true);
		mStateStep++;
		mAttStep = 0;
		break;
		// ステップ1：アニメーション終了待ち
	case 1:
		// 仰け反りアニメーションが終了したら、
		// 待機状態へ戻す
		if (IsAnimationFinished())
		{
			// 待機状態へ移行
			ChangeState((int)EState::eIdle);
			ChangeAnimation((int)EAnimType::eIdle);
		}
		break;
	}
}

// 死亡状態の更新処理
void CHeavyWarrior::UpdateDeath()
{
	// ステップごとに処理を分ける
	switch (mStateStep)
	{
		// ステップ0：死亡アニメーション再生
	case 0:
		mMoveSpeed = CVector::zero;
		ChangeAnimation((int)EAnimType::eDeath, true);
		mDeathVec = -VectorZ();
		mToDeath = true;
		SetInvincible(true);
		mStateStep++;
		break;
		// ステップ1：死亡アニメーション着地待ち
	case 1:
		if (GetAnimationFrame() >= DEATH_END_FRAME)
		{
			mToDeath = false;
			mStateStep++;
		}

		if (mToDeath)
		{
			// 1秒あたりの移動速度
			CVector move = mDeathVec * 20.0f * Times::DeltaTime();
			Position(Position() + move);
		}
		break;
		// ステップ2：アニメーション終了待ち
	case 2:
		// 死亡アニメーションが終了したら、削除
		if (IsAnimationFinished())
		{
			mStateStep++;
		}
		break;
		// ステップ3：死亡後の待ち
	case 3:
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

void CHeavyWarrior::UpdateVictory()
{
	switch (mStateStep)
	{
	case 0:
		mMoveSpeed = CVector::zero;
		// 勝利アニメーションを再生
		ChangeAnimation((int)EAnimType::eVictory);
		mStateStep++;
		break;
	case 1:
		mStateStep++;
		break;
	case 2:
		break;
	}
}

void CHeavyWarrior::AttPattGearBox()
{
	if (GetDistToTarget() > ATTACK_RANGE + (mStepMag * 0.5f))
	{
		// Idleへ移行
		ChangeState((int)EState::eIdle);
		mAttPattern = (int)EAttPattern::None;
	}
	switch (mAttPattern)
	{
	case (int)EAttPattern::PatternA:
		AttPatternA();
		break;
	case (int)EAttPattern::PatternB:
		AttPatternB();
		break;
	case (int)EAttPattern::PatternC:
		AttPatternC();
		break;
	case (int)EAttPattern::PatternD:
		AttPatternD();
		break;
	default:
		break;
	}
}

void CHeavyWarrior::AttPatternA()
{
	switch (mAttStep)
	{
	case 0:
		// Attack1へ移行
		ChangeState((int)EState::eAttack1);
		break;
	case 1:
		mAttStep = 0;
		int rand = Math::Rand(1, 100);
		// 確率で隙ができる
		if (rand <= mNegProb)
		{
			// Neglectへ移行
			ChangeState((int)EState::eNeglect);
		}
		else
		{
			// Idleへ移行
			ChangeState((int)EState::eIdle);
		}
		break;
	}
}

void CHeavyWarrior::AttPatternB()
{
	switch (mAttStep)
	{
	case 0:
		// Attack1へ移行
		ChangeState((int)EState::eAttack1);
		break;
	case 1:
		// Attack2へ移行
		ChangeState((int)EState::eAttack2);
		break;
	case 2:
		mAttStep = 0;
		int rand = Math::Rand(1, 100);
		// 確率で隙ができる
		if (rand <= mNegProb)
		{
			// Neglectへ移行
			ChangeState((int)EState::eNeglect);
		}
		else
		{
			// Idleへ移行
			ChangeState((int)EState::eIdle);
		}
		break;
	}
}

void CHeavyWarrior::AttPatternC()
{
	switch (mAttStep)
	{
	case 0:
		// Attack1へ移行
		ChangeState((int)EState::eAttack1);
		break;
	case 1:
		// Attack2へ移行
		ChangeState((int)EState::eAttack2);
		break;
	case 2:
		// AttackXへ移行
		ChangeState((int)EState::eAttackX);
		break;
	case 3:
		mAttStep = 0;
		int rand = Math::Rand(1, 100);
		// 確率で隙ができる
		if (rand <= mNegProb)
		{
			// Neglectへ移行
			ChangeState((int)EState::eNeglect);
		}
		else
		{
			// Idleへ移行
			ChangeState((int)EState::eIdle);
		}
		break;
	}
}

void CHeavyWarrior::AttPatternD()
{
}

// 更新
void CHeavyWarrior::Update()
{
	// 状態に合わせて、更新処理を切り替える
	switch ((EState)mState)
	{
		// 戦闘準備状態
	case EState::eReserve:	UpdateReserve();	break;
		// 待機状態
	case EState::eIdle:		UpdateIdle();		break;
		// 追いかける
	case EState::eChase:	UpdateChase();		break;
		// 斬り攻撃1
	case EState::eAttack1:	UpdateAttack1();	break;
		// 斬り攻撃1B
	case EState::eAttack1B:	UpdateAttack1B();	break;
		// 斬り攻撃2
	case EState::eAttack2:	UpdateAttack2();	break;
		// 斬り攻撃X
	case EState::eAttackX:	UpdateAttackX();	break;
		// 回避:右
	case EState::eAvoidR:	UpdateAvoidR();		break;
		// 回避:左
	case EState::eAvoidL:	UpdateAvoidL();		break;
		// 後隙
	case EState::eNeglect:	UpdateNeg();		break;
		// 仰け反り
	case EState::eHit:		UpdateHit();		break;
		// 死亡状態
	case EState::eDeath:	UpdateDeath();		break;
		// 勝利
	case EState::eVictory:	UpdateVictory();	break;
	}

	if (mState == (int)EState::eIdle ||
		mState == (int)EState::eChase)
	{
		UpdateBattleTempo();
	}

	// 敵のベースクラスの更新
	CEnemy::Update();

	// 武器の行列を更新
	mpSword->UpdateMtx();

	if (Position().Y() < -100.0f)
	{
		Position(0.0f, 10.0f, -100.0f);
	}

#ifdef _DEBUG
	// 戦闘相手までの距離をデバッグ表示
	if (mpBattleTarget != nullptr)
	{
		CVector targetPos = mpBattleTarget->Position();
		CVector pos = Position();
		targetPos.Y(pos.Y());
		float dist = CVector::Distance(targetPos, pos);
		CDebugPrint::Print("Dist:%.2f\n", dist);
		CDebugPrint::Print("EnemyHP:%.2f\n", mHp);
	}
	CDebugPrint::Print("EnemyState:%d\n", mState);
	CDebugPrint::Print("EnemyAnimType:%d\n", mAnimType);
#endif // _DEBUG
}

void CHeavyWarrior::SetInBattle(int state)
{
	if (state == 0)
	{
		// 待機状態へ移行
		ChangeState((int)EState::eIdle);
	}
	if (state == 2)
	{
		// 勝利状態へ移行
		ChangeState((int)EState::eVictory);
	}
}