/// <summary>
/// GameObjectNode
/// 2017/10/13
/// Yusuke Nakata
/// </summary>
#pragma once
#include "Node.h"


namespace gameobject {
	class GameObjectInterface;
}


namespace scenegraph
{

	class GameObjectNode :  public Node
	{
	public:
		explicit GameObjectNode(std::shared_ptr<gameobject::GameObjectInterface>&& gameobject);
//		GameObjectNode() = default;

		GameObjectNode& operator=(const GameObjectNode&) = delete;

		~GameObjectNode() = default;
		//������
		void Initialize()override;
		//�X�V
		void Update()override;
		//�`��
		void Render()override;
		//�I��
		void Finalize()override;

		/// <summary>
		/// �m�[�h�𕡐�����(�ċN�֐�)
		/// </summary>
		/// <returns>���������m�[�h</returns>
		std::shared_ptr<NodeAbstract> Clone()override;

	private:
		std::shared_ptr<gameobject::GameObjectInterface> gameObject;
	};
}

