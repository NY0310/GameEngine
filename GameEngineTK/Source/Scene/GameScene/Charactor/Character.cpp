#include "Character.h"

using namespace std;





Character::Character()
{
	isDied = false;
	hp = 0;
}

void Character::CheckData()
{
	if (hp <= 0)
	{
		isDied = true;
		OnDied();
	}
}

