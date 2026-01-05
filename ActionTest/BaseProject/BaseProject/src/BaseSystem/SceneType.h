#pragma once

//シーンの種類
enum class EScene
{
	eNone,	//どのシーンにも所属していない

	eTitle,		//タイトルシーン
	eHome,		//ホームシーン
	eBarracks,	//兵舎シーン
	eGame,		//ゲームシーン

	eBootMenu,	//起動時メニュー
	eColTest,	//衝突判定テスト
};
