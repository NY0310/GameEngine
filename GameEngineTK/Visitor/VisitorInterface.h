/// <summary>
/// �r�W�^�[�N���X
/// Yusuke Nakata
/// 2017/10/13
/// </summary>
#pragma once

#include <memory>

namespace scenegraph
{
	class Node;
	class Leaf;
	//�r�W�^�[�̃C���^�[�t�F�C�X
	class VisitorInterface
	{
	public:
		//�R���X�g���N�^
		VisitorInterface() = default;
		//�f�X�g���N�^
		~VisitorInterface() = default;
		//�R�s�[�R���X�g���N�^�g�p�s��
		VisitorInterface(const VisitorInterface&) = delete;
		//����֎~
		VisitorInterface& operator=(const VisitorInterface&) = delete;
		//�m�[�h��p�̌����֐�
		virtual void Visit(std::shared_ptr<scenegraph::Node> node) = 0;
		//���[�t��p�̌����֐�
		virtual void Visit(std::shared_ptr<scenegraph::Leaf> leaf) = 0;

	};
}


