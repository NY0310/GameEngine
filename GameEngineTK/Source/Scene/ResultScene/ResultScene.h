/// <summary>
/// ���U���g�V�[��
/// Yusuke Nakata
/// 2018/01/11
/// </summary>
#pragma once
#include "../Scene.h"

namespace Scene
{
	class ResultScene : public SceneAbstract {
	public:
		//�R���X�g���N�^
		ResultScene() = default;
		//������
		void Initialize()final {};
		//�X�V
		void Update()final {};
		//�`��
		void Render()final {};
		//�I��
		void Finalize()final {};
		//�R�s�[�R���X�g���N�^�֎~
		ResultScene(const ResultScene&) = delete;
		//����֎~
		ResultScene& operator=(const ResultScene&) = delete;
	};

};