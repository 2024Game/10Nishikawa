#pragma once
#include "CTask.h"
#include "CSaveManager.h"

class CFont;
class CImage;
class CExpandButton;

class CBarracksSceneUI : public CTask
{
public:
	// コンストラクタ
	CBarracksSceneUI(CSaveManager* saveManager);
	// デストラクタ
	~CBarracksSceneUI();

	// 強化画面終了か
	bool IsEnd() const;
	// タイトルへ移行するか
	bool IsGoHome() const;

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

	// [タイトルに戻る]クリック時のコールバック関数
	void OnClickGoHome();

	// [ページめくり]クリック時のコールバック関数
	void OnClickPageUp();
	// [ページめくり]クリック時のコールバック関数
	void OnClickPageDown();

	// [体力UP]クリック時のコールバック関数
	void OnClickHpUp();
	// [試合後HP回復UP]クリック時のコールバック関数
	void OnClickHpRegeneUp();
	// [スタミナUP]クリック時のコールバック関数
	void OnClickStUp();
	// [スタミナ回復UP]クリック時のコールバック関数
	void OnClickStRegeneUp();
	// [攻撃力UP]クリック時のコールバック関数
	void OnClickAttackUp();
	// [治療院の効果UP]クリック時のコールバック関数
	void OnClickHealerUp();
	// [引き継ぎLvUP]クリック時のコールバック関数
	void OnClickReincarnationUp();


	EState mState;		// 現在の状態
	int mStateStep;		// 状態内でのステップ管理用
	float mElapsedTime;	// 経過時間計測用
	bool mIsEnd;		// タイトル画面終了フラグ
	int mSelectIndex;	// 現在選択している項目
	int mPageNum;		// 現在表示しているページ

	int mRows;    // 1列あたりの項目数
	int mCols;    // 列数 <- UIのサイズ的に１列で良かった(つまり頑張り損)

	CSaveManager* mpSaveManager;
	CImage* mpBarracksBg;			// 背景イメージ
	CFont* mpMoneyFont;				// 所持金のフォント
	CFont* mpStatusFont;			// ステータスのフォント
	CFont* mpPerkFont;				// 強化項目のフォント
	CText* mpDayText;				// 日数のテキスト
	CText* mpMoneyText;				// 所持金のテキスト
	CText* mpStatusText;			// ステータスのテキスト
	std::vector<CExpandButton*> mButtons;
	std::vector<CExpandButton*> mURButtons;	// Update Required Buttons (要更新ボタン)
	std::vector<CImage*> mBgImages;
	std::vector<CText*> mURTexts;	// Update Required Texts (要更新テキスト)
	std::vector<CText*> mTexts;
};