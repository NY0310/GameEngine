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
#include "Scene.h"
#include "GameScene/GameScene.h"
#include "TitleScene/TitleScene.h"
#include "ResultScene/ResultScene.h"


namespace Scene
{

	class SceneManager : public NYLibrary::RootGameObjectNode
	{
	public:
		//�R���X�g���N�^
		SceneManager() = default;
		//�f�X�g���N�^
		~SceneManager() = default;
		//�R�s�[�R���X�g���N�^�֎~
		SceneManager(const SceneManager&) = delete;
		//����֎~
		SceneManager& operator=(const SceneManager&) = delete;
		//�V�[���ύX
		void ReplaceScene(SceneKind scenekind);
		//void CreateAddChild();
	private:
		//�V�[��
		std::shared_ptr<SceneAbstract> scene;
		//�V�[���I�u�W�F�N�g�ɂɃV�[���ύX�̃I�u�W�F�N�g�֐���o�^����
		void SetReplaceSceneObject(SceneKind scenekind){
			std::function<void(SceneKind)> thisFunction = std::bind(&SceneManager::ReplaceScene, this, std::placeholders::_1);
			scene->addListener(thisFunction);
		}
	};
};


