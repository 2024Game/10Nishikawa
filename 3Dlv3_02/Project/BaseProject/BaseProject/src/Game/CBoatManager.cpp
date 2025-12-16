#include "CBoatManager.h"
#include "CBoat.h"

// staticメンバ変数は定義しないといけないのでnullptrで初期化
CBoatManager* CBoatManager::msInstance = nullptr;

CBoatManager::CBoatManager()
	:CTask(ETaskPriority::eManager)
{
}

CBoatManager::~CBoatManager()
{
	msInstance = nullptr;
}

CBoatManager* CBoatManager::Instance()
{
	// 仮のシングルトンパターン(コピーは作れてしまう)
	if (msInstance == nullptr)
	{
		msInstance = new CBoatManager();
	}
	return msInstance;
}

const std::vector<CBoat*>& CBoatManager::GetBoats() const
{
	return mBoats;
}

void CBoatManager::Add(CBoat* boat)
{
	mBoats.push_back(boat);
}

void CBoatManager::Remove(CBoat* boat)
{
	/*
		std::vector<int> v = {1, 2, 3, 2, 4};

		v.begin() から v.end() までを走査。
		2と一致する要素を見つけたら、それをスキップして、前方に詰める。
		auto it = std::remove(v.begin(), v.end(), 2);

		v = {1, 3, 4, ?, ?} ← 2は前方に詰められて消えたように見えるが、サイズは変わっていない
		v.erase(it, v.end()); // ← ここで本当に削除
	*/
	auto result = std::remove(mBoats.begin(), mBoats.end(), boat);
	mBoats.erase(result, mBoats.end());
}
