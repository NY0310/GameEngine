/// <summary>
/// ライト
/// </summary>
#pragma once
#include <D3DX10.h>


class Light
{
public:
	//コンストラクタ
	Light(D3DXVECTOR3 eyeposition);
	//デストラクタ
	~Light() = default;
	//引数なしのコンストラクタ禁止
	Light() = delete;
	//コピーコンストラクタ禁止
	Light(const Light&) = delete;
	//代入禁止
	Light& operator=(const Light&) = delete;
	//ライトビューを取得
	const D3DXMATRIX GetView() { return view; }
private:
	D3DXMATRIX view;//ビュー
	D3DXVECTOR3 eyePosition;//視点
	D3DXVECTOR3 refPosition;//注視点
	D3DXVECTOR3 upVector;//上方向ベクトル
};
