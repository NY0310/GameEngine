#include "Character.h"

using namespace std;





void Character::CheckData()
{
	if (hp <= 0)
	{
		isDied = true;
		OnDied();
	}
}

