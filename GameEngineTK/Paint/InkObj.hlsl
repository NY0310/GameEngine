cbuffer global
{
	matrix wvp;
	float4 color;
};

struct vsOutput
{
	float4 Pos : SV_POSITION;
};


/// <summary>
/// バーテックスシェーダー
/// </summary>
vsOutput VS(float4 Pos : POSITION)
{
	vsOutput output = (vsOutput)0;
	output.Pos = mul(Pos, wvp);
	return output;
}


float4 PS(vsOutput input) : SV_TARGET
{
	return color;
}