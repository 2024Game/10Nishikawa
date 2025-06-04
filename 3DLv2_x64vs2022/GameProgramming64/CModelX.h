#ifndef CMODELX_H
#define CMODELX_H

#include <vector>	//vectorクラスのインクルード（動的配列）
#include "CMatrix.h"	//マトリクスクラスのインクルード
#include "CVector.h"
//#include "CMaterial.h"
class CModelX;		// CModelXクラスの宣言
class CModelXFrame;	// CModelXFrameクラスの宣言
class CMaterial;	//マテリアルの宣言
class CSkinWeights; //スキンウェイトクラス
class CAnimationSet; //アニメーションセットクラス
class CAnimation; //アニメーションクラス
class CAnimationKey;  //アニメーションキークラス

#define MODEL_FILE "res\\sample.blend.x"  //入力ファイル名
//領域開放をマクロ化
#define SAFE_DELETE_ARRAY(a) {if(a) delete[] a; a = nullptr;}
//配列のサイズ取得をマクロ化
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
/*
CModelX
Xファイル形式の3Dモデルデータをプログラムで認識する
*/

class CModelX
{
	friend CModelXFrame;
	friend CAnimationSet;
	friend CAnimation;
public:
	CModelX();
	~CModelX();
	void Render();
	//ノードの読み飛ばし
	void SkipNode();
	//単語の取り出し
	char* GetToken();
	char* Token();
	void Load(const char* file);
	bool EOT(); // トークンが無くなったらtrue
	//フレーム名に該当するフレームのアドレスを返す
	CModelXFrame* FindFrame(char* name);
	std::vector<CAnimationSet*>& AnimationSet();
	void AnimateFrame();
	std::vector<CModelXFrame*>& Frames();
	//スキンウェイトのフレーム番号設定
	void SetSkinWeightFrameIndex();
	//頂点にアニメーションを適用
	void AnimateVertex();
private:
	//アニメーションセットの配列
	std::vector<CAnimationSet*> mAnimationSet;
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
	void Render();
	//スキンウェイトにフレーム番号を設定する
	void SetSkinWeightFrameIndex(CModelX* model);
	//頂点にアニメーション適用
	void AnimateVertex(CModelX* model);
private:
	//スキンウェイト
	std::vector<CSkinWeights*> mSkinWeights;

	int mMaterialNum;	//マテリアル数
	int mMaterialIndexNum;//マテリアル番号数（面数）
	int* mpMaterialIndex;	  //マテリアル番号
	std::vector<CMaterial*> mMaterial;//マテリアルデータ

	int mNormalNum;	//法線数
	CVector* mpNormal;//法線ベクトル
	int mFaceNum;	//面数
	int* mpVertexIndex;	//面を構成する頂点インデックス
	int mVertexNum;	//頂点数
	CVector* mpVertex;	//頂点データ
	CVector* mpAnimateVertex;  //アニメーション用頂点
	CVector* mpAnimateNormal;  //アニメーション用法線
};

class CModelXFrame
{
	friend CModelX;
	friend CAnimation;
	friend CAnimationSet;
public:
	//コンストラクタ
	CModelXFrame(CModelX* model);
	//デストラクタ
	~CModelXFrame();
	void Render();
	int Index();
	//合成行列の作成
	void AnimateCombined(CMatrix* parent);
	const CMatrix& CombinedMatrix();
private:
	CMesh* mpMesh;	//Meshデータ
	std::vector<CModelXFrame*> mChild;  //子フレームの配列
	CMatrix mTransformMatrix;  //変換行列
	char* mpName;   //フレーム名前
	int mIndex;  //フレーム番号
	CMatrix mCombinedMatrix;	//合成行列
};

/*
 CSkinWeights
 スキンウェイトクラス
*/
class CSkinWeights
{
	friend CModelX;
	friend CMesh;
public:
	CSkinWeights(CModelX* model);
	~CSkinWeights();
	const int& FrameIndex();
	const CMatrix& Offset();
private:
	char* mpFrameName;	//フレーム名
	int mFrameIndex;	//フレーム番号
	int mIndexNum;		//頂点番号数
	int* mpIndex;		//頂点番号配列
	float* mpWeight;	//頂点ウェイト配列
	CMatrix mOffset;	//オフセットマトリックス
};

/*
 CAnimationSet
 アニメーションセット
*/
class CAnimationSet
{
	friend CModelX;
public:
	CAnimationSet(CModelX* model);
	~CAnimationSet();
	void Time(float time);		//時間の設定
	void Weight(float weight);  //重みの設定

	void AnimateMatrix(CModelX* model);
	std::vector<CAnimation*>& Animation();

	// --- Geter ---
	// mTimeを返す
	float Time();
	// mMaxTimeを返す
	float MaxTime();

private:
	//アニメーションセット名
	char* mpName;
	//アニメーション
	std::vector<CAnimation*> mAnimation;
	float mTime;		//現在時間
	float mWeight;		//重み
	float mMaxTime;		//最大時間
};

/*
 CAnimation
 アニメーションクラス
*/
class CAnimation
{
	friend CAnimationSet;
	friend CModelX;
public:
	CAnimation(CModelX* model);
	~CAnimation();
private:
	char* mpFrameName;	//フレーム名
	int mFrameIndex;	//フレーム番号
	int mKeyNum;		//キー数（時間数）
	CAnimationKey* mpKey;	//キーの配列
};

/*
 CAnimationKey
 アニメーションキークラス
*/
class CAnimationKey
{
	friend CAnimation;
	friend CAnimationSet;
private:
	//時間
	float mTime;
	//行列
	CMatrix mMatrix;
};

#endif // !CMODELX_H