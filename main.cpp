#include <algorithm>
#include <iostream>
#include <vector>
#include "WorldMap.h"

using namespace std;

vector<Tile*> ExploreNode(Tile* tile, WorldMap* worldMap);
Tile* FindPath(int startx, int starty, int goalx, int goaly, vector<Tile*>& openList, vector<Tile*>& closedList, WorldMap* worldMap, std::vector<Tile*>& danglingEntities);

int main()
{
	cout << "Hello World" << endl;

	WorldMap* worldMap = new WorldMap();
	std::cout << "New World Map created" << std::endl;

	int startx = 0;
	int starty = 0;

	int goalx = 3;
	int goaly = 3;

	vector<Tile*> openList;
	vector<Tile*> closedList;

	//Tile* StartTile = &worldMap->tiles[starty][startx];

	std::vector<Tile*> danglingEntities;

	Tile* successTile = FindPath(startx, starty, goalx, goaly, openList, closedList, worldMap, danglingEntities);
	successTile->OutputLocation();
	Tile* currentTile = successTile;
	
	while (currentTile != nullptr)
	{
		auto openIt = std::remove_if(openList.begin(), openList.end(), [&](const Tile* tile /* 0x57847623 */) { return currentTile == tile; });
		openList.erase(openIt, openList.end());
		auto closedIt = std::remove_if(closedList.begin(), closedList.end(), [&](const Tile* tile /* 0x57847623 */) { return currentTile == tile; });
		closedList.erase(closedIt, closedList.end());
		currentTile = currentTile->m_ParentTile;
	}

	std::vector<Tile*> parents;
	std::vector<Tile*> deletedEntities;
	for (Tile* openTile : openList)
	{
		auto v = openTile->GetAllParents(deletedEntities);
		parents.insert(parents.end(), v.begin(), v.end());
	}
	for (Tile* closedTile : closedList)
	{
		auto v = closedTile->GetAllParents(deletedEntities);
		parents.insert(parents.end(), v.begin(), v.end());
	}
	for (Tile* danglingTile : danglingEntities)
	{
		auto v = danglingTile->GetAllParents(deletedEntities);
		parents.insert(parents.end(), v.begin(), v.end());
	}

	std::sort(parents.begin(), parents.end());
	parents.erase(std::unique(parents.begin(), parents.end()), parents.end());

	for (Tile* openTile : openList)
	{

		if (std::find(parents.begin(), parents.end(), openTile) == parents.end() && std::find(closedList.begin(), closedList.end(), openTile) == closedList.end())
		{
			auto allParents = openTile->GetAllParents(deletedEntities);
			openTile->DeallocateMemory(deletedEntities);
			deletedEntities.insert(deletedEntities.end(), allParents.begin(), allParents.end());
			deletedEntities.push_back(openTile);
			delete openTile;
		}
	}
	for (Tile* closedTile : closedList)
	{
		if (std::find(parents.begin(), parents.end(), closedTile) == parents.end())
		{
			auto allParents = closedTile->GetAllParents(deletedEntities);
			closedTile->DeallocateMemory(deletedEntities);
			deletedEntities.insert(deletedEntities.end(), allParents.begin(), allParents.end());
			deletedEntities.push_back(closedTile);
			delete closedTile;
		}
	}
	for (Tile* danglingTile : danglingEntities)
	{
		if (std::find(parents.begin(), parents.end(), danglingTile) == parents.end())
		{
			auto allParents = danglingTile->GetAllParents(deletedEntities);
			danglingTile->DeallocateMemory(deletedEntities);
			deletedEntities.insert(deletedEntities.end(), allParents.begin(), allParents.end());
			deletedEntities.push_back(danglingTile);
			
		}
		delete danglingTile;
	}
	delete worldMap;
	std::cout << "world map deleted" << std::endl;
	successTile->DeallocateMemory(deletedEntities);
	delete successTile;
	std::cout << "Tile deleted" << std::endl;
	_CrtDumpMemoryLeaks();
	return 0;
}

