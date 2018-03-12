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
	//���݂̃V�[���Ƃ̐e�q�֌W������
	RemoveChild(scene);
	scene.reset();
	switch (scenekind)
	{
	case SceneKind::Title:
		scene = make_shared<TitleScene>();
		break;
	case SceneKind::Game:
		scene = make_shared<GameScene>();
		break;
	case SceneKind::Result:
		scene = make_shared<ResultScene>();
		break;
	default:
		assert(!"���݂��Ȃ��N���X");
		scene = make_shared<NullScene>();
		break;
	}

	//���̃V�[�����q���Ƃ��ēo�^
	AddChild(scene);

	//�V�[���I�u�W�F�N�g�ɂɃV�[���ύX�̃I�u�W�F�N�g�֐���o�^����
	void SetReplaceSceneObject();
}



