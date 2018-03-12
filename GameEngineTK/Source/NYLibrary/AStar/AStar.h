#pragma once

#include <d3d11_1.h>
#include <SimpleMath.h>
#include <vector>
#include <list>
#include <memory>

#include "MapNode.h"
#include "AttributeMap.h"

namespace NYLibrary
{
	class AStar
	{
	public:
		//�R���X�g���N�^
		AStar();
		//������
		void Initialize();
		//�o�H�T��
		void SearchRoute();
		//�������̍폜
		void Clear();
		//�A�g���r���[�g�}�b�v��ݒ�
		void SetAttributeMap(const std::shared_ptr<AttributeMap>& attributemap) { m_attributeMap = attributemap; }
		//�T�����ʂ��擾����
		const std::vector<DirectX::SimpleMath::Vector2>& GetResultPath() { return resultPath; }
	private:
		//�}�b�v
		std::vector<std::vector<MapNode*>> m_map;
		//�I�[�v�����X�g
		std::list<MapNode*> m_openList;
		//�I�[�v�����X�g�C�e���[�^
		std::list<MapNode*>::iterator m_openListIterator;
		//�N���[�Y�h���X�g
		std::list<MapNode*> m_closedList;
		//�ŏ��X�R�A�m�[�h�C�e���[�^
		std::list<MapNode*>::iterator m_minNodeIterator;
		//�A�g���r���[�g�}�b�v
		std::shared_ptr<AttributeMap> m_attributeMap;
		//�I���m�[�h
		MapNode* endNode;
		//���ʌo�H
		std::vector<DirectX::SimpleMath::Vector2>resultPath;
		//�}�b�v�̐ݒ�
		void SetMap();
		//�}�b�v�̑����̕\��
		void PrintMapAttr();
		//�I�[�v�����X�g�ɒǉ�
		void AddOpenList();
		//���ʌo�H�쐬
		void CreateResultPath();

		//�ǂ��ǂ���
		bool isWall(int i, int j);
	};
};