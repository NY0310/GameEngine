/// <summary>
/// GameObjectNode
/// 2017/10/13
/// Yusuke Nakata
/// </summary>
#pragma once
#include "Node.h"

//
//namespace gameobject {
//	class GameObjectInterface;
//}


namespace NYLibrary
{
	/// <summary>
	///	���[�g�m�[�h
	/// ���̃N���X���v���W�F�N�g�̃A�_���ƃf�l�u�ɂȂ�悤��
	/// �q��������Ă��������B
	/// ��������΂��̃N���X�̃����o���Ăяo���ƑS�ẴN���X�̃����o���Ăяo����܂��B
	/// </summary>
	class RootGameObjectNode :  public Node
	{
	public:
		RootGameObjectNode() {}
		//RootGameObjectNode& operator=(const RootGameObjectNode&) = delete;

		~RootGameObjectNode() = default;
		//������
		void LoopInitialize()override;

		void LoopCreateAddChild()override;		
		//�X�V
		void LoopUpdate()override;
		//�`��
		void LoopRender()override;
		//�I��
		void LoopFinalize()override;
		// �m�[�h�𕡐�����(�ċN�֐�)
		std::shared_ptr<NodeAbstract> Clone();

	private:
		//�q����ǉ����e�q�֌W���\�z����
		void CreateAddChild()override {}
		//������
		void Initialize()override {}
		//�X�V
		void Update()override {}
		//�`��
		void Render()override {}
		//�I��
		void Finalize()override {}

	};


	/// <summary>
	/// �Q�[���I�u�W�F�N�g�m�[�h�G���v�e�B�[
	/// ���̃N���X�͋�I�u�W�F�N�g�Ɏg�p���Ă�������
	/// ���W�A�s��@�\��������Ă��܂���B
	/// </summary>
	class GameObjectNodeEmpty : public Node
	{
	public:
		GameObjectNodeEmpty() {}
		GameObjectNodeEmpty& operator=(const GameObjectNodeEmpty&) = delete;

		~GameObjectNodeEmpty() = default;
		//�q����ǉ����e�q�֌W���\�z����
		void CreateAddChild()override {}
		//������
		void Initialize()override {}
		//�X�V
		void Update()override {}
		//�`��
		void Render()override {}
		//�I��
		void Finalize()override {}
		// �m�[�h�𕡐�����(�ċN�֐�)
		std::shared_ptr<NodeAbstract> Clone()final;
	private:
		void LoopCreateAddChild()final;
		//������
		void LoopInitialize()final;
		//�X�V
		virtual void LoopUpdate()override;
		//�`��
		void LoopRender()final;
		//�I��
		void LoopFinalize()final;

	};

	/// <summary>
	/// �Q�[���I�u�W�F�N�g�m�[�h
	/// ���W�A�s��@�\�����B���t���[���s��v�Z���܂��B
	/// </summary>
	class GameObjectNode : public GameObjectNodeEmpty
	{
	public:
		GameObjectNode() {}
		GameObjectNode& operator=(const GameObjectNode&) = delete;

		~GameObjectNode() = default;
	private:
		void LoopUpdate()final;

	};
};
