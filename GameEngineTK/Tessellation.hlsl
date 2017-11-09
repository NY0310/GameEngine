//構造体
cbuffer CONSTANT : register(b0)
{
	matrix g_mWVP;
};

struct VS_OUTPUT
{
	float3 pos : POSITION;
};

struct HS_CONSTANT_OUTPUT
{
	float factor[3]  : SV_TessFactor;
	float inner_factor : SV_InsideTessFactor;
};

struct HS_OUTPUT
{
	float3 pos  : POSITION;
};

struct DS_OUTPUT
{
	float4 pos : SV_POSITION;
};

//
//
//バーテックスシェーダー
VS_OUTPUT VS(float3 pos :POSITION)
{
	VS_OUTPUT Out;
	Out.pos = pos;
	return Out;
}
//
//
//ハルシェーダーコンスタント
HS_CONSTANT_OUTPUT HSConstant(InputPatch<VS_OUTPUT, 3> ip, uint pid : SV_PrimitiveID)
{
	HS_CONSTANT_OUTPUT Out;

	float devide = 5;

	Out.factor[0] = devide;
	Out.factor[1] = devide;
	Out.factor[2] = devide;

	Out.inner_factor = devide;

	return Out;
}
//
//
//ハルシェーダー
[domain("tri")]//ドメインタイプ
[partitioning("integer")]//分割数を整数か小数点数
[outputtopology("triangle_cw")]//最終的な分割後ポリゴンについて
//triangle_cw　時計回りのトライアングルか、
//triangle_ccw 反時計回りのトライアングル
//line 線ポリゴンかを指定できる	
[outputcontrolpoints(3)]//出力コントロールポイントの数。
[patchconstantfunc("HSConstant")]//パッチ定義関数名指定
HS_OUTPUT HS(InputPatch<VS_OUTPUT, 3> ip, uint cpid : SV_OutputControlPointID, uint pid : SV_PrimitiveID)
{
	HS_OUTPUT Out;
	Out.pos = ip[cpid].pos;
	return Out;
}
//
//
//ドメインシェーダー
[domain("tri")]
DS_OUTPUT DS(HS_CONSTANT_OUTPUT In, float3 UV : SV_DomaInLocation, const OutputPatch<HS_OUTPUT, 3> patch)
{
	DS_OUTPUT Out;

	//float3 top_u = lerp(patch[0].pos, patch[1].pos, UV.x);
	//float3 bottom_u = lerp(patch[3].pos, patch[2].pos, UV.x);
	//Out.pos = float4(lerp(top_u, bottom_u, UV.y), 1);


	float3 pos = patch[0].pos*UV.x + patch[1].pos*UV.y + patch[2].pos*UV.z;
	Out.pos = float4(pos, 1);
	Out.pos = mul(Out.pos, g_mWVP);
	return Out;
}
//
//
//ピクセルシェーダー
float4 PS(DS_OUTPUT In) : SV_Target
{
	return float4(1,1,1,1);
}