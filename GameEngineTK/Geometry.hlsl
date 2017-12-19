//グローバル
Texture2D g_texColor: register(t0);//シェーディングするモデルテクスチャ
Texture2D g_texInk: register(t1);//インクテクスチャ
Texture2D g_texDepth: register(t2);//深度テクスチャ

SamplerState g_samLinear : register(s0);


//グローバル
cbuffer global_0:register(b0)
{
	matrix g_mW;//ワールド行列
	matrix g_mWVP; //ワールドから射影までの変換行列
	matrix g_mWLP; //ワールド・”ライトビュー”・プロジェクションの合成
	matrix g_mWLPT; //ワールド・”ライトビュー”・プロジェクション・UV行列の合成
	float4 g_vLightDir;  //ライトの方向ベクトル
	float4 g_vEye;//カメラ位置
};

cbuffer global_1:register(b1)
{
	float4 g_Ambient = float4(0, 0, 0, 0);//アンビエント光
	float4 g_Diffuse = float4(1, 0, 0, 0); //拡散反射(色）
	float4 g_Specular = float4(1, 1, 1, 1);//鏡面反射

};

//深度テクスチャ用のグローバル
cbuffer ZTexglobal:register(b2)
{
	matrix mWLP; //ワールド・”ライトビュー”・プロジェクションの合成
};
//バーテックスシェーダー出力構造体
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR0;
	float3 Light : TEXCOORD0;
	float3 Normal : TEXCOORD1;
	float3 EyeVector : TEXCOORD2;
	float2 Tex : TEXCOORD3;
	float4 LightTexCoord : TEXCOORD4;
	float4 LighViewPos : TEXCOORD5;
	//	float4 LTexCoord : TEXCOORD4;

};


//バーテックスシェーダー出力構造体
struct VS_OUTPUT_DEPTH//深度テクスチャーレンダー用
{
	float4 Pos : SV_POSITION;
	float4 Depth :POSITION;
};



/// <summary>
/// 深度テクスチャ用バーテックスシェーダー
/// </summary>
VS_OUTPUT_DEPTH VS_Depth(float4 Pos : POSITION)
{
	VS_OUTPUT_DEPTH output = (VS_OUTPUT_DEPTH)0;
	output.Pos = mul(Pos, mWLP);
	return  output;
}


/// <summary>
/// 深度テクスチャ用ピクセルシェーダ
/// </summary>
float4 PS_Depth(VS_OUTPUT_DEPTH input) :SV_Target
{
	float4 color = input.Depth.z / input.Depth.w;
	return color;
}

//
//バーテックスシェーダー
//
VS_OUTPUT VS(float4 Pos : POSITION, float4 Norm : NORMAL, float2 Tex : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	//射影変換（ワールド→ビュー→プロジェクション）
	//法線をワールド空間に
	output.Normal = mul(Norm, (float3x3)g_mW);
	output.Pos = mul(Pos, g_mWVP);
	//ライト方向
	output.Light = g_vLightDir;
	//視線ベクトル
	float3 PosWorld = mul(Pos, g_mW);
	output.EyeVector = g_vEye - PosWorld;

	float3 Normal = normalize(output.Normal);
	float3 LightDir = normalize(output.Light);
	float3 ViewDir = normalize(output.EyeVector);
	float4 NL = saturate(dot(Normal, LightDir));

	float3 Reflect = normalize(2 * NL * Normal - LightDir);
	float4 specular = pow(saturate(dot(Reflect, ViewDir)), 4);

	output.Color = g_Diffuse * NL + specular*g_Specular;

	output.Tex = Tex;

	//ライトビューを参照するとき、手がかりとなるテクスチャ座標
	output.LightTexCoord = mul(Pos, g_mWLPT);
	//ライトビューの座標
	output.LighViewPos = mul(Pos, g_mWLP);


	return output;
}


//
//バーテックスシェーダー
//
VS_OUTPUT VS_NoTex(float4 Pos : POSITION, float4 Norm : NORMAL)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	//射影変換（ワールド→ビュー→プロジェクション）
	//法線をワールド空間に
	output.Normal = mul(Norm, (float3x3)g_mW);
	output.Pos = mul(Pos, g_mWVP);
	//ライト方向
	output.Light = g_vLightDir;
	//視線ベクトル
	float3 PosWorld = mul(Pos, g_mW);
	output.EyeVector = g_vEye - PosWorld;

	float3 Normal = normalize(output.Normal);
	float3 LightDir = normalize(output.Light);
	float3 ViewDir = normalize(output.EyeVector);
	float4 NL = saturate(dot(Normal, LightDir));

	float3 Reflect = normalize(2 * NL * Normal - LightDir);
	float4 specular = pow(saturate(dot(Reflect, ViewDir)), 4);

	output.Color = g_Diffuse * NL + specular*g_Specular;


	return output;
}

//
//ピクセルシェーダー
//
float4 PS(VS_OUTPUT input) : SV_Target
{
	float4 color;
	color = g_texInk.Sample(g_samLinear, input.Tex);
	/*if (color.a - 1 >= 0)
		color = g_texColor.Sample(g_samLinear, input.Tex);*/
	return color;
////影の処理
//input.LightTexCoord /= input.LightTexCoord.w;
//float TexValue = g_texDepth.Sample(g_samLinear, input.LightTexCoord).r;
//float LightLength = input.LighViewPos.z / input.LighViewPos.w;
//if (TexValue + 0.0001<LightLength)//ライトビューでの長さが短い（ライトビューでは遮蔽物がある）
//{
//	color /= 3;//影
//}

}

//
//ピクセルシェーダー
//
float4 PS_NoTex(VS_OUTPUT input) : SV_Target
{
	float4 color = input.Color;

	return color;
}