#include "WorldMap.h"
#include <iostream>
#include <math.h>

int Tile::GetCost() const
{
	return m_Cost;
}

int Tile::GetHeuristic(int goalX, int goalY) const
{
	//return sqrt((goalY - m_Y) * (goalY - m_Y) + (goalX - m_X) * (goalX - m_X));
	return abs(goalY - m_Y) + abs(goalX - m_X);
}

int Tile::GetScore() const
{
	return m_Score;
}

void Tile::SetScore(int value)
{
	m_Score = value;
}

int Tile::GetTileTypeCost()
{
	switch (m_TileType)
	{
	case TileType::Grass:
		return 1;
	case TileType::Water:
		return 3;
	case TileType::Fire:
		return 4;
	case TileType::Woods:
		return 2;
	case TileType::Wall:
		return 1000;
	default:
		std::cout << "Invalid tile type" << std::endl;
		return 0;

	}
}

void Tile::OutputLocation()
{
	if (m_ParentTile != nullptr)
	{
		m_ParentTile->OutputLocation();
	}
	std::cout << "X: " << m_X << "Y: " << m_Y << std::endl;
}

void Tile::DeallocateMemory(std::vector<Tile*> deletedEntities)
{
	if (m_ParentTile != nullptr && std::find(deletedEntities.begin(), deletedEntities.end(), m_ParentTile) == deletedEntities.end())
	{
		m_ParentTile->DeallocateMemory(deletedEntities);
		delete m_ParentTile;
	}
	std::cout << "Tile deleted" << std::endl;
}

bool Tile::IsFoundInList(const std::vector<Tile*>& tileList) const
{
	bool found = false;
	if (m_ParentTile != nullptr)
	{
		found = m_ParentTile->IsFoundInList(tileList);
	}
	found = found || std::find_if(tileList.begin(), tileList.end(), [&](const Tile* tile) { return tile == this; }) != tileList.end();
	
	return found;
}

void Tile::SetCost(int value)
{
	m_Cost = value;
}

std::vector<Tile*> Tile::GetAllParents(std::vector<Tile*> deletedEntities)
{
	std::vector<Tile*> result;
	if (m_ParentTile != nullptr && std::find(deletedEntities.begin(), deletedEntities.end(), m_ParentTile) == deletedEntities.end())
	{
		auto v = m_ParentTile->GetAllParents(deletedEntities);
		result.insert(result.end(), v.begin(), v.end());
		result.push_back(m_ParentTile);
	}
	return result;
}

//bool operator==(const Tile& LHS, const Tile& RHS)
//{
//	return (LHS.m_X == RHS.m_X && LHS.m_Y == RHS.m_Y);
//}


