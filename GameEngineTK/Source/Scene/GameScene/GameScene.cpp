#include "GameScene.h"
using namespace std;
using namespace NYLibrary;

void Scene::GameScene::CreateAddChild()
{
	player = make_shared<Player>(10);
	AddChild(player);
	map = make_shared<Map>();
	AddChild(map);
}
