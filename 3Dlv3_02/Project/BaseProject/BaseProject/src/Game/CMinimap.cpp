#include "CMinimap.h"
#include "CPlayer.h"
#include "CFish.h"
#include "CFishManager.h"
#include "CImage.h"

CMinimap::CMinimap()
	:mpFishfinderSound(nullptr)
{
	SetPos(WINDOW_WIDTH - 125.0f, 125.0f);
	CVector2 size = CVector2(175.0f, 175.0f);
	SetSize(size);
	SetCenter(size * 0.5f);
	mUpdateTimer = 0.0f;
	// SEデータ取得
	mpFishfinderSound = CResourceManager::Get<CSound>("FishfinderSound");

	// タイトル画面の背景イメージを生成
	mpRadarImage = new CImage
	(
		"UI/radar.png",
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eDefault,
		false,
		false
	);
	mpRadarImage->SetSize(size * 1.15f);
	mpRadarImage->SetCenter((size * 1.15f) * 0.5f);
	mpRadarImage->SetPos(WINDOW_WIDTH - 125.0f, 125.0f);
	mpRadarImage->SetAlpha(0.8f);
}

CMinimap::~CMinimap()
{
	SAFE_DELETE(mpRadarImage);
}

void CMinimap::Render()
{
	//mpRadarImage->Update();
	const float screenWidth = WINDOW_WIDTH;
	const float screenHeight = WINDOW_HEIGHT;
	const float mapWorldRadius = 250.0f;

	CPlayer* player = CPlayer::Instance();
	if (!player) return;
	CVector playerPos = player->Position();

	CVector forward = player->VectorZ();
	float rad = atan2f(forward.X(), forward.Z());

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//DrawFilledCircle(mPosition.X(), mPosition.Y(), mSize.X() * 0.5f, 0.1f, 0.1f, 0.1f, 0.27f);
	// レーダー背景描画
	mpRadarImage->Render();

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

	// 🐟 登録済みの魚の座標を描画
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

		if (info.position.Y() > -75.0f)
		{
			DrawFilledCircle(drawX, drawY, 3.0f, 1.0f, 0.25f, 0.25f, 1.0f);
		}
		else
		{
			DrawFilledCircle(drawX, drawY, 3.0f, 0.6f, 0.15f, 0.15f, 1.0f);
		}

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
