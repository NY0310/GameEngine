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


namespace scenegraph
{

	class RootGameObjectNode :  public Node
	{
	public:
		//explicit GameObjectNode(tInterface>&& gameobject);

		RootGameObjectNode& operator=(const RootGameObjectNode&) = delete;

		~RootGameObjectNode() = default;
		//������
		void LoopInitialize()override;
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


		//std::shared_ptr<gameobject::GameObjectInterface> gameObject;
	};
	class GameObjectNode : public Node
	{
	public:
		//explicit GameObjectNode();

		GameObjectNode& operator=(const GameObjectNode&) = delete;

		~GameObjectNode() = default;
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
		//������
		void LoopInitialize()override;
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

	};
}
