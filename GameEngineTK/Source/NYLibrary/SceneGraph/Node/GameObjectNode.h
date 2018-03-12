/// <summary>
/// GameObjectNode
/// 2017/10/13
/// Yusuke Nakata
/// </summary>
#pragma once
#include "Node.h"
#include "../../MatrixObject/MatrixObject.h"

//
//namespace gameobject {
//	class GameObjectInterface;
//}


namespace NYLibrary
{

	class RootGameObjectNode :  public Node
	{
	public:
		//explicit GameObjectNode(tInterface>&& gameobject);

		RootGameObjectNode& operator=(const RootGameObjectNode&) = delete;

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

		/// <summary>
		/// �m�[�h�𕡐�����(�ċN�֐�)
		/// </summary>
		/// <returns>���������m�[�h</returns>
		std::shared_ptr<NodeAbstract> Clone()override;

	private:
		//�q����ǉ����e�q�֌W���\�z����
		void CreateAddChild()override {};
		//������
		void Initialize()override {}
		//�X�V
		void Update()override {}
		//�`��
		void Render()override {}
		//�I��
		void Finalize()override {}

	};
	class GameObjectNodeEmpty : public NYLibrary::Node
	{
	public:
		//explicit GameObjectNode();
		GameObjectNodeEmpty() {}
		GameObjectNodeEmpty& operator=(const GameObjectNodeEmpty&) = delete;

		~GameObjectNodeEmpty() = default;
		//�q����ǉ����e�q�֌W���\�z����
		void CreateAddChild()override {};
		//������
		void Initialize()override {}
		//�X�V
		void Update()override {}
		//�`��
		void Render()override {}
		//�I��
		void Finalize()override {}
		void LoopCreateAddChild()override;
		//������
		void LoopInitialize()override;
		//�X�V
		virtual void LoopUpdate()override;
		//�`��
		void LoopRender()override;
		//�I��
		void LoopFinalize()override;

		/// <summary>
		/// �m�[�h�𕡐�����(�ċN�֐�)
		/// </summary>
		/// <returns>���������m�[�h</returns>
		std::shared_ptr<NodeAbstract> Clone()override;

	private:

	};

	class GameObjectNode : public NYLibrary::GameObjectNodeEmpty,public NYLibrary::MatrixObject
	{
	public:
		GameObjectNode() {}
		//explicit GameObjectNode();

		GameObjectNode& operator=(const GameObjectNode&) = delete;

		~GameObjectNode() = default;
		//�q����ǉ����e�q�֌W���\�z����
		void CreateAddChild()override {};
		//�X�V
		void LoopUpdate()override;

	private:

	};

};
