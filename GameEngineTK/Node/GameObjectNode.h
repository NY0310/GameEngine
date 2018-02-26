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
		RootGameObjectNode() = default;

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
		//std::shared_ptr<gameobject::GameObjectInterface> gameObject;
	};
	class GameObjectNode : public Node
	{
	public:
		//explicit GameObjectNode();
		GameObjectNode() = default;

		//GameObjectNode& operator=(const GameObjectNode&) = delete;

		~GameObjectNode() = default;
		//������
		void Initialize()override {}
		//�X�V
		void Update()override {}
		//�`��
		void Render()override {}
		//�I��
		void Finalize()override {}

		/// <summary>
		/// �m�[�h�𕡐�����(�ċN�֐�)
		/// </summary>
		/// <returns>���������m�[�h</returns>
		std::shared_ptr<NodeAbstract> Clone()override;

	private:
		//������
		void LoopInitialize()override;
		//�X�V
		void LoopUpdate()override;
		//�`��
		void LoopRender()override;
		//�I��
		void LoopFinalize()override;

	};
}
