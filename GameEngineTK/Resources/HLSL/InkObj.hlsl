//cbuffer global
//{
//	matrix wvp;
//	float4 color;
//};
//
//struct vsOutput 
//{
//	float4 Pos : SV_POSITION;
//};
//
//
///// <summary>
///// バーテックスシェーダー
///// </summary>
//vsOutput VS(float4 Pos : POSITION)
//{
//	vsOutput output = (vsOutput)0;
//	output.Pos = mul(Pos, wvp);
//	return output;
//}
//
//
//float4 PS(vsOutput input) : SV_TARGET
//{
//	return color;
//}



//グローバル
cbuffer global_0:register(b0)
{
	matrix g_mWVP; //ワールドから射影までの変換行列
	float4 color;
};

//バーテックスシェーダー出力構造体
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float3 Normal : TEXCOORD1;
	float2 Tex : TEXCOORD3;
};






//
//バーテックスシェーダー
//
VS_OUTPUT VS(float4 Pos : POSITION)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	//射影変換（ワールド→ビュー→プロジェクション）
	//法線をワールド空間に
	output.Pos = mul(Pos, g_mWVP);


	return output;
}




//
//ピクセルシェーダー
//
float4 PS(VS_OUTPUT input) : SV_Target
{
	return color;
}
