#pragma once
#include "CSceneBase.h"
#include <memory>
#include "CFish.h"
class CGameMenu;
class CSound;

//タイトルシーン
class CGameScene :public CSceneBase
{
public:
	//コンストラクタ
	CGameScene();
	//デストラクタ
	~CGameScene();
	//シーン読み込み
	void Load();
	//シーンの更新処理
	void Update();

	void DrawMiniMap();

	void DrawFilledCircle(float cx, float cy, float radius, float r, float g, float b, float a);

private:
	CGameMenu* mpGameMenu;

	std::vector<std::unique_ptr<CFish>> fishList;

	struct FishInfo
	{
		std::string typeName;  // 魚の種類（例："RainbowTrout"）
		float scale;           // スケール（1軸だけ保存するなら1つでOK）
		CVector position;      // 現在のワールド座標
	};
	std::vector<FishInfo> fishInfoList;
	std::vector<CRainbowTrout*> fishObjects; // 実際の魚へのポインタリスト
};
