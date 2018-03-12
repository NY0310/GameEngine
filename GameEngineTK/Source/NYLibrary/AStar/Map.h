#pragma once


#include <d3d11_1.h>
#include <DirectXMath.h>
#include "AttributeMap.h"
#include "../Obj/Obj.h"
#include "AStar.h"
#include "../SceneGraph/Node/GameObjectNode.h"

namespace NYLibrary
{
	class Map :public GameObjectNodeEmpty
	{
	public:
		Map() = default;
		~Map() = default;
		Map(const Map&) = delete;
		Map& operator=(const Map&) = delete;
		//������
		void Initialize();
		//�`��
		void Render();
		//�ŒZ�o�H�擾
		std::vector<DirectX::SimpleMath::Vector2> GetShortestPath() { aStar->SearchRoute(); return aStar->GetResultPath(); }
	private:
		//�}�b�v�̃��X�g�T�C�Y�ύX
		void MapObjResize(const int Hight, const int windth);
		//�}�b�v����
		void CreateMap();
		//�z�u����I�u�W�F�N�g
		std::vector<std::vector<std::shared_ptr<Obj>>> mapObj;
		//�A�g���r���[�g�}�b�v
		std::shared_ptr<AttributeMap> attributeMap;
		//A*
		std::unique_ptr<AStar>aStar;
	};
};
