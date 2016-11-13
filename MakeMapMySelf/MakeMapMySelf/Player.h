#ifndef PLAYER_H
#define PLAYER_H

class Dungeon;

class Player
{
public:
	enum Direction
	{
		North,
		South,
		West,
		East,
		DirectionCount,
	};

private:
	int x, y;

public:
	void setPosition(int x, int y);

	void move(Dungeon& dungeon, Direction dir);
};

#endif