#pragma once
#include "CObjectBase.h"
#include "CModelX.h"

// プレイヤーの残像（ゴースト）を表現するクラス
class CPlayerAfterImage : public CObjectBase
{
public:
	// 引数: モデル、ボーン行列配列の先頭ポインタ、ボーン数、残像の生存時間
	CPlayerAfterImage(CModelX* model, const CMatrix* combinedMatrix, int boneCount, float lifeTime = 0.3f);
	virtual ~CPlayerAfterImage();

	// 毎フレームの更新（透明度の減衰と、寿命が切れた時の消滅処理）
	void Update() override;

	// 描画処理（コピーしたボーン姿勢で半透明描画する）
	void Render() override;

private:
	CModelX* mpModel;             // プレイヤーのモデルへの参照
	CMatrix* mpCombinedMatrixCopy; // コピーしたボーンの姿勢行列
	int mBoneCount;               // ボーン数
	float mAlpha;                 // 現在の透明度 (0.0f ～ 1.0f)
	float mLifeTime;              // 残りの生存時間
	float mMaxLifeTime;           // 最大生存時間
};