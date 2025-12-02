#pragma once
#include "CTask.h"
#include "CSaveManager.h"

class CFont;
class CImage;
class CExpandButton;

class CHomeSceneUI : public CTask
{
public:
	// コンストラクタ
	CHomeSceneUI(CSaveManager* saveManager);
	// デストラクタ
	~CHomeSceneUI();

	// ホーム画面終了か
	bool IsEnd() const;
	// ゲームを開始するか
	bool IsStartGame() const;
	// タイトルへ移行するか
	bool IsGoTitle() const;

	// 更新
	void Update() override;
	// 描画
	void Render() override;

private:
	// 待機状態
	void UpdateIdle();
	// メニューを開く
	void UpdateOpen();
	// メニュー選択
	void UpdateSelect();
	// フェードアウト
	void UpdateFadeOut();

	// タイトルの状態
	enum class EState
	{
		eIdle,		// 待機状態
		eOpen,		// メニューを開く
		eSelect,	// メニュー選択
		eFadeOut,	// フェードアウト
	};
	// 状態切り替え
	void ChangeState(EState state);

	// [ゲーム開始]クリック時のコールバック関数
	void OnClickStartGame();
	// [タイトルに戻る]クリック時のコールバック関数
	void OnClickGoTitle();

	// [燃料タンク容量UP]クリック時のコールバック関数
	void OnClickIncreaseHP();
	// [OPTION]クリック時のコールバック関数
	void OnClickOption();
	// [QUIT]クリック時のコールバック関数
	void OnClickQuit();

	EState mState;		// 現在の状態
	int mStateStep;		// 状態内でのステップ管理用
	float mElapsedTime;	// 経過時間計測用
	bool mIsEnd;		// タイトル画面終了フラグ
	int mSelectIndex;	// 現在選択している項目

	CSaveManager* mpSaveManager;
	CFont* mpLogoFont;		// タイトルロゴのフォント
	CText* mpMoneyText;		// 所持金のテキスト
	CText* mpStatusText;	// ステータスのテキスト
	CText* mpStartText;		// 「CLICK TO START」のテキスト
	std::vector<CExpandButton*> mButtons;
	std::vector<CImage*> mBgImages;
	std::vector<CText*> mTexts;
};