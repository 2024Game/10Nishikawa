#include "CMatrix.h"
//標準入出力関数のインクルード
#include <stdio.h>
// 円周率M_PIを有効にする
#define _USE_MATH_DEFINES
//数学関数のインクルード
#include <math.h>

CMatrix CMatrix::Transpose() const
{
	CMatrix tmp; //返却用のCMatrixインスタンスを作成

	//tmpの２次元配列に、インスタンスの２次元配列を代入
	tmp.mM[0][0] = mM[0][0];
	tmp.mM[0][1] = mM[1][0];
	tmp.mM[0][2] = mM[2][0];
	tmp.mM[0][3] = mM[3][0];
	tmp.mM[1][0] = mM[0][1];
	tmp.mM[1][1] = mM[1][1];
	tmp.mM[1][2] = mM[2][1];
	tmp.mM[1][3] = mM[3][1];
	tmp.mM[2][0] = mM[0][2];
	tmp.mM[2][1] = mM[1][2];
	tmp.mM[2][2] = mM[2][2];
	tmp.mM[2][3] = mM[3][2];
	tmp.mM[3][0] = mM[0][3];
	tmp.mM[3][1] = mM[1][3];
	tmp.mM[3][2] = mM[2][3];
	tmp.mM[3][3] = mM[3][3];

	return tmp; //代入されたtmpを返す
}

float* CMatrix::M() const
{
	return (float*)mM[0];
}

//*演算子のオーバーロード
//CMatrix * CMatrix の演算結果を返す
const CMatrix CMatrix::operator*(const CMatrix& m) const
{
	CMatrix t;
	int s;
	for (s = 0; s <= 3; s++)
	{
		t.mM[s][0] = mM[s][0] * m.mM[0][0] + mM[s][1] * m.mM[1][0] + mM[s][2] * m.mM[2][0] + mM[s][3] * m.mM[3][0];
		t.mM[s][1] = mM[s][0] * m.mM[0][1] + mM[s][1] * m.mM[1][1] + mM[s][2] * m.mM[2][1] + mM[s][3] * m.mM[3][1];
		t.mM[s][2] = mM[s][0] * m.mM[0][2] + mM[s][1] * m.mM[1][2] + mM[s][2] * m.mM[2][2] + mM[s][3] * m.mM[3][2];
		t.mM[s][3] = mM[s][0] * m.mM[0][3] + mM[s][1] * m.mM[1][3] + mM[s][2] * m.mM[2][3] + mM[s][3] * m.mM[3][3];
	}
	return t;
}

void CMatrix::M(int row, int col, float value)
{
	mM[row][col] = value;
}

//移動行列の作成
//Translate(移動量X, 移動量Y, 移動量Z)
CMatrix CMatrix::Translate(float mx, float my, float mz)
{
	Identity(); //初期化
	mM[3][0] = mx;
	mM[3][1] = my;
	mM[3][2] = mz;
	//この行列を返す
	return *this;
}


//回転行列（X軸）の作成
//RotateY(角度)
CMatrix CMatrix::RotateX(float degree)
{
	//角度からラジアンを求める
	float rad = degree / 180.0f * M_PI;
	//単位行列にする
	Identity();

	//Y軸で回転する行列の設定
	mM[1][1] = mM[2][2] = cosf(rad);
	mM[1][2] = sinf(rad);
	mM[2][1] = -mM[1][2];
	//行列を返す
	return *this;
}

//回転行列（Y軸）の作成
//RotateY(角度)
CMatrix CMatrix::RotateY(float degree)
{
	//角度からラジアンを求める
	float rad = degree / 180.0f * M_PI;
	//単位行列にする
	Identity();

	//Y軸で回転する行列の設定
	mM[0][0] = mM[2][2] = cosf(rad);
	mM[0][2] = -sinf(rad);
	mM[2][0] = -mM[0][2];
	//行列を返す
	return *this;
}

//回転行列（Z軸）の作成
//RotateZ(角度)
CMatrix CMatrix::RotateZ(float degree)
{
	//角度からラジアンを求める
	float rad = degree / 180.0f * M_PI;
	//単位行列にする
	Identity();
	//Z軸で回転する行列の設定
	mM[0][0] = mM[1][1] = cosf(rad);
	mM[0][1] = sinf(rad);
	mM[1][0] = -mM[0][1];
	//行列を返す
	return *this;
}

float CMatrix::M(int r, int c) const
{
	return mM[r][c];
}

//拡大縮小行列の作成
//Scale(倍率X, 倍率Y, 倍率Z)
CMatrix CMatrix::Scale(float sx, float sy, float sz)
{
	Identity(); //初期化
	mM[0][0] = sx, mM[1][1] = sy, mM[2][2] = sz;

	//この行列を返す
	return *this;
}

void CMatrix::Print()
{
	printf("%10f %10f %10f %10f\n",
		mM[0][0], mM[0][1], mM[0][2], mM[0][3]);
	printf("%10f %10f %10f %10f\n",
		mM[1][0], mM[1][1], mM[1][2], mM[1][3]);
	printf("%10f %10f %10f %10f\n",
		mM[2][0], mM[2][1], mM[2][2], mM[2][3]);
	printf("%10f %10f %10f %10f\n",
		mM[3][0], mM[3][1], mM[3][2], mM[3][3]);
}

//デフォルトコンストラクタ
CMatrix::CMatrix()
{
	Identity();
}

//単位行列の作成
//初期化にも使える、、、、たぶん
CMatrix CMatrix::Identity()
{
	mM[0][0] = 1.0f, mM[0][1] = 0.0f, mM[0][2] = 0.0f, mM[0][3] = 0.0f;
	mM[1][0] = 0.0f, mM[1][1] = 1.0f, mM[1][2] = 0.0f, mM[1][3] = 0.0f;
	mM[2][0] = 0.0f, mM[2][1] = 0.0f, mM[2][2] = 1.0f, mM[2][3] = 0.0f;
	mM[3][0] = 0.0f, mM[3][1] = 0.0f, mM[3][2] = 0.0f, mM[3][3] = 1.0f;
	//この行列を返す
	return *this;
}
