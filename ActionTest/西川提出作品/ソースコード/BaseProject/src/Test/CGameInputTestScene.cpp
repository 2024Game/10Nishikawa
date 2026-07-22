#include "CGameInputTestScene.h"

// コンストラクタ
CGameInputTestScene::CGameInputTestScene()
	: CSceneBase(EScene::eGameInputTest)
	, mpGameInput(nullptr)
{
	// GameInputの本体オブジェクトを作成
	if (FAILED(GameInputCreate(&mpGameInput)))
	{
		printf("GameInputオブジェクト作成失敗\n");
		return;
	}
}

// デストラクタ
CGameInputTestScene::~CGameInputTestScene()
{
	// GameInputオブジェクトが作成されていたら、解放する
	if (mpGameInput != nullptr)
	{
		mpGameInput->Release();
	}
}

// シーン読み込み
void CGameInputTestScene::Load()
{
	// 背景色設定
	System::SetClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

// 入力更新処理（GameInputKindController用）
void CGameInputTestScene::UpdateController(IGameInputReading* reading)
{
	// コントローラーの軸入力数を取得（左スティックや右スティックなど）
	uint32_t axisCount = reading->GetControllerAxisCount();
	// コントローラーのボタン数を取得
	uint32_t buttonCount = reading->GetControllerButtonCount();
	// コントローラーのスイッチ数を取得（十字ボタンなど）
	uint32_t switchCount = reading->GetControllerSwitchCount();

	CDebugPrint::Print("■GameInputKindController\n");
	CDebugPrint::Print("\n");

	// スティックなどの軸入力が存在すれば
	if (axisCount > 0)
	{
		CDebugPrint::Print("●軸入力情報\n");

		// 軸入力数の配列を作成して、現在の状態を取得
		float* axes = new float[axisCount];
		uint32_t readAxisCount =
			reading->GetControllerAxisState(axisCount, axes);

		for (uint32_t i = 0; i < readAxisCount; i++)
		{
			CDebugPrint::Print("Axis[%d] %.2f\n", i, axes[i]);
		}

		// 確保した配列を解放
		delete[] axes;
		CDebugPrint::Print("\n");
	}

	// ボタンが存在すれば
	if (buttonCount > 0)
	{
		CDebugPrint::Print("●ボタン情報\n");

		// ボタンの個数分のboolの配列を作成
		bool* buttons = new bool[buttonCount];
		// 各ボタンの現在の入力状態を取得
		uint32_t readButtonCount =
			reading->GetControllerButtonState(buttonCount, buttons);

		// 入力されていたら、ボタンの番号を表示
		for (uint32_t i = 0; i < readButtonCount; i++)
		{
			if (buttons[i]) CDebugPrint::Print("[%d]\n", i);
		}

		// 作成した配列を解放
		delete[] buttons;
		CDebugPrint::Print("\n");
	}

	// 十字ボタンなどのスイッチが存在すれば
	if (switchCount > 0)
	{
		CDebugPrint::Print("●スイッチ情報\n");

		// 各スイッチの配列を作成し、現在の入力状態を取得
		GameInputSwitchPosition* switches = new GameInputSwitchPosition[switchCount];
		uint32_t readSwitchCount =
			reading->GetControllerSwitchState(switchCount, switches);

		for (uint32_t i = 0; i < readSwitchCount; i++)
		{
			switch (switches[i])
			{
			case GameInputSwitchCenter:		CDebugPrint::Print("[入力なし]\n");	break;
			case GameInputSwitchUp:			CDebugPrint::Print("[↑]\n");		break;
			case GameInputSwitchDown:		CDebugPrint::Print("[↓]\n");		break;
			case GameInputSwitchLeft:		CDebugPrint::Print("[←]\n");		break;
			case GameInputSwitchRight:		CDebugPrint::Print("[→]\n");		break;
			case GameInputSwitchUpRight:	CDebugPrint::Print("[↑→]\n");		break;
			case GameInputSwitchDownRight:	CDebugPrint::Print("[↓→]\n");		break;
			case GameInputSwitchUpLeft:		CDebugPrint::Print("[↑←]\n");		break;
			case GameInputSwitchDownLeft:	CDebugPrint::Print("[↓←]\n");		break;
			}
		}

		// 作成した配列を解放
		delete[] switches;
		CDebugPrint::Print("\n");
	}
}

// 入力更新処理（GameInputKindGamepad用）
void CGameInputTestScene::UpdateGamePad(IGameInputReading* reading)
{
	// コントローラーの現在の入力状態を取得
	GameInputGamepadState state;
	reading->GetGamepadState(&state);

	CDebugPrint::Print("■GameInputKindGamepad\n");

	CDebugPrint::Print("\n");
	CDebugPrint::Print("●アナログ情報\n");
	// 左スティック
	CDebugPrint::Print("LStick [X:%.2f, Y:%.2f]\n", state.leftThumbstickX, state.leftThumbstickY);
	// 右スティック
	CDebugPrint::Print("RStick [X:%.2f, Y:%.2f]\n", state.rightThumbstickX, state.rightThumbstickY);
	// 左トリガー
	CDebugPrint::Print("LTrigger [%.2f]\n", state.leftTrigger);
	// 右トリガー
	CDebugPrint::Print("RTrigger [%.2f]\n", state.rightTrigger);

	CDebugPrint::Print("\n");
	CDebugPrint::Print("●デジタル情報\n");
	if (state.buttons & GameInputGamepadNone) CDebugPrint::Print("[None]\n");
	if (state.buttons & GameInputGamepadMenu) CDebugPrint::Print("[Menu]\n");
	if (state.buttons & GameInputGamepadView) CDebugPrint::Print("[View]\n");

	if (state.buttons & GameInputGamepadA) CDebugPrint::Print("[A]\n");
	if (state.buttons & GameInputGamepadB) CDebugPrint::Print("[B]\n");
	if (state.buttons & GameInputGamepadC) CDebugPrint::Print("[C]\n");
	if (state.buttons & GameInputGamepadX) CDebugPrint::Print("[X]\n");
	if (state.buttons & GameInputGamepadY) CDebugPrint::Print("[Y]\n");
	if (state.buttons & GameInputGamepadZ) CDebugPrint::Print("[Z]\n");

	if (state.buttons & GameInputGamepadDPadUp)		CDebugPrint::Print("[↑]\n");
	if (state.buttons & GameInputGamepadDPadDown)	CDebugPrint::Print("[↓]\n");
	if (state.buttons & GameInputGamepadDPadLeft)	CDebugPrint::Print("[←]\n");
	if (state.buttons & GameInputGamepadDPadRight)	CDebugPrint::Print("[→]\n");

	if (state.buttons & GameInputGamepadLeftShoulder)		CDebugPrint::Print("[LS]\n");
	if (state.buttons & GameInputGamepadRightShoulder)		CDebugPrint::Print("[RS]\n");
	if (state.buttons & GameInputGamepadLeftTriggerButton)	CDebugPrint::Print("[LT]\n");
	if (state.buttons & GameInputGamepadRightTriggerButton)	CDebugPrint::Print("[RT]\n");

	if (state.buttons & GameInputGamepadLeftThumbstick)		CDebugPrint::Print("[LThumb]\n");
	if (state.buttons & GameInputGamepadLeftThumbstickUp)	CDebugPrint::Print("[L↑]\n");
	if (state.buttons & GameInputGamepadLeftThumbstickDown)	CDebugPrint::Print("[L↓]\n");
	if (state.buttons & GameInputGamepadLeftThumbstickLeft)	CDebugPrint::Print("[L←]\n");
	if (state.buttons & GameInputGamepadLeftThumbstickRight)CDebugPrint::Print("[L→]\n");

	if (state.buttons & GameInputGamepadRightThumbstick)		CDebugPrint::Print("[RThumb]\n");
	if (state.buttons & GameInputGamepadRightThumbstickUp)		CDebugPrint::Print("[R↑]\n");
	if (state.buttons & GameInputGamepadRightThumbstickDown)	CDebugPrint::Print("[R↓]\n");
	if (state.buttons & GameInputGamepadRightThumbstickLeft)	CDebugPrint::Print("[R←]\n");
	if (state.buttons & GameInputGamepadRightThumbstickRight)	CDebugPrint::Print("[R→]\n");

	if (state.buttons & GameInputGamepadPaddleLeft1)	CDebugPrint::Print("[PL1]\n");
	if (state.buttons & GameInputGamepadPaddleLeft2)	CDebugPrint::Print("[PL2]\n");
	if (state.buttons & GameInputGamepadPaddleRight1)	CDebugPrint::Print("[PL3]\n");
	if (state.buttons & GameInputGamepadPaddleRight2)	CDebugPrint::Print("[PL4]\n");
}

// 更新
void CGameInputTestScene::Update()
{
	IGameInputReading* reading;

	if (SUCCEEDED(mpGameInput->GetCurrentReading(
		GameInputKindController | GameInputKindGamepad,
		nullptr, &reading)))
	{
		// 接続されているコントローラーの種類に応じて、更新処理を切り替える
		GameInputKind kind = reading->GetInputKind();
		// XInput形式のコントローラーはGameInputKindGamepad
		if (kind & GameInputKindGamepad)
		{
			UpdateGamePad(reading);
		}
		// DirectInput形式のコントローラーはGameInputKindController
		else if (kind & GameInputKindController)
		{
			UpdateController(reading);
		}
	}
}
