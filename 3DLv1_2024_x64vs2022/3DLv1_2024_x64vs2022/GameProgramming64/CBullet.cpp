#include "CBullet.h"

#define BVELOCITY CVector (0.0f, 0.0f, 0.3f)


//幅と奥行きの設定
//Set(幅, 奥行)
void CBullet::Set(float w, float d)
{
	//スケール設定
	mScale = CVector(1.0f, 1.0f, 1.0f);
	//三角形の頂点設定→ヒント 3
	mT.Vertex (CVector(w, 0.0f, 0.0f), CVector(-w, 0.0f, 0.0f), CVector(0.0f, 0.0f, d));
	//三角形の法線設定
	mT.Normal(CVector(0.0f, 1.0f, 0.0f));
}

//更新
void CBullet::Update()
{
	//位置更新　進行方向へ１進む→ヒント 17
	mPosition = mPosition + BVELOCITY * mMatrixRotate;
	CTransform::Update();
}

//描画
void CBullet::Render()
{
	//DIFFUSE黄色設定
	float c[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, c);
	//三角形描画→ヒント 12
	mT.Render(mMatrix);
}