vector<Tile*> ExploreNode(Tile* tile, WorldMap* worldMap)
{
	vector<Tile*> successorNodes;

	int X = tile->m_X;
	int Y = tile->m_Y;

	if (Y < worldMap->mapSize - 1)
	{
		//Tile* NorthTile = &worldMap->tiles[Y + 1][X];
		Tile* NorthTile = new Tile{};
		std::cout << "New tile created" << std::endl;
		NorthTile->m_Cost = worldMap->tiles[Y + 1][X].m_Cost;
		NorthTile->m_ParentTile = tile;
		NorthTile->m_Score = worldMap->tiles[Y + 1][X].m_Score;
		NorthTile->m_TileType = worldMap->tiles[Y + 1][X].m_TileType;
		NorthTile->m_X = worldMap->tiles[Y + 1][X].m_X;
		NorthTile->m_Y = worldMap->tiles[Y + 1][X].m_Y;
		//NorthTile->m_ParentTile = tile;
		successorNodes.push_back(NorthTile);
	}
	if (Y > 0)
	{
		//Tile* SouthTile = &worldMap->tiles[Y - 1][X];
		//SouthTile->m_ParentTile = tile;
		Tile* SouthTile = new Tile{};
		std::cout << "New tile created" << std::endl;
		SouthTile->m_Cost = worldMap->tiles[Y - 1][X].m_Cost;
		SouthTile->m_ParentTile = tile;
		SouthTile->m_Score = worldMap->tiles[Y - 1][X].m_Score;
		SouthTile->m_TileType = worldMap->tiles[Y - 1][X].m_TileType;
		SouthTile->m_X = worldMap->tiles[Y - 1][X].m_X;
		SouthTile->m_Y = worldMap->tiles[Y - 1][X].m_Y;
		successorNodes.push_back(SouthTile);
	}
	if (X < worldMap->mapSize - 1)
	{
		Tile* EastTile = new Tile{};
		std::cout << "New tile created" << std::endl;
		EastTile->m_Cost = worldMap->tiles[Y][X + 1].m_Cost;
		EastTile->m_ParentTile = tile;
		EastTile->m_Score = worldMap->tiles[Y][X + 1].m_Score;
		EastTile->m_TileType = worldMap->tiles[Y][X + 1].m_TileType;
		EastTile->m_X = worldMap->tiles[Y][X + 1].m_X;
		EastTile->m_Y = worldMap->tiles[Y][X + 1].m_Y;
		successorNodes.push_back(EastTile);
	}
	if (X > 0)
	{
		//Tile* WestTile = &worldMap->tiles[Y][X - 1];
		//WestTile->m_ParentTile = tile;
		Tile* WestTile = new Tile{};
		std::cout << "New tile created" << std::endl;
		WestTile->m_Cost = worldMap->tiles[Y][X - 1].m_Cost;
		WestTile->m_ParentTile = tile;
		WestTile->m_Score = worldMap->tiles[Y][X - 1].m_Score;
		WestTile->m_TileType = worldMap->tiles[Y][X - 1].m_TileType;
		WestTile->m_X = worldMap->tiles[Y][X - 1].m_X;
		WestTile->m_Y = worldMap->tiles[Y][X - 1].m_Y;
		successorNodes.push_back(WestTile);
	}

	return successorNodes;
}

