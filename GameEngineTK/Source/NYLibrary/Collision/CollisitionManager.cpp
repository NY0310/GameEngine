#include "CollisitionManager.h"

using namespace std;

/// <summary>
/// CollisitionManager‚ğæ“¾‚·‚é
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
