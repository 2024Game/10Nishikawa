#pragma once
#include "CUIBase.h"

class CText;

class CGameSceneUI : public CUIBase
{
public:
	// コンストラクタ
	CGameSceneUI();
	// デストラクタ
	~CGameSceneUI();

private:
	CText* mpText;	// 文字列描画用
};