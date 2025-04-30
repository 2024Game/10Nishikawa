#ifndef CMODELX_H
#define CMODELX_H

#include <vector>	//vectorクラスのインクルード（動的配列）
#include "CMatrix.h"	//マトリクスクラスのインクルード
#include "CVector.h"
class CModelX;		// CModelXクラスの宣言
class CModelXFrame;	// CModelXFrameクラスの宣言

#define MODEL_FILE "res\\sample.blend.x"  //入力ファイル名
//領域開放をマクロ化
#define SAFE_DELETE_ARRAY(a) {if(a) delete[] a; a = nullptr;}
/*
CModelX
Xファイル形式の3Dモデルデータをプログラムで認識する
*/

class CModelX
{
	friend CModelXFrame;
public:
	CModelX();
	~CModelX();
	//ノードの読み飛ばし
	void SkipNode();
	//単語の取り出し
	char* GetToken();
	char* Token();
	void Load(const char* file);

private:
	std::vector<CModelXFrame*> mFrame;  //フレームの配列
	char* mpPointer;
	char mToken[1024];
	//cが区切り文字ならtrueを返す
	bool IsDelimiter(char c);
};


//CMeshクラスの定義
class CMesh
{
public:
	//コンストラクタ
	CMesh();
	//デストラクタ
	~CMesh();
	//読み込み処理
	void Init(CModelX* model);
private:
	int mVertexNum;	//頂点数
	CVector* mpVertex;	//頂点データ
};



class CModelXFrame
{
	friend CModelX;
public:
	//コンストラクタ
	CModelXFrame(CModelX* model);
	//デストラクタ
	~CModelXFrame();
private:
	CMesh* mpMesh;	//Meshデータ
	std::vector<CModelXFrame*> mChild;  //子フレームの配列
	CMatrix mTransformMatrix;  //変換行列
	char* mpName;   //フレーム名前
	int mIndex;  //フレーム番号
};

#endif // !CMODELX_H

