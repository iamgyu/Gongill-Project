#ifndef MONSTER_H
#define MONSTER_H

class Dungeon;

class Monster
{
public:
	enum Direction
	{
		North,
		South,
		West,
		East,
		DirectionCount
	};

private:
	int x, y;

public:
	void setPosition(int x, int y);
	
	void move(Dungeon& dungeon, Direction dir);
};

#endif