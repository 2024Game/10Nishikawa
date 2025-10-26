#include "BGMData.h"

// BGMデータのテーブル
const BGMData BGM_DATA[] =
{
	//BGMの種類			リソースパス										ベース音量		ループ	ループ開始	ループ終了
	{ EBGMType::eTitle,	"Sound\\BGM\\ラテン・自然・海上のテラス.wav",		1.0f,			true,		0,			0		},
	{ EBGMType::eGame,	"Sound\\BGM\\ラテン・自然・穏やかで深い海.wav",		1.0f,			true,		0,			0		},
	{ EBGMType::eMenu,	"Sound\\BGM\\menu.wav",								0.0625f,		true,		0,			0		},
};

// BGMデータ数
const int BGM_DATA_COUNT = ARRAY_SIZE(BGM_DATA);