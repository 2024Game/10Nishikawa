#include "CFishManager.h"
#include "CFish.h"

// staticメンバ変数は定義しないといけないのでnullptrで初期化
CFishManager* CFishManager::msInstance = nullptr;

CFishManager::CFishManager()
	:CTask(ETaskPriority::eManager)
{
}

CFishManager::~CFishManager()
{
	msInstance = nullptr;
}

CFishManager* CFishManager::Instance()
{
	// 仮のシングルトンパターン(コピーは作れてしまう)
	if (msInstance == nullptr)
	{
		msInstance = new CFishManager();
	}
	return msInstance;
}

const std::vector<CFish*>& CFishManager::GetFishes() const
{
	return mFishes;
}

void CFishManager::Add(CFish* fish)
{
	mFishes.push_back(fish);
}

void CFishManager::Remove(CFish* fish)
{
	/*
		std::vector<int> v = {1, 2, 3, 2, 4};

		v.begin() から v.end() までを走査。
		2と一致する要素を見つけたら、それをスキップして、前方に詰める。
		auto it = std::remove(v.begin(), v.end(), 2);

		v = {1, 3, 4, ?, ?} ← 2は前方に詰められて消えたように見えるが、サイズは変わっていない
		v.erase(it, v.end()); // ← ここで本当に削除
	*/
	auto result = std::remove(mFishes.begin(), mFishes.end(), fish);
	mFishes.erase(result, mFishes.end());
}
