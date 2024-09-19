#pragma once
class CRectangle
{
public:
	//bool Collision(四角形のポインタ, X調整ポインタ,Y調整ポインタ)
	//戻り値
	//true:衝突している
	//false;衝突していない
	bool Collision(CRectangle* pRect, float* ax, float* ay);

	CRectangle();
	~CRectangle();
	void Render();
	void Render(float x, float y, float w, float h);
	void Set(float x, float y, float w, float h);
	float X();
	float Y();
	float W();
	float H();
	void X(float x);
	void Y(float y);
	//bool Collision(四角形のポインタ)
	//戻り値
	//true:衝突している
	//false;衝突していない
	bool Collision(CRectangle* pRect);

private:
	float mX; //X座標
	float mY; //Y座標
	float mW; //幅
	float mH; //高さ
};