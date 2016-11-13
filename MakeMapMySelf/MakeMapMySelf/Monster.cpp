#include "Monster.h"
#include "Dungeon.h"


void Monster::setPosition(int x, int y)
{
	this->x = x;
	this->y = y;
}

void Monster::move(Dungeon& dungeon, Direction dir)
{
	int dx = 0, dy = 0;

	if (dir == Direction::North) dy = -1;
	else if (dir == Direction::South) dy = 1;
	else if (dir == Direction::West) dx = -1;
	else if (dir == Direction::East) dx = 1;

	Dungeon::Tile nextTile = dungeon.getTile(x + dx, y + dy);

	if (nextTile == Dungeon::Tile::Floor)
	{
		dungeon.setTile(x, y, dungeon.hidedTileByMonster);
		dungeon.setTile(x + dx, y + dy, Dungeon::Tile::Monster);
		dungeon.hidedTileByMonster = nextTile;
		x = x + dx;
		y = y + dy;
	}
}