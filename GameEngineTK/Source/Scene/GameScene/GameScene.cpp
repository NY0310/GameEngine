#include "GameScene.h"
using namespace std;
using namespace NYLibrary;

void Scene::GameScene::CreateAddChild()
{
	player = make_shared<Player>();
	AddChild(player);
	stage = make_shared<Stage>();
	AddChild(stage);
	//map = make_shared<Map>();
	//AddChild(map);
	enemy = make_shared<Enemy>();
	AddChild(enemy);
}