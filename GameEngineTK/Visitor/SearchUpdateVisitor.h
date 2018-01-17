/// <summary>
/// �X�V�m�[�h�����r�W�^�[
/// Yusuke Nakata 
/// 2017/10/09
/// </summary>


#include "VisitorInterface.h"

#include <list>


namespace scenegraph {
	class NodeAbstract;

	class SearchUpdateVisitor : public VisitorInterface, public std::enable_shared_from_this<SearchUpdateVisitor>
	{
	public:
		//�X�V�\�ȃm�[�h�̃��X�g
		using UpdateList = std::list<std::shared_ptr<NodeAbstract>>;
		//�R���X�g���N�^
		SearchUpdateVisitor();
		//�f�X�g���N�^
		~SearchUpdateVisitor();
		//�R�s�[�R���X�g���N�^�֎~
		SearchUpdateVisitor(const SearchUpdateVisitor&) = delete;
		//����֎~
		SearchUpdateVisitor& operator=(const SearchUpdateVisitor&) = delete;
		//�m�[�h�p�̌����֐�
		void Visit(std::shared_ptr<scenegraph::Node> node) override;
		//���[�t�p�̌����֐�
		void Visit(std::shared_ptr<scenegraph::Leaf> leaf) override;
		//�X�V���X�g�擾
		inline UpdateList GetUpdataList() { return updateList; }
		//�X�V���X�g����ɂ���
		inline void ClearUpdateList() { updateList.clear(); }
	private:
		//�X�V�\�ȃm�[�h�̃��X�g
		UpdateList updateList;
	};
}
