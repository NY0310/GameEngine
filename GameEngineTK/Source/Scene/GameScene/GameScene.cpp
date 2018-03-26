#include "GameScene.h"
using namespace std;
using namespace NYLibrary;

void Scene::GameScene::CreateAddChild()
{
	stage = make_shared<Stage>();
	AddChild(stage);
	map = make_shared<Map>();
	AddChild(map);
	player = make_shared<Player>(10);
	AddChild(player);
}
