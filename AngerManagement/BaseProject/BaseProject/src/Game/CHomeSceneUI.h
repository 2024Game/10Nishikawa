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
	// ゲームを終了するか
	bool IsExitGame() const;
	// 訓練所へ移行するか
	bool IsGoBarracks() const;

	// 更新
	void Update() override;
	// 描画
	void Render() override;

	// 情報更新
	void InformationUpdate();

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
	// [訓練所へ移行]クリック時のコールバック関数
	void OnClickGoBarracks();
	// [引退する]クリック時のコールバック関数
	void OnClickGoReincarnation();
	// [QUIT]クリック時のコールバック関数
	void OnClickQuit();

	// [治療院へ行く]クリック時のコールバック関数
	void OnClickHealer();
	// [格下との戦い]クリック時のコールバック関数
	void OnClickEasy();
	// [同格との戦い]クリック時のコールバック関数
	void OnClickNormal();
	// [格上との戦い]クリック時のコールバック関数
	void OnClickHard();


	EState mState;		// 現在の状態
	int mStateStep;		// 状態内でのステップ管理用
	float mElapsedTime;	// 経過時間計測用
	bool mIsEnd;		// タイトル画面終了フラグ
	int mSelectIndex;	// 現在選択している項目

	int mRows;    // 1列あたりの項目数
	int mCols;    // 列数 <- UIのサイズ的に１列で良かった(つまり頑張り損)

	CSaveManager* mpSaveManager;
	CImage* mpHomeBg;			// 背景イメージ
	CFont* mpMoneyFont;			// 所持金のフォント
	CFont* mpStatusFont;		// ステータスのフォント
	CFont* mpPerkFont;			// 強化パークのフォント
	CText* mpDayText;			// 日数のテキスト
	CText* mpMoneyText;			// 所持金のテキスト
	CText* mpStatusText;		// ステータスのテキスト
	CText* mpStartText;			// 「CLICK TO START」のテキスト
	std::vector<CExpandButton*> mButtons;
	std::vector<CImage*> mBgImages;
	std::vector<CText*> mURTexts; // Update Required Texts (要更新テキスト)
	std::vector<CText*> mTexts;
};