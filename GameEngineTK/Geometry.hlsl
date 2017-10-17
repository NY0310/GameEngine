//グローバル
Texture2D g_texColor: register(t0);
SamplerState g_samLinear : register(s0);

cbuffer global
{
	matrix g_mW;//ワールド行列
	matrix g_mWVP; //ワールドから射影までの変換行列
	float4 g_vLightDir; //ライトの方向ベクトル
	float4 g_Diffuse; //拡散反射(色）
	float4 g_Specular;//鏡面反射
	float4 g_vEye;//カメラ位置
};

//バーテックスシェーダー出力構造体
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 PosLocal : POSITION;
	float4 Normal : NORMAL;
	float2 Tex : TEXCOORD;
};
//
//バーテックスシェーダー
//
VS_OUTPUT VS( float4 Pos : POSITION ,float4 Normal : NORMAL,float2 Tex : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos=mul(Pos,g_mWVP);
	output.PosLocal =Pos;
	output.Normal=Normal;
	output.Tex=Tex;

	return output;
}
//
//ピクセルシェーダー
//
float4 PS( VS_OUTPUT input ) : SV_Target
{
	float4 PosWorld=mul(input.PosLocal,g_mW);
	float3 Normal = normalize(mul(input.Normal,(float3x3)g_mW));
	float3 LightDir = normalize(g_vLightDir);
	float3 EyeVector = g_vEye - PosWorld;
	float3 ViewDir = normalize(EyeVector); 
	float4 NL = saturate(dot(Normal, LightDir)); 
	float3 Reflect = normalize(2 * NL * Normal - LightDir); 
	float4 Specular =pow(saturate(dot(Reflect, ViewDir)),1); 

	float4 Color=(g_Diffuse * NL + Specular*g_Specular) +g_texColor.Sample( g_samLinear, input.Tex );
	return Color;
}