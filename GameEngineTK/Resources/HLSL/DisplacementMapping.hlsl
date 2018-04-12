Texture2D g_NormalTexture: register(t0);
Texture2D g_SnowHeightTexture: register(t1);
SamplerState g_samPoint : register(s0);

cbuffer CONSTANT : register(b0)
{
	matrix g_mWVP : packoffset(c0);
	matrix g_mW : packoffset(c4);
	float3 g_vEyePos : packoffset(c8);
	float g_fMinDistance : packoffset(c9);
	float g_fMaxDistance : packoffset(c10);
	int g_iMaxDevide : packoffset(c11);
	float4 g_LightDir : packoffset(c12);
	float4 g_SnowGlow : packoffset(c13);//雪山の成長
};

struct VS_OUTPUT
{
	float3 pos : POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL;
};

struct HS_CONSTANT_OUTPUT
{
	float factor[3]  : SV_TessFactor;
	float inner_factor : SV_InsideTessFactor;
};

struct HS_OUTPUT
{
	float3 pos  : POSITION;
	float2 uv :TEXCOORD0;
};

struct DS_OUTPUT
{
	float4 pos  : SV_POSITION;
	float2 uv:TEXCOORD0;
};

//
//
//
VS_OUTPUT VS(float3 pos :POSITION, float2 uv : TEXCOORD0, float3 normal : NORMAL)
{
	VS_OUTPUT Out;
	Out.pos = pos;
	Out.uv = uv;
	Out.normal = normal;

	return Out;
}
//
//
//ハルシェーダーコンスタント
HS_CONSTANT_OUTPUT HSConstant(InputPatch<VS_OUTPUT, 3> ip, uint pid : SV_PrimitiveID)
{
	HS_CONSTANT_OUTPUT Out;
	float divide = 0;

	float distance = length(g_vEyePos - ip[0].pos);
	if (distance<g_fMinDistance) distance = g_fMinDistance;
	if (distance>g_fMaxDistance) distance = g_fMaxDistance;

	float x = (distance - g_fMinDistance) / (g_fMaxDistance - g_fMinDistance);
	divide = (1 - x)*g_iMaxDevide + 1;

	divide = 12;//つなぎ目が目立つときは分割数は固定

	Out.factor[0] = divide;
	Out.factor[1] = divide;
	Out.factor[2] = divide;

	Out.inner_factor = divide;

	return Out;
}
//
//
//ハルシェーダー
[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("HSConstant")]
HS_OUTPUT HS(InputPatch<VS_OUTPUT, 3> ip, uint cpid : SV_OutputControlPointID, uint pid : SV_PrimitiveID)
{
	HS_OUTPUT Out;
	Out.pos = ip[cpid].pos;
	Out.uv = ip[cpid].uv;

	return Out;
}
//
//
//
[domain("tri")]
DS_OUTPUT DS(HS_CONSTANT_OUTPUT In, float3 UV : SV_DomaInLocation, const OutputPatch<HS_OUTPUT, 3> patch)
{
	DS_OUTPUT Out;

	float2 uv = patch[0].uv*UV.x + patch[1].uv*UV.y + patch[2].uv*UV.z;

	Out.uv = uv;

	float4 height = g_NormalTexture.SampleLevel(g_samPoint, uv, 0) / 12;//細かいバンプのハイト
	height += g_SnowHeightTexture.SampleLevel(g_samPoint, uv, 0)*g_SnowGlow.x / 2;//雪が積もっている部分を高くするハイト

	float3 pos = patch[0].pos*UV.x + patch[1].pos*UV.y + patch[2].pos*UV.z;
	Out.pos = float4(pos, 1);
	Out.pos.y += (height.x + height.y + height.z) / 3;
	Out.pos = mul(Out.pos, g_mWVP);

	return Out;
}
//
//
//
float4 PS(DS_OUTPUT input) : SV_Target
{
	float4 PosWorld = mul(input.pos,g_mW);
	float4 Normal = g_NormalTexture.SampleLevel(g_samPoint,input.uv,0) * 2 - 1;
	float3 LightDir = normalize(g_LightDir).xyz;
	float3 EyeVector = g_vEyePos - PosWorld;
	float3 ViewDir = normalize(EyeVector);
	float4 NL = saturate(dot(Normal, LightDir));
	float3 Reflect = normalize(2 * NL * Normal - LightDir);
	float4 Specular = pow(saturate(dot(Reflect, ViewDir)),32);

	return float4(0.4,0.4,0.5,1) + float4(0.8,0.85,1.0,1)*NL + float4(1,1,1,1)*Specular;
}

