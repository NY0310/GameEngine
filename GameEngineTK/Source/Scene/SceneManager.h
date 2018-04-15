/// <summary>
/// �V�[���Ǘ��N���X
/// Yusuke Nakata
/// 2018/01/10
/// </summary>
#pragma once
#include <memory>
#include <assert.h>
#include <functional>
#include "../NYLibrary/SceneGraph/Node/GameObjectNode.h"
#include "GameScene/GameScene.h"
#include "TitleScene/TitleScene.h"
#include "ResultScene/ResultScene.h"


namespace Scene
{

	class SceneManager : public NYLibrary::RootGameObjectNode
	{
	public:
		//�R���X�g���N�^
		SceneManager() { SetReplaceSceneObject(); }
		//�f�X�g���N�^
		~SceneManager() = default;
		//�R�s�[�R���X�g���N�^�֎~
		SceneManager(const SceneManager&) = delete;
		//����֎~
		SceneManager& operator=(const SceneManager&) = delete;
		//�V�[���ύX
		void ReplaceScene(SceneKind scenekind);
		//�X�V
		void Update();
		//���̃V�[����o�^
		void RegisterNextScene(SceneKind nextScene) {
			this->nextScene = nextScene;
		}
	private:
		//�V�[��
		std::shared_ptr<SceneAbstract> scene;
		//�V�[���I�u�W�F�N�g�ɃV�[���ύX�̃I�u�W�F�N�g�֐���o�^����
		void SetReplaceSceneObject(){
			std::function<void(SceneKind)> thisFunction = std::bind(&SceneManager::RegisterNextScene, this, std::placeholders::_1);
			AddRunWithScene(thisFunction);
		}
		//���̃V�[��
		SceneKind nextScene;
	};
};


