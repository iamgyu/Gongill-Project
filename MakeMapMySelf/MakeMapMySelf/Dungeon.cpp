#include "Dungeon.h"
#include "Player.h"
#include "Monster.h"

Dungeon::Dungeon(int width, int height)	// 멤버 이니셜라이저를 통한 변수들 초기화
	:_width(width)
	, _height(height)
	, _tiles(width * height, Tile::Unused)
	, _rooms()
	, _exits() {}

void Dungeon::generate(int maxFeatures)	// 방을 만들 수 있는지 없는지, 그 자리에 feature을 둘 수 있는지 없는지 체크 하는 함수
{
	if (!makeRoom(_width / 2, _height / 2, static_cast<Direction>(randomInt(4), true)))	// 중앙에 방을 만들 수 없을 때
	{
		std::cout << "Can't place the first room....";
		return;
	}

	for (int i = 1; i < maxFeatures; i++)
	{
		if (!createFeature())	// feture을 둘 수 없을 때
		{
			std::cout << "Can't place more features (place " << i << ").\n";
			break;
		}
	}
}

void Dungeon::print()	// 맵 출력
{
	for (int y = 0; y < _height; ++y)
	{
		for (int x = 0; x < _width; ++x)
			std::cout << static_cast<char>(getTile(x, y));

		std::cout << std::endl;
	}
}

Dungeon::Tile Dungeon::getTile(int x, int y) const	// 정보 은닉을 위한 getter
{
	if (x < 0 || y < 0 || x >= _width || y >= _height)
		return Tile::Unused;

	return _tiles[x + y * _width];
}

void Dungeon::setTile(int x, int y, Tile tile)	// 정보 은닉을 위한 setter
{
	_tiles[x + y * _width] = tile;
}

bool Dungeon::createFeature()	// feature을 둘 수 있는지 가능 여부를 판단하는 함수
{
	for (int i = 0; i < 1000; ++i)
	{
		if (_exits.empty())	
			break;

		// choose a random size of a random room or corridor
		int r = randomInt(_exits.size());
		int x = randomInt(_exits[r].x, _exits[r].x + _exits[r].width - 1);
		int y = randomInt(_exits[r].y, _exits[r].y + _exits[r].height - 1);

		for (int j = 0; j < static_cast<int>(Direction::DirectionCount); ++j)	
		{
			if (createFeature(x, y, static_cast<Direction>(j)))		// overload 된 함수 이용
			{
				_exits.erase(_exits.begin() + r);
				return true;
			}
		}
	}

	return false;
}

bool Dungeon::createFeature(int x, int y, Direction dir)	// 또 다른 feature을 둘 수 있는지 가능 여부를 판단 하는 함수 (function overloading)
{
	static const int roomchance = 50;	// corridorChance = 100 - roomChance
	
	int dx = 0;
	int dy = 0;

	if (dir == Direction::North)	// North
		dy = 1;
	else if (dir == Direction::South)	// South
		dy = -1; 
	else if (dir == Direction::West)	// West
		dx = 1;
	else if (dir == Direction::East)	// East
		dx = -1;

	if (getTile(x + dx, y + dy) != Tile::Floor)
		return false;

	if (randomInt(100) < roomchance)	
	{
		if (makeRoom(x, y, dir))
		{
			setTile(x, y, Tile::Floor);

			return true;
		}
	}
	
	/*
	else
	{
		if (makeCorridor(x, y, dir))
		{
			if (getTile(x + dx, y + dy) == Tile::Floor)
				setTile(x, y, Tile::Floor);
			else
				setTile(x, y, Tile::Corridor);

			return true;
		}
	}
	*/

	return false;
}

bool Dungeon::makeRoom(int x, int y, Direction dir, bool firstRoom)		// 방을 만들어 내는 함수
{
	static const int minRoomSize = 3;
	static const int maxRoomSize = 6;

	Rect room;
	room.width = randomInt(minRoomSize, maxRoomSize);
	room.height = randomInt(minRoomSize, maxRoomSize);

	if (dir == Direction::North)
	{
		room.x = x - room.width / 2;
		room.y = y - room.height;
	}

	else if (dir == Direction::South)
	{
		room.x = x - room.width / 2;
		room.y = y + 1;
	}

	else if (dir == Direction::West)
	{
		room.x = x - room.width;
		room.y = y - room.height / 2;
	}

	else if (dir == Direction::East)
	{
		room.x = x + 1;
		room.y = y - room.height / 2;
	}

	if (placeRect(room, Tile::Floor))	// room 위치에 Floor을 둘 수 있다면
	{
		_rooms.emplace_back(room);

		if (dir != Direction::South || firstRoom)
			_exits.emplace_back(Rect{ room.x, room.y - 1, room.width, 1 });
		if (dir != Direction::North || firstRoom)
			_exits.emplace_back(Rect{ room.x, room.y + room.height, room.width, 1 });
		if (dir != Direction::East || firstRoom)
			_exits.emplace_back(Rect{ room.x - 1, room.y, 1, room.height });
		if (dir != Direction::West || firstRoom)
			_exits.emplace_back(Rect{ room.x + room.width, room.y, 1, room.height });

		return true;
	}

	return false;
}

