/// <summary>
/// �^�C�g���V�[��
/// Yusuke Nakata
/// 2018/01/11
/// </summary>
#pragma once
#include "../Scene.h"

namespace Scene
{

	class TitleScene : public SceneAbstract {
	public:
		TitleScene() = default;
		//������
		void Initialize()final;
		//�X�V
		void Update()final;
		//�`��
		void Render()final {};
		//�I��
		void Finalize()final {};
		//�R�s�[�R���X�g���N�^�֎~
		TitleScene(const TitleScene&) = delete;
		//����֎~
		TitleScene& operator=(const TitleScene&) = delete;
	};
};