#ifndef DUNGEON_H
#define DUNGEON_H

#include <iostream>
#include <random>
#include <vector>

class Player;
class Monster;

namespace 
{
	std::random_device rd;
	std::mt19937 mt(rd());

	int randomInt(int exclusiveMax)
	{
		std::uniform_int_distribution<> dist(0, exclusiveMax - 1);
		return dist(mt);
	}

	int randomInt(int min, int max)
	{
		std::uniform_int_distribution<> dist(0, max - min);
		return dist(mt) + min;
	}

	bool randomBool(double probability = 0.5)
	{
		std::bernoulli_distribution dist(probability);
		return dist(mt);
	}
}

struct Rect
{
	int x, y;
	int	width, height;
};

class Dungeon
{
public:
	enum class Tile
	{
		Unused = '.',
		Floor = ' ',
		Wall = '#',
		Player = 'P',
		Monster = 'M',
	};

	enum class Direction
	{
		North,
		South,
		West,
		East,
		DirectionCount
	};

private:
	int _width, _height;
	std::vector<Tile> _tiles;
	std::vector<Rect> _rooms;
	std::vector<Rect> _exits;

public:
	Tile hidedTileByPlayer = Tile::Floor;
	Tile hidedTileByMonster = Tile::Floor;

	Dungeon(int width, int height);

	Tile getTile(int x, int y) const;
	void setTile(int x, int y, Tile tile);

	void generate(int mixFeatures);
	void print();

	bool placePlayer(Player& p);
	bool placeMonster(Monster& m);

private:
	bool createFeature();
	bool createFeature(int x, int y, Direction dir);

	bool makeRoom(int x, int y, Direction dir, bool firstRoom = false);
	//bool makeCorridor(int x, int y, Direction dir);

	bool placeRect(const Rect& rect, Tile tile);
	bool placeObject(Tile tile);
	bool placeObject(Tile tile, int& x, int& y);
};

#endif