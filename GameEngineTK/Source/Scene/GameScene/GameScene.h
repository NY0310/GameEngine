/// <summary>
/// �Q�[���V�[��
/// Yusuke Nakata
/// 2018/01/11
/// </summary>
#pragma once
#include "../Scene.h"
#include "Charactor/Player/MainPlayer.h"
#include "../../NYLibrary/AStar/Map.h"

namespace Scene
{
	class GameScene : public SceneAbstract {
	public:
		//�R���X�g���N�^
		GameScene() = default;
		//�q����ǉ�����
		void CreateAddChild();
		//������
		void Initialize()final { nextSceneKind = SceneKind::Result; };
		//�X�V
		void Update()final {};
		//�`��
		void Render()final {};
		//�I��
		void Finalize()final {};
		//�R�s�[�R���X�g���N�^�֎~
		GameScene(const GameScene&) = delete;
		//����֎~
		GameScene& operator=(const GameScene&) = delete;
	private:
		std::shared_ptr<Player> player;
		std::shared_ptr<Map> map;
	};
};