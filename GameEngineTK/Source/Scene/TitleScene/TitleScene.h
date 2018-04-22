/// <summary>
/// �^�C�g���V�[��
/// Yusuke Nakata
/// 2018/01/11
/// </summary>
#pragma once
#include "../Scene.h"
#include "../GameScene/Charactor/Player/MainPlayer.h"
#include "../TitleScene/PlayUI/PlayUI.h"
#include "../GameScene/Stage/Stage.h"
#include "../../NYLibrary/Sound/ADX2Le.h"
#include "../../Sounds/CueSheet_0.h"


namespace Scene
{

	class TitleScene : public SceneAbstract {
	public:
		TitleScene() = default;
		//�I�u�W�F�N�g�𐶐����q���Ƃ��ēo�^
		void CreateAddChild();
		//������
		void Initialize()final;
		//�X�V
		void Update()final;
		//�`��
		void Render()final {};
		//�I��
		void Finalize();
		//�R�s�[�R���X�g���N�^�֎~
		TitleScene(const TitleScene&) = delete;
		//����֎~
		TitleScene& operator=(const TitleScene&) = delete;
	private:
		std::shared_ptr<Player> player;
		std::shared_ptr<PlayUI> playUI;
		std::shared_ptr<Stage> floor;
	};
};