#pragma once
#include "../../NYLibrary/Component/Collider/SegmentCollider/SegmentCollider.h"

namespace NYLibrary
{

	class InkSegmentCollider :  public SegmentCollider
	{
	public:
		//コンストラクタ
		InkSegmentCollider(ObjectData* objectData);
		InkSegmentCollider() = default;
		//初期化
		void Initialize();
		void Update();
		//色を取得
		D3DXVECTOR4 GetColor() { return color; }
	private:
		D3DXVECTOR4 color;

	};
}
