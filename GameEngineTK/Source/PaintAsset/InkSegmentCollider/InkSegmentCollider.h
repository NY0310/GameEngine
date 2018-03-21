#pragma once
#include "../../NYLibrary/Component/Collider/SegmentCollider/SegmentCollider.h"

namespace NYLibrary
{
	class InkSegmentCollider :  public SegmentCollider
	{
	public:
		//�R���X�g���N�^
		InkSegmentCollider(const std::string& tag, ObjectData* matrixObject);
		//������
		void Initialize();
		void Update() { SegmentCollider::Update(); }
		//�F���擾
		D3DXVECTOR4 GetColor() { return color; }
	private:
		D3DXVECTOR4 color;
	};
}
