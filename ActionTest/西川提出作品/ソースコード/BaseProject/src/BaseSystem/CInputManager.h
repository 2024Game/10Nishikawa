#pragma once
#include "CInput.h"
#include "GameInput.h"

using namespace GameInput::v3;

// 入力を一括にまとめるクラス
class CInputManager
{
public:
	/// <summary>
	/// キーの入力状態を更新
	/// （呼び出しは1フレームに1回まで）
	/// </summary>
	static void Update();
private:
	// コンストラクタ
	//（インスタンスを生成できないようにprivate）
	CInputManager();

	// GameInputの本体オブジェクト
	IGameInput* mpGameInput;
};