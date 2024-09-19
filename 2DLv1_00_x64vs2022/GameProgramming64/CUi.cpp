#include "CUi.h"
#include <stdio.h>


CUi::CUi()
	: mTime(0)
	, mHp(0)
{	//フォントファイルのロード
	mFont.Load("FontWhite.png", 1, 64);
}

void CUi::Clear()
{
	mFont.Draw(36, 300, 18, 36, "GAME CLEAR!!");
	mFont.Draw(36, 200, 18, 36, "PUSH ENTER KEY!");
}

void CUi::Over()
{
	mFont.Draw(36, 300, 18, 36, "GAME OVER!!");
	mFont.Draw(36, 200, 18, 36, "PUSH ENTER KEY!");
}

void CUi::Start()
{
	mFont.Draw(36, 300, 18, 36, "START PUSH ENTER KEY!");
}

void CUi::Enemy(int enemy)
{
	mEnemy = enemy;
}

void CUi::Hp(int hp)
{
	mHp = hp;
}

void CUi::Time(int time)
{
	mTime = time;
}

void CUi::Render()
{
	char str[16];
	sprintf(str, "TIME:%03d", mTime);
	mFont.Draw(620, 580, 10, 20, str);

	char uHp[16];
	sprintf(uHp, "HP:%01d", mHp);
	mFont.Draw(20, 20, 10, 20, uHp);

	//char str[16];
	//sprintf(str, "TIME:%03d", mTime);
	//mFont.Draw(620, 580, 10, 20, str);
	//sprintf(str, "HP:%01d", mHp);
	//mFont.Draw(20, 20, 10, 20, str);
	//でも可能
	// 
	//{
		//char str[16];
		//sprintf(str, "HP:%01d", mHp);
		//mFont.Draw(20, 20, 10, 20, str);
	//}でも可能

	char uEne[16];
	sprintf(uEne, "ENEMY:%01d", mEnemy);
	mFont.Draw(20, 580, 10, 20, uEne);
}
