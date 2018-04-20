#pragma once
#include "../../NYLibrary/Component/Collider/SegmentCollider/SegmentCollider.h"

namespace NYLibrary
{

	class InkSegmentCollider :  public SegmentCollider
	{
	public:
		//�R���X�g���N�^
		InkSegmentCollider(ObjectData* objectData);
		InkSegmentCollider() = default;
		//������
		void Initialize();
		void Update();
		//�F���擾
		D3DXVECTOR4 GetColor() { return color; }
	private:
		D3DXVECTOR4 color;

	};
}
