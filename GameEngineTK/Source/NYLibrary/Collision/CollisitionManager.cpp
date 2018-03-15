#include "CollisitionManager.h"

using namespace std;

/// <summary>
/// CollisitionManager���擾����
/// </summary>
/// <returns>CollisitionManager</returns>
CollisitionManager * CollisitionManager::GetInstance()
{
	if (!collisitionManager)
	{
		collisitionManager = make_unique<CollisitionManager>();
	}
	return collisitionManager.get();
}
