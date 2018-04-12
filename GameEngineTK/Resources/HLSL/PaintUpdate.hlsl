Texture2D upDateTexure : register(t0);
SamplerState samLinear : register(s0);

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
};



/// <summary>
/// バーテックスシェーダー
/// </summary>
VS_OUTPUT VS(float4 Pos : POSITION, float2 Tex : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Tex = Tex;
	output.Pos = Pos;
	return output;
}


float4 PS(VS_OUTPUT input) : SV_TARGET
{
	return upDateTexure.Sample(samLinear, input.Tex);
}