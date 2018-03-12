/// <summary>
/// �m�[�h
/// Yusuke Nakata
/// 2017/10/09
/// </summary>

#include "NodeAbstract.h"


namespace NYLibrary
{
	class Node : public virtual NodeAbstract,public std::enable_shared_from_this<Node>
	{
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		Node();

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		virtual ~Node();

		/// <summary>
		/// �R�s�[�R���X�g���N�^�֎~
		/// </summary>
		//Node(const Node&) = delete;

		/// <summary>
		/// ����֎~
		/// </summary>
		/// <param name="">�m�[�h</param>
		/// <returns>�m�[�h</returns>
		Node& operator=(const Node&) = delete;

		/// <summary>
		/// �q��ǉ�����
		/// </summary>
		/// <param name="child">�q</param>
		inline void AddChild(std::shared_ptr<NodeAbstract>&& child)override
		{
			//�e��ݒ�
			child->SetParent(shared_from_this());
			children.emplace_back(child);
		}

		/// <summary>
		/// �q��؂藣��(�A�h���X�͍폜���Ȃ�)
		/// </summary>
		/// <param name="child">�폜����q</param>
		/// <returns>�폜�����q�܂���nullptr</returns>
		std::shared_ptr<NodeAbstract> RemoveChild(const std::shared_ptr<NodeAbstract>& child)override;

		/// <summary>
		/// ���M���폜(�e����؂藣��������)
		/// </summary>
		void RemoveFromParent()override {
			parent._Get()->RemoveChild(shared_from_this());
		}

		/// <summary>
		/// �q�����擾����
		/// </summary>
		/// <returns>�q���̃A�h���X�̃��X�g</returns>
		NodeList& GetChildren()override { return children; }

		/// <summary>
		/// �q���擾
		/// </summary>
		/// <param name="childrenListIndex">�q�����X�g�̗v�f�ʒu</param>
		/// <returns>�q�̃A�h���X</returns>
		std::shared_ptr<NodeAbstract> GetChild(int childrenListIndex)const override { return children[childrenListIndex]; }

		/// <summary>
		/// �r�W�^�[�̎󂯓���
		/// </summary>
		/// <param name="visitor">�g�p����r�W�^�[</param>
		void Accept(std::shared_ptr<VisitorInterface>&& visitor)override;

		/// <summary>
		/// �m�[�h�̐����J�E���g����(�ċN�֐�)
		/// </summary>
		/// <param name="cnt">�m�[�h�̐�</param>
		void Count(int& cnt)override;

		///// <summary>
		///// �m�[�h�𕡐�����(�ċN�֐�)
		///// </summary>
		///// <returns>���������m�[�h</returns>
		//std::shared_ptr<NodeAbstract> Clone()override;
	protected:
		//�q��
		NodeList children;
	};

};