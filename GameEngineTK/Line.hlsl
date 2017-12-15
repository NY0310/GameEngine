//グローバル
cbuffer global
{
	matrix g_WVP; //ワールドから射影までの変換行列
};
//構造体
struct PS_INPUT
{
	float4 Pos : SV_POSITION;
};

//
//
//バーテックスシェーダー
PS_INPUT VS(float4 Pos : POSITION)
{
	PS_INPUT Out;
	Out.Pos = mul(Pos, g_WVP);
	return Out;
}
//
//
//ピクセルシェーダー
float4 PS(PS_INPUT Input) : SV_Target
{
	return float4(1,1,1,1);
}