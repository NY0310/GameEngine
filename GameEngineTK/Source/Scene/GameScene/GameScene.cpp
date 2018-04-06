#include "GameScene.h"
using namespace std;
using namespace NYLibrary;

void Scene::GameScene::CreateAddChild()
{
	player = make_shared<Player>(10);
	AddChild(player);
	stage = make_shared<Stage>();
	AddChild(stage);
	map = make_shared<Map>();
	AddChild(map);
}
