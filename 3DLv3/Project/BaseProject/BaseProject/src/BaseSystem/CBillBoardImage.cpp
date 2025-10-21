#include "CBillboardImage.h"

// コンストラクタ
CBillboardImage::CBillboardImage(std::string path, ETag tag, ETaskPauseType pause)
	: CImage3D(path, tag, ETaskPriority::eBillboard, 0, pause)
{
	// ビルボードをオン
	SetBillboard(true);
}

// デストラクタ
CBillboardImage::~CBillboardImage()
{
}
