#pragma once
#include "CSceneBase.h"
#include "GameInput.h"

using namespace GameInput::v3;

// GameInputテストシーン
class CGameInputTestScene : public CSceneBase
{
public:
	// コンストラクタ
	CGameInputTestScene();
	// デストラクタ
	~CGameInputTestScene();

	// シーン読み込み
	void Load() override;

	// 更新
	void Update() override;

private:
	// 入力更新処理（GameInputKindController用）
	void UpdateController(IGameInputReading* reading);
	// 入力更新処理（GameInputKindGamepad用）
	void UpdateGamePad(IGameInputReading* reading);

	IGameInput* mpGameInput;	// GameInputの本体オブジェクト
};