#include "CMinimap.h"
#include "CPlayer.h"
#include "CFish.h"
#include "CFishManager.h"

CMinimap::CMinimap()
	:mpFishfinderSound(nullptr)
{
	SetPos(1280.0f - 125.0f, 125.0f);
	CVector2 size = CVector2(175.0f, 175.0f);
	SetSize(size);
	SetCenter(size * 0.5f);
	mUpdateTimer = 0.0f;
	// SEデータ取得
	mpFishfinderSound = CResourceManager::Get<CSound>("FishfinderSound");
}

CMinimap::~CMinimap()
{
}

void CMinimap::Render()
{
	const float screenWidth = 1280.0f;
	const float screenHeight = 720.0f;
	const float mapWorldRadius = 250.0f;

	CPlayer* player = CPlayer::Instance();
	if (!player) return;
	CVector playerPos = player->Position();

	CVector forward = player->VectorZ();
	float rad = atan2f(forward.X(), forward.Z());

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	DrawFilledCircle(mPosition.X(), mPosition.Y(), mSize.X() * 0.5f, 0.1f, 0.1f, 0.1f, 0.7f);

	const std::vector<CFish*> fishes = CFishManager::Instance()->GetFishes();

	// 5秒ごとに魚の座標リストを更新
	mUpdateTimer += Times::DeltaTime();
	if (mUpdateTimer >= 5.0f)
	{
		mFishMapInfos.clear();
		mFishMapInfos.reserve(fishes.size());

		for (CFish* fish : fishes)
		{
			FishMapInfo info;
			info.position = fish->Position();
			mFishMapInfos.push_back(info);
		}
		mpFishfinderSound->Play(0.1f,true);
		mUpdateTimer = 0.0f;
	}

	// 🐟 登録済み魚座標を描画
	for (const FishMapInfo& info : mFishMapInfos)
	{
		float dx = info.position.X() - playerPos.X();
		float dz = info.position.Z() - playerPos.Z();
		float dist = sqrtf(dx * dx + dz * dz);
		if (dist > mapWorldRadius) continue;

		float rotatedX = -(dx * cosf(rad) - dz * sinf(rad));
		float rotatedZ = dx * sinf(rad) + dz * cosf(rad);

		float scale = mSize.X() * 0.5f / mapWorldRadius;
		float drawX = mPosition.X() + rotatedX * scale;
		float drawY = mPosition.Y() - rotatedZ * scale;

		DrawFilledCircle(drawX, drawY, 3.0f, 1.0f, 0.25f, 0.25f, 1.0f);
	}

	DrawFilledCircle(mPosition.X(), mPosition.Y(), 5.0f, 0.2f, 1.0f, 0.2f, 1.0f);
	glDisable(GL_BLEND);
}

void CMinimap::DrawFilledCircle(float cx, float cy, float radius, float r, float g, float b, float a)
{
	glColor4f(r, g, b, a);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(cx, cy); // 中心
	const int numSegments = 32;
	for (int i = 0; i <= numSegments; i++)
	{
		float theta = 2.0f * 3.1415926f * float(i) / float(numSegments);
		float x = radius * cosf(theta);
		float y = radius * sinf(theta);
		glVertex2f(cx + x, cy + y);
	}
	glEnd();
}
