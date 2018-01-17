#pragma once
/// <summary>
/// �Q�[���I�u�W�F�N�g�C���^�[�t�F�C�X 
/// ���̃N���X���p�����Ď���I�u�W�F�N�g�N���X���쐬����
/// 2017/10/10
/// Yusuke Nakata
/// </summary>

namespace gameobject
{
	class GameObjectInterface
	{
	public:
		//�R���X�g���N�^
		GameObjectInterface() = default;
		//�R�s�[�R���X�g���N�^�֎~
		GameObjectInterface(const GameObjectInterface&) = delete;
		//����֎~
		GameObjectInterface& operator=(const GameObjectInterface&) = delete;

		//�f�X�g���N�^
		virtual ~GameObjectInterface() = default;

		//������
		virtual void Initialize() = 0;

		//�X�V����
		virtual void Update() = 0;

		//�`��
		virtual void Render() = 0;

		//�I��
		virtual void Finalize() = 0;
	private:

	};

}


