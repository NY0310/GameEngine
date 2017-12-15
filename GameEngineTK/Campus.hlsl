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
	float4 inkUv;//インクのUV
	float inkScale;//インクテクスチャのUV座標

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
	//正規デバイス座標系に変換しUV値に対応する
//	pos.xyz /= (pos.w + 1) * 0.2f;
	
	return output;
}


/// <summary>
/// ピクセルシェーダー
/// </summary>
float4 PS(VS_OUTPUT input) : SV_Target
{
	float4 color;
	if (inkUv.x - inkScale < input.Tex.x && input.Tex.x < inkUv.x + inkScale &&
		inkUv.y - inkScale < input.Tex.y &&
		input.Tex.y < inkUv.y + inkScale)
	{
		color = g_texInk.Sample(g_samLinear, (inkUv.xy - input.Tex) / inkScale * 0.5f + 0.5f);
		if (color.a - 1 >= 0)
		{
			color = inkColor;
		}
		else
		{
			discard;
		}
	
	}
	else
	{
		discard;
	}
	return color;
}











