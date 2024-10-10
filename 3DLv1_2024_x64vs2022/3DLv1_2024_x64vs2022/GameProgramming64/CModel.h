#pragma once
#ifndef CMODEL_H
#define CMODEL_H

#include "CMaterial.h"
//vectorのインクルード
#include <vector>
#include "CTriangle.h"

/*
モデルクラス
モデルデータの入力や表示
*/
class CModel 
{
public:
	//モデルファイルの入力
	//Load(モデルファイル名, マテリアルファイル名)
	void Load(const char* obj, const char* mtl);

	//描画
	void Render();

	~CModel();

private:
	//三角形の可変長配列
	std::vector<CTriangle> mTriangles;

	//マテリアルポインタの可変長配列
	std::vector<CMaterial*> mpMaterials;

};

#endif
