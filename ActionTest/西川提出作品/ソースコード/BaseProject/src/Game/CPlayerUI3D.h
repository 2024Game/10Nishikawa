#pragma once
#include "CObjectBase.h"

class CImage3D;

// 3D空間に配置するイメージクラス
class CPlayerUI3D : public CObjectBase
{
public:
	// コンストラクタ
	CPlayerUI3D(CObjectBase* owner);
	// デストラクタ
	virtual ~CPlayerUI3D();

	// 持ち主を設定
	void SetOwner(CObjectBase* owner);

	// 表示のON/OFFを設定
	void SetShow(bool show);

	// 更新
	void Update() override;
	// 描画
	void Render() override;

private:
	CObjectBase* mpOwner;	// インジケーターの持ち主
	CImage3D* mpWhiteImg;	// 白イメージ
	CVector2 mImageSize;	// ゲージのイメージのサイズ
	CColor mBarColor;

	bool mIsShow;
};