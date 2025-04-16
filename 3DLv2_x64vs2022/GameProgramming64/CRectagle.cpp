
#include "CRectangle.h"
#include <stdio.h>
#include "glut.h"

bool CRectangle::Collision(CRectangle* r, float* ax, float* ay)
{
	if (mX < r->mX)
		*ax = r->mX - mX - mW - r->mW;
	else
		*ax = mX - r->mX - mW - r->mW;
	//0ˆÈã‚ÍÕ“Ë‚µ‚È‚¢
	if (*ax >= 0.0f)
		return false;

	if (mY < r->mY)
		*ay = r->mY - mY - mH - r->mH;
	else
		*ay = mY - r->mY - mH - r->mH;
	//0ˆÈã‚ÍÕ“Ë‚µ‚È‚¢
	if (*ay >= 0.0f)
		return false;

	//Y‚ª’Z‚¢‚©”»’è
	if (*ax < *ay)
	{//YC³AX‚Í0
		*ax = 0.0f;
		//ã‚ÌŽž
		if (mY > r->mY)
			*ay = -*ay;
	}
	else
	{//XC³AY‚Í0
		*ay = 0.0f;
		//‰E‚ÌŽž
		if (mX > r->mX)
			*ax = -*ax;
	}
	return true;
}

CRectangle::CRectangle()
{
	printf("ƒCƒ“ƒXƒ^ƒ“ƒX‚ª¶‚Ü‚ê‚Ü‚µ‚½\n");
}

CRectangle::~CRectangle()
{
}

void CRectangle::Render()
{
	glBegin(GL_QUADS);
	glVertex2f(mX - mW, mY - mH);
	glVertex2f(mX + mW, mY - mH);
	glVertex2f(mX + mW, mY + mH);
	glVertex2f(mX - mW, mY + mH);
	glEnd();
}

void CRectangle::Render(float x, float y, float w, float h)
{
	glBegin(GL_QUADS);
	glVertex2f(x - w, y - h);
	glVertex2f(x + w, y - h);
	glVertex2f(x + w, y + h);
	glVertex2f(x - w, y + h);
	glEnd();
}

void CRectangle::Set(float x, float y, float w, float h)
{
	mX = x;
	mY = y;
	mW = w;
	mH = h;
}

float CRectangle::X()
{
	return mX;
}

float CRectangle::Y()
{
	return mY;
}

float CRectangle::W()
{
	return mW;
}

float CRectangle::H()
{
	return mH;
}

void CRectangle::Y(float y)
{
	mY = y;
}
void CRectangle::X(float x)
{
	mX = x;
}

bool CRectangle::Collision(CRectangle* r)
{
	float distX = mX - r->mX;
	if (distX < 0.0f)
		distX = -distX;
	if (distX >= mW + r->mW)
		return false;
	//‰Û‘è‚P‚R
	float distY = mY - r->mY;
	if (distY < 0.0f)
		distY = -distY;
	if (distY >= mH + r->mH)
		return false;





	return true;
}