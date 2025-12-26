#pragma once
#include "CSceneBase.h"
#include <memory>
#include "CPlayer.h"
#include "CBoat.h"
class CGameMenu;
class CSound;
class CHomeSceneUI;

//タイトルシーン
class CHomeScene :public CSceneBase
{
public:
	//コンストラクタ
	CHomeScene();
	//デストラクタ
	~CHomeScene();
	//シーン読み込み
	void Load();
	//シーンの更新処理
	void Update();

private:
	CGameMenu* mpGameMenu;
	CSaveManager* mpSaveManager;
	CPlayer* mpPlayer;
	CSound* mpWhistleSE;
	CHomeSceneUI* mpCHomeSceneUI;
	bool mInGame;
};