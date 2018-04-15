#include "SceneManager.h"
using namespace std;
using namespace NYLibrary;
using namespace Scene;


/// <summary>
/// �V�[����ύX����
/// </summary>
/// <param name="scene">���̃V�[��</param>
void SceneManager::ReplaceScene(SceneKind scenekind)
{

	if (scenekind == SceneKind::NullScene)
		return;
	int c = 0;
	Count(c);
	if (scene)
	{
		//���݂̃V�[���Ƃ̐e�q�֌W������
		scene->LoopFinalize();
	}
	c = 0;

	Count(c);

	scene = nullptr;
	//scene.reset();
	switch (scenekind)
	{
	case SceneKind::TitleScene:
		scene = make_shared<TitleScene>();
		break;
	case SceneKind::GameScene:
		scene = make_shared<GameScene>();
		break;
	case SceneKind::ResultScene:
		scene = make_shared<ResultScene>();
		break;
	default:
		assert(!"���݂��Ȃ��N���X");
		scene = make_shared<NullScene>();
		break;
	}

	c = 0;

	Count(c);

	//���̃V�[�������Z�b�g
	nextScene = SceneKind::NullScene;
	//���̃V�[�����q���Ƃ��ēo�^
	AddChild(scene);
	LoopCreateAddChild();
	LoopInitialize();

	c = 0;

	Count(c);

}

/// <summary>
/// �X�V
/// </summary>
void SceneManager::Update()
{
	ReplaceScene(nextScene);
}



