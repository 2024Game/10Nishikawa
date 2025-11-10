#include "CMinimap.h"
#include "CPlayer.h"
#include "CFish.h"
#include "CFishManager.h"

CMinimap::CMinimap()
{

}

CMinimap::~CMinimap()
{
}

void CMinimap::Render()
{
	// ウィンドウ解像度
	const float screenWidth = 1080.0f;
	const float screenHeight = 720.0f;

	// ミニマップ設定
	const float mapWorldRadius = 300.0f;
	const float mapPixelRadius = 100.0f;
	const float mapCenterX = 150.0f;
	const float mapCenterY = 150.0f; // 左下固定

	CPlayer* player = CPlayer::Instance();
	if (!player) return;
	CVector playerPos = player->Position();

	// --- 深度無効化して2Dモードへ ---
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// --- 背景円 ---
	DrawFilledCircle(mapCenterX, mapCenterY, mapPixelRadius, 0.15f, 0.15f, 0.15f, 0.7f);

	// --- 魚を描く ---
	const std::vector<CFish*> fishes = CFishManager::Instance()->GetFishes();
	for (CFish* fish : fishes)
	{
		float dx = fish->Position().X() - playerPos.X();
		float dz = fish->Position().Z() - playerPos.Z();
		float dist = sqrtf(dx * dx + dz * dz);
		if (dist > mapWorldRadius) continue;

		float scale = mapPixelRadius / mapWorldRadius;
		float mapX = dx * scale;
		float mapY = dz * scale;

		float drawX = mapCenterX + mapX;
		float drawY = mapCenterY - mapY;

		DrawFilledCircle(drawX, drawY, 3.0f, 1.0f, 0.3f, 0.3f, 1.0f);
	}

	// --- プレイヤー ---
	DrawFilledCircle(mapCenterX, mapCenterY, 5.0f, 0.2f, 1.0f, 0.2f, 1.0f);

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
