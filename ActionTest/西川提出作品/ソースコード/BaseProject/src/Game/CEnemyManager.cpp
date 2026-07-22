#include "CEnemyManager.h"
#include "CField.h"
#include "Maths.h"

CEnemyManager* CEnemyManager::mpInstance = nullptr;

CEnemyManager::CEnemyManager()
{

}

CEnemyManager::~CEnemyManager()
{

}

CEnemyManager* CEnemyManager::Instance()
{
	if (mpInstance == nullptr) 
	{
		mpInstance = new CEnemyManager();
	}
	return mpInstance;
}

void CEnemyManager::ClearInstance()
{
	SAFE_DELETE(mpInstance);
}

void CEnemyManager::Add(CEnemy* enemy)
{
	mEnemies.push_back(enemy);
}

void CEnemyManager::Remove(CEnemy* enemy)
{
	auto result = std::remove(mEnemies.begin(), mEnemies.end(), enemy);
	mEnemies.erase(result, mEnemies.end());
}

void CEnemyManager::SetInBattle(int num)
{
	if (mEnemies.empty()) return;

	for (CEnemy* enemy : mEnemies)
	{
		if (enemy != nullptr && enemy->GetHp() > 0.0f)
		{
			enemy->SetInBattle(num);
		}
	}
}

// 指定したエネミーが存在するかどうか
bool CEnemyManager::Exist(CEnemy* enemy) const
{
	auto find = std::find(mEnemies.begin(), mEnemies.end(), enemy);
	return find != mEnemies.end();
}

bool CEnemyManager::Surviv() const
{
	if (mEnemies.empty()) return false;

	for (CEnemy* enemy : mEnemies)
	{
		if (enemy != nullptr && enemy->GetHp() > 0.0f)
		{
			return true;
		}
	}
	return false;
}

CEnemy* CEnemyManager::GetLockOnTarget(float lockOnDist)
{
	CCamera* camera = CCamera::CurrentCamera();
	if (camera == nullptr) return nullptr;

	// カメラ位置を取得
	CVector camaraPos = camera->Position();
	// スクリーンの範囲を取得
	CRect screenRect = CRect(0.0f, 0.0f, WINDOW_WIDTH, WINDOW_HEIGHT);
	CVector2 center = CVector2(WINDOW_WIDTH, WINDOW_HEIGHT) * 0.5f;

	// 生成済みのエネミーの中で、最も適したエネミーを選択
	float maxScore = 0.0f;
	CEnemy* bestEnemy = nullptr;

	for (CEnemy* enemy : mEnemies)
	{
		// エネミーのワールド座標
		CVector wpos = enemy->Position() + enemy->GetLockOnOffsetPos();
		// エネミーのスクリーン座標
		// スクリーン座標はx,yだが、zにカメラからの距離が入る
		CVector spos = camera->WorldToScreenPos(wpos);

		// カメラからの距離がマイナス(後ろ)にいる場合は、対象ではない
		if (spos.Z() < 0.0f) continue;
		// 画面外にいる場合も対象ではない
		if (!CCollider::CollisionRectPoint(screenRect, spos)) continue;

		float wdist = CVector::Distance(camaraPos, wpos);
		// カメラからの距離が離れすぎていても、対象ではない
		if (wdist > lockOnDist) continue;

		// カメラとエネミーとの間に遮蔽物があれば、対象ではない
		if (CField::CollisionRay(camaraPos, wpos)) continue;

		float sdist = CVector2::Distance(center, spos);

		// wdist,sdist をそれぞれ正規化(Normalization)する
		float wnorm = 1.0f - Math::Clamp01(wdist / lockOnDist);
		float snorm = 1.0f - Math::Clamp01(sdist / WINDOW_WIDTH);

		// 合計スコアにおけるワールド空間のスコアの比率(重み)
		static const float wweight = 0.3f;
		// 合計スコア = ワールドスコア(3割) + スクリーンスコア(7割)
		float score = wnorm * wweight + snorm * (1.0f - wweight);

		// 合計スコアが現在のmaxScoreを超えていたら
		if (score > maxScore)
		{
			// ロックオン対象を更新
			bestEnemy = enemy;
			maxScore = score;
		}
	}

	return bestEnemy;
}
