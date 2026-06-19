#pragma once
#include "CTask.h"

class CFont;
class CImage;

class CTutorialUI : public CTask
{
public:
	// コンストラクタ
	CTutorialUI();
	// デストラクタ
	~CTutorialUI();

	// 更新
	void Update() override;
	// 描画
	void Render() override;

	// チュートリアルの進行度
	enum class EState
	{
		eTuto0,		// チュートリアルへようこそ
		eTuto1,		// 移動と攻撃
		eTuto2,		// 回避とジャスト回避
		eTuto3,		// ジャスト回避後のカウンター攻撃
		eTuto4,		// キック
		eTuto5,		// ジャストキック
		eTuto6,		// 敵が動くまでの準備期間
		eTuto7,		// 敵が動き出す
		eTuto8,		// プレイヤーの勝利
	};

	// 状態切り替え
	void ChangeState(EState state);

private:
	// 情報更新
	void InformationUpdate();

	EState mState;		// 現在の状態
	int mStateStep;		// 状態内でのステップ管理用
	float mElapsedTime;	// 経過時間計測用

	CImage* mpTextBg;		// テキストの背景イメージ
	CImage* mpIconImg;		// アイコンのイメージ
	CFont* mpFont;			// チュートリアルのフォント
	CText* mpText;			// チュートリアルのテキスト
};