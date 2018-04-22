#include "GameScene.h"
using namespace std;
using namespace NYLibrary;
using namespace Scene;

void GameScene::CreateAddChild()
{
	player = make_shared<Player>();
	AddChild(player);
	stage = make_shared<Stage>();
	AddChild(stage);
	enemy = make_shared<Enemy>();
	AddChild(enemy);
}

void GameScene::Initialize()
{
	ADX2Le* adx2 = ADX2Le::GetInstance();
	adx2->Play(CRI_CUESHEET_0_GAME_0);
}

void GameScene::Finalize()
{
}
