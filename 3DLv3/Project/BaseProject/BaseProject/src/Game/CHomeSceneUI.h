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

	// [燃料タンク容量UP]クリック時のコールバック関数
	void OnClickIncreaseHP();
	// [船体スピードUP]クリック時のコールバック関数
	void OnClickIncreaseBoatSpeed();
	// [爆弾の​沈下速度UP]クリック時のコールバック関数
	void OnClickIncreaseBarrelSpeed();
	// [爆弾の​威力UP]クリック時のコールバック関数
	void OnClickIncreaseBlastPower();
	// [爆弾の​爆発半径UP]クリック時のコールバック関数
	void OnClickIncreaseBlastRadius();
	// [爆弾の​追尾性能UP]クリック時のコールバック関数
	void OnClickIncreaseBarrelTracking();

	// [OPTION]クリック時のコールバック関数
	void OnClickOption();
	// [QUIT]クリック時のコールバック関数
	void OnClickQuit();

	EState mState;		// 現在の状態
	int mStateStep;		// 状態内でのステップ管理用
	float mElapsedTime;	// 経過時間計測用
	bool mIsEnd;		// タイトル画面終了フラグ
	int mSelectIndex;	// 現在選択している項目

	int mRows;    // 1列あたりの項目数
	int mCols;    // 列数 <- UIのサイズ的に１列で良かった(つまり頑張り損)

	CSaveManager* mpSaveManager;
	CFont* mpLogoFont;		// タイトルロゴのフォント
	CText* mpMoneyText;		// 所持金のテキスト
	CText* mpStatusText;	// ステータスのテキスト
	CText* mpStartText;		// 「CLICK TO START」のテキスト
	std::vector<CExpandButton*> mButtons;
	std::vector<CImage*> mBgImages;
	std::vector<CText*> mURTexts; // Update Required Texts (要更新テキスト)
	std::vector<CText*> mTexts;
};