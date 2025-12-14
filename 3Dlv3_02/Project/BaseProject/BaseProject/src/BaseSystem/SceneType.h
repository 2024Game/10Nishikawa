#pragma once

//シーンの種類
enum class EScene
{
	eNone,	//どのシーンにも所属していない
	
	eTitle,	// タイトルシーン
	eGame,	// ゲームシーン
	eHome,	// 強化等を行うシーン

	eBootMenu,	//起動時メニュー
	eColTest,	//衝突判定テスト
};
