#pragma once
#include "CTask.h"

class CFish;

// 魚管理クラス
class CFishManager : public CTask
{
public:
	// CFishManagerのインスタンスを返す
	static CFishManager* Instance();

	// 魚のリストを取得
	/*
		msInstanceは4バイト以上有るので、&(アンパサンド)をつけて参照渡しにすることで
		4バイトのデータのやり取りで済むので、高速化できる。
		ただし、元のデータを直接操作できるのでconst(読み取り専用)をつける。
	*/
	const std::vector<CFish*>& GetFishes() const;

	void Add(CFish* fish);
	void Remove(CFish* fish);

private:
	CFishManager();
	~CFishManager();

	static CFishManager* msInstance;

	std::vector<CFish*> mFishes;	// 魚のリスト
};