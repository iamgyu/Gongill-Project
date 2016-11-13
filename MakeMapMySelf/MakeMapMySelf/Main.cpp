#include "Dungeon.h"
#include "Player.h"
#include "Monster.h"

int main()
{
	Player p;
	Monster m;

	Dungeon d(79, 24);
	d.generate(50);
	d.placePlayer(p);
	d.placeMonster(m);

	while (true)
	{
		d.print();

		int randomNum = randomInt(0, 3);

		char input;
		std::cout << "Press W(Up), A(Left), S(Down), D(Right)";
		std::cin >> input;

		if (input == 'W' || input == 'w')
		{
			p.move(d, Player::Direction::North);
			if (randomNum == 0)
				m.move(d, Monster::Direction::North);
			else if (randomNum == 1)
				m.move(d, Monster::Direction::West);
			else if (randomNum == 2)
				m.move(d, Monster::Direction::South);
			else if (randomNum == 3)
				m.move(d, Monster::Direction::East);
		}

		else if (input == 'A' || input == 'a')
		{
			p.move(d, Player::Direction::West);
			if (randomNum == 0)
				m.move(d, Monster::Direction::North);
			else if (randomNum == 1)
				m.move(d, Monster::Direction::West);
			else if (randomNum == 2)
				m.move(d, Monster::Direction::South);
			else if (randomNum == 3)
				m.move(d, Monster::Direction::East);
		}

		else if (input == 'S' || input == 's')
		{
			p.move(d, Player::Direction::South);
			if (randomNum == 0)
				m.move(d, Monster::Direction::North);
			else if (randomNum == 1)
				m.move(d, Monster::Direction::West);
			else if (randomNum == 2)
				m.move(d, Monster::Direction::South);
			else if (randomNum == 3)
				m.move(d, Monster::Direction::East);
		}

		else if (input == 'D' || input == 'd')
		{
			p.move(d, Player::Direction::East);
			if (randomNum == 0)
				m.move(d, Monster::Direction::North);
			else if (randomNum == 1)
				m.move(d, Monster::Direction::West);
			else if (randomNum == 2)
				m.move(d, Monster::Direction::South);
			else if (randomNum == 3)
				m.move(d, Monster::Direction::East);
		}

		else std::cout << "Invalid Input!" << std::endl;

		system("cls");
	}


	return 0;
}