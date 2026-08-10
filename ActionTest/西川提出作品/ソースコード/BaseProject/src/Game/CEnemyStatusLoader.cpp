#include "CEnemyStatusLoader.h"
#include "Define.h"
#include <fstream>
#include <sstream>

// static member initialization
std::map<std::string, std::map<int, EnemyStatusData>>
	CEnemyStatusLoader::sStatusTable;

bool CEnemyStatusLoader::Load(const std::string& filePath)
{
	// Construct the full path using RES_DIR
	std::string fullPath = RES_DIR;
	fullPath += filePath;

	std::ifstream file(fullPath);
	if (!file.is_open()) return false;

	// Extract enemy name from file path
	// e.g. "EnemyStatus\\Soldier.csv" -> "Soldier"
	std::string enemyName = filePath;
	size_t slashPos = enemyName.find_last_of("\\/");
	if (slashPos != std::string::npos)
	{
		enemyName = enemyName.substr(slashPos + 1);
	}
	size_t dotPos = enemyName.find('.');
	if (dotPos != std::string::npos)
	{
		enemyName = enemyName.substr(0, dotPos);
	}

	std::string line;
	// Skip header line
	std::getline(file, line);

	while (std::getline(file, line))
	{
		// Skip empty lines
		if (line.empty()) continue;

		// Remove trailing \r if present
		if (!line.empty() && line.back() == '\r')
		{
			line.pop_back();
		}
		if (line.empty()) continue;

		std::stringstream ss(line);
		std::string token;
		EnemyStatusData data = {};

		// Level
		std::getline(ss, token, ',');
		int level = std::stoi(token);

		// MaxHp
		std::getline(ss, token, ',');
		data.maxHp = std::stof(token);

		// MaxSt
		std::getline(ss, token, ',');
		data.maxSt = std::stof(token);

		// GainSt
		std::getline(ss, token, ',');
		data.gainSt = std::stof(token);

		// AttackCost1
		std::getline(ss, token, ',');
		data.attackCost1 = std::stof(token);

		// AttackCost2
		std::getline(ss, token, ',');
		data.attackCost2 = std::stof(token);

		// AttackCost3
		std::getline(ss, token, ',');
		data.attackCost3 = std::stof(token);

		// AvoidCost
		std::getline(ss, token, ',');
		data.avoidCost = std::stof(token);

		// StepMag
		std::getline(ss, token, ',');
		data.stepMag = std::stof(token);

		// AttackMag
		std::getline(ss, token, ',');
		data.attackMag = std::stof(token);

		// AtSpeedMag
		std::getline(ss, token, ',');
		data.atSpeedMag = std::stof(token);

		// NegTime
		std::getline(ss, token, ',');
		data.negTime = std::stof(token);

		// NegProb
		std::getline(ss, token, ',');
		data.negProb = std::stof(token);

		// Can1B
		std::getline(ss, token, ',');
		data.can1B = (std::stoi(token) != 0);

		sStatusTable[enemyName][level] = data;
	}

	file.close();
	return true;
}

const EnemyStatusData* CEnemyStatusLoader::GetStatus(
	const std::string& enemyName, int level)
{
	auto itEnemy = sStatusTable.find(enemyName);
	if (itEnemy == sStatusTable.end()) return nullptr;

	auto itLevel = itEnemy->second.find(level);
	if (itLevel == itEnemy->second.end()) return nullptr;

	return &itLevel->second;
}
