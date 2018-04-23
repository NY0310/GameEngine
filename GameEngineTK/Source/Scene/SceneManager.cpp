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

	if (scene)
	{
		//���݂̃V�[���Ƃ̐e�q�֌W������
		scene->LoopFinalize();
	}

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



	//���̃V�[�������Z�b�g
	nextScene = SceneKind::NullScene;
	//���̃V�[�����q���Ƃ��ēo�^
	AddChild(scene);
	LoopCreateAddChild();
	LoopInitialize();
}

/// <summary>
/// �X�V
/// </summary>
void SceneManager::Update()
{
	ReplaceScene(nextScene);
}



