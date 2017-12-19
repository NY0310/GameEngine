/// <summary>
///	オブジェクトに貼るインクをテクスチャを作成
/// </summary>

//グローバル
Texture2D g_texInk: register(t0);//インクテクスチャ

SamplerState g_samLinear : register(s0);
//グローバル



cbuffer global_0:register(b0)
{
	float4 inkColor;//インクの基本色
};


struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
};


/// <summary>
/// バーテックスシェーダー
/// </summary>
VS_OUTPUT VS(float4 Pos : POSITION ,float2 Tex : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Tex = Tex;
	output.Pos =  Pos;	
	return output;
}


/// <summary>
/// ピクセルシェーダー
/// </summary>
float4 PS(VS_OUTPUT input) : SV_Target
{
	float4 color;
	color = g_texInk.Sample(g_samLinear,input.Tex);
	if (color.a - 1 >= 0)
	{
		color = inkColor;
	}
	else
	{
		discard;
	}
	

	return color;
}











