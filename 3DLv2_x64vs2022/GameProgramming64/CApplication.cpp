#include "CApplication.h"
#include "CMatrix.h"
#include "CTransform.h"
#include "CCollisionManager.h"
#include "CBillBoard.h"
#include "CEnemy3.h"


#define SOUND_BGM "res\\mario.wav" //BGM音声ファイル
#define SOUND_OVER "res\\mdai.wav" //ゲームオーバー音声ファイル

//モデルデータの指定
#define MODEL_OBJ "res\\f14.obj", "res\\f14.mtl"
//背景モデルデータの指定
#define MODEL_BACKGROUND  "res\\sky.obj", "res\\sky.mtl"
//敵輸送機モデル
#define MODEL_C5 "res\\c5.obj", "res\\c5.mtl"

CUi* CApplication::spUi = nullptr;

CApplication::~CApplication()
{
	delete spUi;	//インスタンスUiの削除
}

CUi* CApplication::Ui()
{
	return spUi;	//インスタンスのポインタを返す
}

CTexture CApplication::mTexture;

CTexture* CApplication::Texture()
{
	return &mTexture;
}

CMatrix CApplication::mModelViewInverse;

const CMatrix& CApplication::ModelViewInverse()
{
	return mModelViewInverse;
}

void CApplication::Start()
{
	//3Dモデルファイルの読み込み
	mModelX.Load(MODEL_FILE);

	//キャラクターにモデルを設定
	mXPlayer.Init(&mModelX);

	mFont.Load("FontG.png", 1, 4096 / 64);

	anim = 1;

	mKnight.Load("res\\knight\\knight_low.x");
	//敵の初期設定
	//mXEnemy.Init(&mModelX);
	mXEnemy.Init(&mKnight);
	//敵の配置
	mXEnemy.Position(CVector(7.0f, 0.0f, 0.0f));

}

void CApplication::Update()
{
	//歩くアニメーションに切り替える
	/*
	アニメーションが終了したら、一つ大きいアニメーション番号で、
	キャラクタクラスのChangeAnimationメソッドを呼び出します。
	*/
	/*
	mCharacter.ChangeAnimation(anim, true, 60);
	
	if (mCharacter.IsAnimationFinished())
	{
		printf("Index:%i\n",mCharacter.AnimationIndex());
		anim++;
		if (anim == 12)
		{
			anim = 1;
		}
	}
	*/
	
	//キャラクタークラスの更新
	mXPlayer.Update();
	//敵の更新
	mXEnemy.Update();

	/*最初のアニメーションの現在時間を45にする
	mModelX.AnimationSet()[0]->Time(mModelX.AnimationSet()[0]->Time() + 1.0f);
	mModelX.AnimationSet()[0]->Time(
		(int)mModelX.AnimationSet()[0]->Time() %
		(int)(mModelX.AnimationSet()[0]->MaxTime() + 1)); 
	*/
	//mModelX.AnimationSet()[0]->Time();

	//最初のアニメーションの重みを1.0（100%)にする
	//mModelX.AnimationSet()[0]->Weight(1.0f);
	//フレームの変換行列をアニメーションで更新する
	//mModelX.AnimateFrame();
	//フレームの合成行列を計算する
	//mModelX.Frames()[0]->AnimateCombined(&mMatrix);
	
	//カメラのパラメータを作成する
	CVector e, c, u;//視点、注視点、上方向
	//視点を求める
	e = CVector(1.0f, 2.0f, 10.0f);
	//注視点を求める
	c = CVector();
	//上方向を求める
	u = CVector(0.0f, 1.0f, 0.0f);
	//カメラの設定
	gluLookAt(e.X(), e.Y(), e.Z(), c.X(), c.Y(), c.Z(), u.X(), u.Y(), u.Z());
	//モデルビュー行列の取得
	glGetFloatv(GL_MODELVIEW_MATRIX, mModelViewInverse.M());
	//逆行列の取得
	mModelViewInverse = mModelViewInverse.Transpose();
	mModelViewInverse.M(0, 3, 0);
	mModelViewInverse.M(1, 3, 0);
	mModelViewInverse.M(2, 3, 0);

	//X軸＋回転
	if (mInput.Key('K'))
	{
		mMatrix = mMatrix * CMatrix().RotateX(1);
	}
	//X軸-回転
	if (mInput.Key('I'))
	{
		mMatrix = mMatrix * CMatrix().RotateX(-1);
	}
	//Y軸＋回転
	if (mInput.Key('L'))
	{
		mMatrix = mMatrix * CMatrix().RotateY(1);
	}
	//Y軸-回転
	if (mInput.Key('J'))
	{
		mMatrix = mMatrix * CMatrix().RotateY(-1);
	}
	//行列設定
	//glMultMatrixf(mMatrix.M());

	//頂点にアニメーションを適用する
	//mModelX.AnimateVertex();

	//モデル描画
	//mModelX.Render();
	mXPlayer.Render();
	//敵描画
	mXEnemy.Render();

	//コライダの描画
	CCollisionManager::Instance()->Render();

	//衝突処理
	CCollisionManager::Instance()->Collision();

	//2D描画開始
	CCamera::Start(0, 800, 0, 600);

	mFont.Draw(20, 20, 10, 12, "3D PROGRAMMING");

	//2Dの描画終了
	CCamera::End();
}