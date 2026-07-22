#pragma once
#include <string>
#include <vector>
#include <map>

// Enemy status data loaded from CSV
struct EnemyStatusData
{
	float maxHp;
	float maxSt;
	float gainSt;
	float attackCost1;
	float attackCost2;
	float attackCost3;
	float avoidCost;
	float stepMag;
	float attackMag;
	float atSpeedMag;
	float negTime;
	float negProb;
	bool  can1B;
};

// CSV loader for enemy status data
class CEnemyStatusLoader
{
public:
	// Load a CSV file for a specific enemy type
	static bool Load(const std::string& filePath);

	// Get status data for a specific enemy name and level
	// Returns nullptr if not found
	static const EnemyStatusData* GetStatus(
		const std::string& enemyName, int level);

private:
	// enemyName -> (level -> data)
	static std::map<std::string,
		std::map<int, EnemyStatusData>> sStatusTable;
};
