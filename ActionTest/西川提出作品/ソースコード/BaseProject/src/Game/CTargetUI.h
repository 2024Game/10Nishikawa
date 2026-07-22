#pragma once
#include "CUIBase.h"

class CImage;

class CTargetUI : public CUIBase
{
public:
	CTargetUI();
	~CTargetUI();
	void SetTargetPos(const CVector& pos);
	void Update() override;
	void Render() override;

private:
	CImage* mpTargetImg;
	CVector2 mImageBaseSize;
	CVector mTargetPos;
};