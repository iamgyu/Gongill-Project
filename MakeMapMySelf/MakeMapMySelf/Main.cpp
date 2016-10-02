#include "Dungeon.h"

int main()
{
	Dungeon d(79, 24);
	d.generate(50);

	d.print();
	return 0;
}