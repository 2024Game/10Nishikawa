#pragma once
#ifndef CMATERIAL_H
#define CMATERIAL_H
#define MATERIAL_NAME_LEN 64 //名前の長さ

#include "CTexture.h"

/*
マテリアルクラス
マテリアルのデータを扱う
*/
class CMaterial
{
public:
	//テクスチャの取得
	CTexture* Texture();
	//デフォルトコンストラクタ
	CMaterial();
	//マテリアルを有効にする
	void Enabled();
	//マテリアルを無効にする
	void Disabled();
	//マテリアルの名前の取得
	char* Name();
	//マテリアルの名前を設定する
	//Name(マテリアルの名前)
	void Name(char* name);
	//mDiffuse配列の取得
	float* Diffuse();
private:
	//マテリアル名
	char mName[MATERIAL_NAME_LEN + 1];
	//拡散光の色RGBA
	float mDiffuse[4];
	//テクスチャ
	CTexture mTexture;
};

#endif
