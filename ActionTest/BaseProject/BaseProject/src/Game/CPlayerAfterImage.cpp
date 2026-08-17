#include "CPlayerAfterImage.h"
#include "Times.h"
#include <glut.h>

CPlayerAfterImage::CPlayerAfterImage(CModelX* model, const CMatrix* combinedMatrix, int boneCount, float lifeTime)
	: CObjectBase(ETag::eNone, ETaskPriority::eEffect, 0, ETaskPauseType::eGame)
	, mpModel(model)
	, mBoneCount(boneCount)
	, mLifeTime(lifeTime)
	, mMaxLifeTime(lifeTime)
	, mAlpha(0.8f) // 出現時の不透明度
{
	// ボーン行列のコピー用メモリを確保し、現在の姿勢を保存する
	mpCombinedMatrixCopy = new CMatrix[boneCount];
	memcpy(mpCombinedMatrixCopy, combinedMatrix, sizeof(CMatrix) * boneCount);

	// タスクの更新と描画を有効にする
	SetShow(true);
	SetEnable(true);
}

CPlayerAfterImage::~CPlayerAfterImage()
{
	// メモリの解放
	delete[] mpCombinedMatrixCopy;
}

void CPlayerAfterImage::Update()
{
	mLifeTime -= Times::UnscaledDeltaTime();
	if (mLifeTime <= 0.0f)
	{
		Kill(); // 生存時間が切れたらタスクマネージャーから削除して自動破棄
		return;
	}

	// 時間の経過に合わせて徐々に透明にする
	mAlpha = (mLifeTime / mMaxLifeTime) * 0.95f;
}

void CPlayerAfterImage::Render()
{
	if (mpModel && mpCombinedMatrixCopy)
	{
		CColor col = CColor::white;
		col.A(mAlpha);
		
		glDepthMask(GL_FALSE); // 描画前にデプスマスクをOFFにする
		mpModel->SetColor(col);
		mpModel->RenderShader(mpCombinedMatrixCopy);
		mpModel->SetColor(CColor::white);
		
		glDepthMask(GL_TRUE);  // 描画後にONに戻す
	}
}
