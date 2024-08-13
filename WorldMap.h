#pragma once

#include <vector>

enum class TileType
{
	Grass,
	Water,
	Fire,
	Woods,
	Wall
};

struct Tile
{
	TileType m_TileType;
	int m_X;
	int m_Y;
	int m_Score;
	int m_Cost;

	int GetCost() const;
	void SetCost(int value);
	int GetHeuristic(int goalX, int goalY) const;
	int GetScore() const;
	void SetScore(int value);

	Tile* m_ParentTile = nullptr;

	//friend bool operator==(const Tile& LHS, const Tile& RHS);

	int GetTileTypeCost();

	void OutputLocation();

	bool IsFoundInList(const std::vector<Tile*>& tileList) const;


	~Tile() 
	{
	}

public:
	std::vector<Tile*> GetAllParents(std::vector<Tile*> deletedEntites);

	void DeallocateMemory(std::vector<Tile*> deletedEntites);
};

class WorldMap
{
public:
	static const int mapSize = 4;

	Tile tiles[mapSize][mapSize] = { 
		{{TileType::Grass, 0, 0}, {TileType::Grass, 0, 1}, {TileType::Grass, 0, 2}, {TileType::Grass, 0, 3}},
		{{TileType::Grass, 1, 0}, {TileType::Water, 1, 1}, {TileType::Fire, 1, 2}, {TileType::Grass, 1, 3}},
		{{TileType::Grass, 2, 0}, {TileType::Fire, 2, 1}, {TileType::Water, 2, 2}, {TileType::Grass, 2, 3}},
		{{TileType::Woods, 3, 0}, {TileType::Grass, 3, 1}, {TileType::Grass, 3, 2}, {TileType::Woods, 3, 3}} };



};