/*	-> 복도를 없앴을 경우 맵이 더 넓어지는 효과를 가져옴, 대신 맵이 좀 이상함
bool Dungeon::makeCorridor(int x, int y, Direction dir)
{
	static const int minCorridorLenght = 3;
	static const int maxCorridorLenght = 6;

	Rect corridor;
	corridor.x = x;
	corridor.y = y;

	if (randomBool())
	{
		corridor.width = randomInt(minCorridorLenght, maxCorridorLenght);
		corridor.height = 1;

		if (dir == Direction::North)
		{
			corridor.y = y - 1;
			
			if (randomBool()) // west
				corridor.x = x - corridor.width + 1;
		}

		else if (dir == Direction::South)
		{
			corridor.y = y + 1;

			if (randomBool()) // west
				corridor.x = x - corridor.width + 1;
		}

		else if (dir == Direction::West)
			corridor.x = x - corridor.width;

		else if (dir == Direction::East)
			corridor.x = x + 1;
	}

	else // vertical corridor
	{
		corridor.width = 1;
		corridor.height = randomInt(minCorridorLenght, maxCorridorLenght);

		if (dir == Direction::North)
			corridor.y = y - corridor.height;
		else if (dir == Direction::South)
			corridor.y = y + 1;
		else if (dir == Direction::West)
		{
			corridor.x = x - 1;

			if (randomBool()) // north
				corridor.y = y - corridor.height + 1;
		}

		else if (dir == Direction::East)
		{
			corridor.x = x + 1;

			if(randomBool()) // north
				corridor.y = y - corridor.height + 1;
		}
	}

	if (placeRect(corridor, Tile::Corridor))
	{
		if (dir != Direction::South && corridor.width != 1)
			_exits.emplace_back(Rect{ corridor.x, corridor.y - 1, corridor.width, 1 });
		if (dir != Direction::North && corridor.width != 1)
			_exits.emplace_back(Rect{ corridor.x, corridor.y + corridor.height, corridor.width, 1 });
		if (dir != Direction::East && corridor.height != 1)
			_exits.emplace_back(Rect{ corridor.x - 1, corridor.y, 1, corridor.height });
		if (dir != Direction::West && corridor.height != 1)
			_exits.emplace_back(Rect{ corridor.x + corridor.width, corridor.y, 1, corridor.height });
		
		return true;
	}

	return false;
}
*/

bool Dungeon::placeRect(const Rect& rect, Tile tile)	// 특정 좌표에 tile을 둘 수 있는 지 가능 여부를 판단하는 함수
{
	if (rect.x < 1 || rect.y < 1 || rect.x + rect.width > _width - 1 || rect.y + rect.height > _height - 1)
		return false;

	for (int y = rect.y; y < rect.y + rect.height; ++y)
		for (int x = rect.x; x < rect.x + rect.width; ++x)
		{
			if (getTile(x, y) != Tile::Unused)
				return false; // area already used
		}

	for (int y = rect.y - 1; y < rect.y + rect.height + 1; ++y)
		for (int x = rect.x - 1; x < rect.x + rect.width + 1; ++x)
		{
			if (x == rect.x - 1 || y == rect.y - 1 || x == rect.x + rect.width || y == rect.y + rect.height)	// Wall tile을 두는 조건
				setTile(x, y, Tile::Wall);
			else
				setTile(x, y, tile);
		}

	return true;
}


bool Dungeon::placeObject(Tile tile)	
{
	if (_rooms.empty())
		return false;

	int r = randomInt(_rooms.size());
	int x = randomInt(_rooms[r].x + 1, _rooms[r].x + _rooms[r].width - 2);
	int y = randomInt(_rooms[r].y + 1, _rooms[r].y + _rooms[r].height - 2);

	if (getTile(x, y) == Tile::Floor)
	{
		setTile(x, y, tile);

		_rooms.erase(_rooms.begin() + r);

		return true;
	}

	return false;
}

bool Dungeon::placeObject(Tile tile, int& _x, int& _y)
{
	if (_rooms.empty())
		return false;

	int r = randomInt(_rooms.size()); // choose a random room
	int x = randomInt(_rooms[r].x + 1, _rooms[r].x + _rooms[r].width - 2);
	int y = randomInt(_rooms[r].y + 1, _rooms[r].y + _rooms[r].height - 2);

	if (getTile(x, y) == Tile::Floor)
	{
		setTile(x, y, tile);
		_x = x;
		_y = y;

		_rooms.erase(_rooms.begin() + r);
		
		return true;
	}

	return false;
}

bool Dungeon::placePlayer(Player& p)
{
	int x = 0, y = 0;
	if (placeObject(Tile::Player, x, y))
	{
		p.setPosition(x, y);
		return true;
	}

	return false;
}

bool Dungeon::placeMonster(Monster& m)
{
	int x = 1, y = 0;
	if (placeObject(Tile::Monster, x, y))
	{
		m.setPosition(x, y);
		return true;
	}

	return false;
}