Tile* FindPath(int startx, int starty, int goalx, int goaly, vector<Tile*>& openList, vector<Tile*>& closedList, WorldMap* worldMap, std::vector<Tile*>& danglingEntities)
{
	Tile* StartTile = new Tile;
	std::cout << "New tile created" << std::endl;
	StartTile->m_Cost = worldMap->tiles[starty][startx].m_Cost;
	StartTile->SetScore(StartTile->GetHeuristic(goalx, goaly) + StartTile->GetCost());
	StartTile->m_X = startx;
	StartTile->m_Y = starty;
	openList.push_back(StartTile);

	while (openList.size() > 0)
	{
		Tile* Q = nullptr;


		auto Qit = std::min_element(openList.begin(), openList.end(), [&](const Tile* lhs, const Tile* rhs) {
			return lhs->GetScore() < rhs->GetScore();
			});


		if (Qit != openList.end())
		{
			Q = (*Qit);
		}
		auto it = std::remove(openList.begin(), openList.end(), (*Qit));
		openList.erase(it);

		bool shouldExitLoop = false;
		vector<Tile*> successorNodes;
		if (Q != nullptr)
		{
			successorNodes = ExploreNode(Q, worldMap);
		}
		for (Tile* successorNode : successorNodes)
		{
			if (successorNode->m_X == goalx && successorNode->m_Y == goaly)
			{
				for (Tile* T : successorNodes)
				{
					if (T != successorNode)
					{
						closedList.push_back(T);
					}
					/*if (std::find_if(openList.begin(), openList.end(), [&](const Tile* tile) { return T == tile; }) == openList.end())
					{
						if (std::find_if(closedList.begin(), closedList.end(), [&](const Tile* tile) { return T == tile; }) == closedList.end())
						{
							if (T != successorNode)
							{
								auto v = T->GetAllParents(deletedEntities);
								T->DeallocateMemory(deletedEntities);
								deletedEntities.insert(deletedEntities.end(), v.begin(), v.end());
								deletedEntities.push_back(T);
								delete T;
								std::cout << "Tile deleted" << std::endl;
							}
						}
					}*/
					//vector<Tile*>::iterator openIt = std::find_if(openList.begin(), openList.end(), [&](const Tile* tile) { return T == tile; });

				}
				danglingEntities.push_back(Q);
				return successorNode;
			}
			float g = (*Q).GetCost() + successorNode->GetTileTypeCost();
			float h = successorNode->GetHeuristic(goalx, goaly);
			successorNode->SetCost(g);
			successorNode->SetScore(g + h);

			vector<Tile*>::iterator existingNodeIt = std::find_if(openList.begin(), openList.end(), [&](const Tile* tile) {
				return (tile->m_X == successorNode->m_X && tile->m_Y == successorNode->m_Y);
				});
			vector<Tile*>::iterator closedNodeIt = std::find_if(closedList.begin(), closedList.end(), [&](const Tile* tile) {
				return (tile->m_X == successorNode->m_X && tile->m_Y == successorNode->m_Y);
				});

			const bool isOnOpenList = existingNodeIt != openList.end();
			const bool isOnClosedList = closedNodeIt != closedList.end();

			if (isOnOpenList)
			{
				if (successorNode->GetScore() > (*existingNodeIt)->GetScore())
				{
					continue;
				}
				else
				{
					(*existingNodeIt)->SetScore(successorNode->GetScore());
					(*existingNodeIt)->SetCost(successorNode->GetCost());

					//auto v = (*existingNodeIt)->GetAllParents(deletedEntities);
					//(*existingNodeIt)->DeallocateMemory(deletedEntities);
					//deletedEntities.insert(deletedEntities.end(), v.begin(), v.end());
					//(*existingNodeIt)->m_ParentTile->DeallocateMemory(deletedEntities);
					//danglingEntities.push_back((*existingNodeIt)->m_ParentTile);
					(*existingNodeIt)->m_ParentTile = successorNode->m_ParentTile;
				}
			}
			if (isOnClosedList)
			{
				if (successorNode->GetScore() < (*closedNodeIt)->GetScore())
				{
					openList.push_back(successorNode);
				}
				else
				{
					continue;
				}
			}
			if (!isOnClosedList && !isOnOpenList)
			{
				openList.push_back(successorNode);
			}
			//openList.push_back(successorNode);
		}
		if (shouldExitLoop)
		{
			return nullptr;
		}
		if (Q != nullptr)
		{
			closedList.push_back(Q);
		}

	}
	return nullptr;
}