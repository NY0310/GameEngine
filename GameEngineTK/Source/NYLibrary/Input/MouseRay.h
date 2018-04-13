//
//#pragma once
//
//#include <d3d11_1.h>
//#include <SimpleMath.h>
//#include "../Collision/Collision.h"
//#include "MouseUtil.h"
//#include "../Camera/FollowCamera.h"
//
//namespace NYLibrary
//{
//	class MouseRay {
//	public:
//		static MouseRay* GetInstance();//インスタンスのアドレスを取得する
//		Segment* RayCreate();//レイ生成
//		static std::unique_ptr<MouseRay> mouseRay;
//	private:
//		MouseRay();
//		using Vector3 = DirectX::SimpleMath::Vector3;
//		using Matrix = DirectX::SimpleMath::Matrix;
//		Vector3 * CalcScreenToXZ(Vector3 * pout, int Sx, int Sy, int Screen_w, int Screen_h, Matrix * View, Matrix * Prj);
//		Vector3 * CalcScreenToWorld(Vector3 * pout, float Sx, float Sy, float fZ, float Screen_w, float Screen_h, Matrix * View, Matrix * Prj);
//	};
//};