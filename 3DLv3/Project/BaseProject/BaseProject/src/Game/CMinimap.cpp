#include "CMinimap.h"
#include "CPlayer.h"
#include "CFish.h"
#include "CFishManager.h"

CMinimap::CMinimap()
{
	SetPos(1280.0f - 125.0f, 125.0f);
	CVector2 size = CVector2(175.0f, 175.0f);
	SetSize(size);
	SetCenter(size * 0.5f);
}

CMinimap::~CMinimap()
{
}

void CMinimap::Render()
{
	// ウィンドウ解像度
	const float screenWidth = 1280.0f;
	const float screenHeight = 720.0f;

	// ミニマップ設定
	const float mapWorldRadius = 300.0f;

	CPlayer* player = CPlayer::Instance();
	if (!player) return;
	CVector playerPos = player->Position();


	CVector forward = player->VectorZ(); // 前方向ベクトル（XZ平面のみ使用）
	// 2Dミニマップでの回転角を計算
	float rad = atan2f(forward.X(), forward.Z()); // X,Zから角度取得
	// プレイヤーの前が上になるように rad を使う


	// --- 深度無効化して2Dモードへ ---
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// --- 背景円 ---
	DrawFilledCircle(mPosition.X(), mPosition.Y(), mSize.X() * 0.5f, 0.15f, 0.15f, 0.15f, 0.7f);

	// --- 魚を描く ---
	const std::vector<CFish*> fishes = CFishManager::Instance()->GetFishes();
	for (CFish* fish : fishes)
	{
		float dx = fish->Position().X() - playerPos.X();
		float dz = fish->Position().Z() - playerPos.Z();
		float dist = sqrtf(dx * dx + dz * dz);
		if (dist > mapWorldRadius) continue;

		// Forwardベクトルに合わせて回転
		float rotatedX = dx * cosf(rad) - dz * sinf(rad);
		float rotatedZ = dx * sinf(rad) + dz * cosf(rad);

		float scale = mSize.X() * 0.5f / mapWorldRadius;
		float drawX = mPosition.X() - rotatedX * scale;
		float drawY = mPosition.Y() - rotatedZ * scale;

		DrawFilledCircle(drawX, drawY, 3.0f, 1.0f, 0.3f, 0.3f, 1.0f);
	}

	// --- プレイヤー ---
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
