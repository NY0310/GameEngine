Texture2D texureResouce : register(t0);
SamplerState samLinear : register(s0);

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
};

cbuffer global:register(b0)
{
	matrix wvp : register(c0); //ワールドから射影までの変換行列
	float4 color :register(c4);
	float transparency : register(c8);
};

cbuffer global2:register(b1)
{
	matrix world : register(c0); //ワールドから射影までの変換行列
	float viewPortWidth : register(c4);//ビューポートX
	float viewPortHeight :register(c5);//ビューポートY
};

/// <summary>
/// 3D用バーテックスシェーダー
/// </summary>
VS_OUTPUT VS3D(float4 pos : POSITION, float2 tex : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.tex = tex;
	pos = mul(pos, wvp);
	output.pos = pos;
	return output;
}


/// <summary>
/// 2D用バーテックスシェーダー
/// </summary>
VS_OUTPUT VS2D(float4 pos : POSITION, float2 tex : TEXCOORD)
{
	//VS_OUTPUT output = (VS_OUTPUT)0;
	//output.tex = tex;
	//output.pos = pos;
	//return output;
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.tex = tex;
	output.pos = mul(pos, world);
	output.pos.x = (output.pos.x / viewPortWidth) * 2 - 1;
	output.pos.y = 1 - (output.pos.y / viewPortHeight) * 2;
	return output;

}

/// <summary>
/// ピクセルシェーダ
/// </summary>
float4 PS(VS_OUTPUT input) : SV_TARGET
{
	float4 outputColor;
	float4 texColor = texureResouce.Sample(samLinear, input.tex);
	if (color.w)
	{
		outputColor = color;
	}
	else
	{
		outputColor = texColor;
	}
	if (texColor.a <= 0)
	{
		discard;
	}
	return float4(outputColor.r, outputColor.g, outputColor.b, transparency);
}