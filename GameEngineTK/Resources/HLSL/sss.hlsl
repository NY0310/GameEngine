cbuffer GLOBAL
{
	matrix g_mW: packoffset(c0);//c(n) nは16バイト単位
	matrix g_mWVP: packoffset(c4);
	matrix g_mWLPT: packoffset(c8);
	matrix g_mWLP: packoffset(c12);
	float4 g_vLightPos: packoffset(c16);//ライトの方向ベクトル
	float4 g_Ambient: packoffset(c17);//アンビエント光
	float4 g_Diffuse: packoffset(c18); //拡散反射
	float4 g_Specular: packoffset(c19);//鏡面反射
	float3 g_vEye: packoffset(c20);
	float g_Transparent : packoffset(c21);//どこまでの長さなら透過するか
};

Texture2D g_texDepth;
SamplerState g_samLinear;
//バーテックスシェーダー出力構造体
struct VS_OUT
{
	float4 Pos : SV_POSITION;
};
//構造体
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float3 Light : TEXCOORD0;
	float3 Normal : TEXCOORD1;
	float3 EyeVector : TEXCOORD2;
	float LightOut : LIGHT_OUT;
	float4 texcoord :TEXCOORD3;
};

//
//深度テクスチャ作成　バーテックスシェーダー
//
VS_OUT VS_Depth(float4 Pos : POSITION)
{
	VS_OUT output = (VS_OUT)0;
	output.Pos = mul(Pos, g_mWLP);

	return output;
}

//
//深度テクスチャ作成　ピクセルシェーダー
//
float4 PS_Depth(VS_OUT input) : SV_Target
{
	return float4(input.Pos.w,0,0,1);//R32フォーマットなので赤以外は不使用
}

//
//SSS　バーテックスシェーダー
//
VS_OUTPUT VS(float4 Pos : POSITION, float4 Normal : NORMAL)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.Pos = mul(Pos, g_mWVP);
	output.Normal = mul(Normal, (float3x3)g_mWVP);
	float3 PosWorld = mul(Pos, g_mW);
	output.EyeVector = g_vEye - PosWorld;
	output.Light = normalize(g_vLightPos - PosWorld);

	float4 texcoord = mul(Pos, g_mWLPT);	//この点が、ライトビューであったときの位置がわかる
	output.texcoord = texcoord;
	float4 Plight = mul(Pos, g_mWLP);
	float LightOut = Plight.w;//ライト出口
	output.LightOut = LightOut;

	return output;
}

//
//SSS　ピクセルシェーダー
//
float4 PS(VS_OUTPUT input) : SV_Target
{
	float4 color = float4(1,0,0,1);
	//フォンシェーディング
	float3 Normal = normalize(input.Normal);
	float3 LightVector = normalize(input.Light);
	float3 EyeVector = normalize(input.EyeVector);
	float NdotL = dot(Normal, LightVector);
	float3 Reflect = normalize(2 * NdotL*Normal - LightVector);
	float specular = pow(saturate(dot(Reflect, EyeVector)), 8);
	float4 phong = g_Diffuse * saturate(NdotL) + 4 * specular*g_Specular;
	//メッシュ、当該地点の厚みを求める
	float LightOut = input.LightOut;
	float LightIn = 0;
	input.texcoord.xyz /= input.texcoord.w;
	LightIn = g_texDepth.Sample(g_samLinear, input.texcoord).r;
	float thickness = abs(LightOut - LightIn);
	//透過する光	
	float4 transparent = g_Diffuse*(1.0f - saturate(thickness / g_Transparent));
	
	color = phong  * 4 + 2 * transparent*3.0f / 4.0f;//フォンシェーディングの色に透過光をプラス

	return color;
}