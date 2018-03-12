/// <summary>
/// �t(�q�������Ȃ�)
/// Nakata Yusuke
/// 2017/10/10
/// </summary>
#pragma once

#include "NodeAbstract.h"
#include "../Visitor/VisitorInterface.h"


namespace NYLibrary
{
	class Leaf : public virtual NodeAbstract ,public std::enable_shared_from_this<Leaf>
	{
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		Leaf() :NodeAbstract() {}
		/// <summary>
		/// �f�X�g���N�^
		/// l�q�����J��
		/// </summary>
		virtual ~Leaf()= default;
		Leaf& operator= (const Leaf&) = delete;
		/// <summary>
		/// ���g���폜����
		/// 
		/// �e����؂藣�������Ƃɍ폜
		/// </summary>
		inline void RemoveFromParent()override {
			parent._Get()->RemoveChild(shared_from_this());
			delete this;
		}

		/// <summary>
		/// �r�W�^�[�̎󂯓���
		/// </summary>
		/// <param name="visitor">�r�W�^�[</param>
		inline void Accept(std::shared_ptr<VisitorInterface>&& visitor)override {
			visitor->Visit(shared_from_this());
		}


		/// <summary>
		/// �m�[�h�̐����J�E���g����
		/// </summary>
		/// <param name="cnt">�m�[�h�̐�</param>
		inline void Count(int& cnt)override{ cnt++;}

	private:
		/// <summary>
		/// �ȉ��̃����o�͎q����ǉ����Ȃ��֌W��
		/// �����������Ȃ�
		/// </summary>
		

		/// <summary>
		/// �q��ǉ�����
		/// </summary>
		/// <param name="child">�q</param>
		inline void AddChild(std::shared_ptr<NodeAbstract>&& child)override {};

		/// <summary>
		/// �q��؂藣��
		/// </summary>
		inline std::shared_ptr<NodeAbstract> RemoveChild(const std::shared_ptr<NodeAbstract>& child) override { return std::shared_ptr<NodeAbstract>(nullptr);  };


		/// <summary>
		/// �q�����擾
		/// </summary>
		inline  NodeList& GetChildren()override { NodeList nodelist;  return nodelist; }

		/// <summary>
		/// �q���擾
		/// </summary>
		inline std::shared_ptr<NodeAbstract> GetChild(int childrenListIndex) const override { return std::shared_ptr<NodeAbstract>(nullptr); }

		/// <summary>
		/// �t�𕡐�����
		/// </summary>
		/// <returns>���������t</returns>
		inline std::shared_ptr<NodeAbstract> Clone()override {
			return shared_from_this();
		}

	};
}