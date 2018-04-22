/// <summary>
/// タイトルシーン
/// Yusuke Nakata
/// 2018/01/11
/// </summary>
#include "TitleScene.h"
using namespace Scene;
using namespace std;
using namespace NYLibrary;

void TitleScene::CreateAddChild()
{
	//オブジェクトを生成し子供として登録
	player = make_shared<Player>();
	AddChild(player);
	playUI = make_shared<PlayUI>();
	AddChild(playUI);
	floor = make_shared<Stage>();
	AddChild(floor);
}

/// <summary>
/// 初期化
/// </summary>
void TitleScene::Initialize()
{
	ADX2Le* adx2 = ADX2Le::GetInstance();
	adx2->Play(CRI_CUESHEET_0_TITLE_0);
}

/// <summary>
/// 更新
/// </summary>
void TitleScene::Update()
{
}


void TitleScene::Finalize()
{
	ADX2Le* adx2 = ADX2Le::GetInstance();
	adx2->Stop();
}
