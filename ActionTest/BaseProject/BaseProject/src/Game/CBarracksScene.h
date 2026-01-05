#pragma once
#include "CSceneBase.h"
#include <memory>
#include "CPlayer.h"
#include "CSaveManager.h"

class CGameMenu;
class CSound;
class CBarracksSceneUI;

//タイトルシーン
class CBarracksScene :public CSceneBase
{
public:
	//コンストラクタ
	CBarracksScene();
	//デストラクタ
	~CBarracksScene();
	//シーン読み込み
	void Load();
	//シーンの更新処理
	void Update();

private:
	CGameMenu* mpGameMenu;
	CSaveManager* mpSaveManager;
	CPlayer* mpPlayer;
	CSound* mpWhistleSE;
	CBarracksSceneUI* mpCBarracksSceneUI;
	bool mInGame;
};