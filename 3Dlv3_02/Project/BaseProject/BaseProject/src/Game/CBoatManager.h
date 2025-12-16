#pragma once
#include "CTask.h"

class CBoat;

// 船管理クラス
class CBoatManager : public CTask
{
public:
	// CBoatManagerのインスタンスを返す
	static CBoatManager* Instance();

	// 魚のリストを取得
	/*
		msInstanceは4バイト以上有るので、&(アンパサンド)をつけて参照渡しにすることで
		4バイトのデータのやり取りで済むので、高速化できる。
		ただし、元のデータを直接操作できるのでconst(読み取り専用)をつける。
	*/
	const std::vector<CBoat*>& GetBoats() const;

	void Add(CBoat* boat);
	void Remove(CBoat* boat);

private:
	CBoatManager();
	~CBoatManager();

	static CBoatManager* msInstance;

	std::vector<CBoat*> mBoats;	// 船のリスト
};