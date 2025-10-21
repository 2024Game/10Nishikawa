#pragma once
#include "CImage3D.h"

// 常にカメラの方向を向くイメージ
class CBillboardImage : public CImage3D
{
public:
	// コンストラクタ
	CBillboardImage(std::string path,
		ETag tag = ETag::eNone,
		ETaskPauseType pause = ETaskPauseType::eDefault);
	// デストラクタ
	virtual ~CBillboardImage();

private:
};