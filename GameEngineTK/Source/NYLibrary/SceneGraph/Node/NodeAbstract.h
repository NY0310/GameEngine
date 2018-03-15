/// <summary>
/// NodeAbstract
/// Node��Leaf���p�����钊�ۃN���X
/// Yusuke Nakata
/// 2017/10/13
/// </summary>
#pragma once

#include <memory> // std::unique_ptr
#include <vector> // std::vector
#include "../../MatrixObject/MatrixObject.h"

namespace NYLibrary
{
	class Object : public MatrixObject{

	};

	class VisitorInterface;
	class NodeAbstract: public Object
	{
	public:
		//�m�[�h�̃��X�g
		using NodeList = std::vector<std::shared_ptr<NodeAbstract>>;
		//�R���X�g���N�^
		NodeAbstract();
		//�f�X�g���N�^
		virtual ~NodeAbstract() = default;
		//�R�s�[�R���X�g���N�^�֎~
		//NodeAbstract(const NodeAbstract&) = delete;
		//����֎~
		NodeAbstract& operator=(const NodeAbstract&) = delete;
		//�q����ǉ�
		virtual void LoopCreateAddChild() = 0;
		//������
		virtual void LoopInitialize() = 0;
		//�X�V
		virtual void LoopUpdate() = 0;
		//�`��
		virtual void LoopRender() = 0;
		//�I��
		virtual void LoopFinalize() = 0;
		//�q����ǉ����e�q�֌W���\�z����
		virtual void CreateAddChild() = 0;
		//������
		virtual void Initialize() = 0;
		//�X�V
		virtual void Update() = 0;
		//�`��
		virtual void Render() = 0;
		//�I��
		virtual void Finalize() = 0;

		//�q��ǉ�
		virtual void AddChild(std::shared_ptr<NodeAbstract>&& child) = 0;
		//�q��؂藣��
		virtual std::shared_ptr<NodeAbstract> RemoveChild(const std::shared_ptr<NodeAbstract>& child) = 0;
		//�e����؂藣�������Ǝ��M���폜
		virtual void RemoveFromParent() = 0;
		//�q�����擾����
		virtual NodeList& GetChildren() = 0;
		//�q���擾����
		virtual std::shared_ptr<NodeAbstract> GetChild(int children)const =0;
		//�r�W�^�[�̎󂯓���
		virtual void Accept(std::shared_ptr<VisitorInterface>&& visitor) = 0;
		//�e�̃A�h���X���擾
		std::weak_ptr<NodeAbstract> Getparent()const { return parent; }
		//�e�̃A�h���X��ݒ肷��
		inline void SetParent(const std::shared_ptr<NodeAbstract>& parent) { this->parent = parent; }
		//�X�V�\��
		bool CanUpdate() { return canUpdate;}
		//�X�V�\�ɂ���
		inline void EnableUpdate() { canUpdate = true; }
		//�X�V�s�ɂ���
		inline void DisableUpdate() { canUpdate = false;}
		//�m�[�h�̐����擾����
		int GetSize();
		//�m�[�h�̐����J�E���g����(�ċN�֐�)
		virtual void Count(int& cnt) = 0;
		//�m�[�h�𕡐�����
		virtual std::shared_ptr<NodeAbstract> Clone() = 0;
	protected:
		//�e�ւ̃|�C���^
		std::weak_ptr<NodeAbstract> parent;
		//�X�V�ł��邩
		bool canUpdate;
	};
};

