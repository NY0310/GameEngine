//定義
#define MAX_BONE_MATRICES 255

//グローバル
Texture2D g_texColor: register(t0);//テクスチャーは レジスターt(n)
Texture2D texInk: register(t1);//インクテクスチャ
SamplerState g_samLinear : register(s0);//サンプラーはレジスターs(n)

cbuffer global_0:register(b0)
{
	float4 g_vLight;  //ライトの方向ベクトル
	float4 g_vEye;//カメラ位置
};

cbuffer global_1:register(b1)
{
	matrix g_mW;//ワールド行列
	matrix g_mWVP; //ワールドから射影までの変換行列
	float4 g_Ambient=float4(0,0,0,0);//アンビエント光
	float4 g_Diffuse=float4(1,0,0,0); //拡散反射(色）
	float4 g_Specular=float4(1,1,1,1);//鏡面反射
};

cbuffer global_bones:register(b2)//ボーンのポーズ行列が入る
{
	matrix g_mConstBoneWorld[MAX_BONE_MATRICES];
};

//スキニング後の頂点・法線が入る
struct Skin
{
	float4 Pos;
	float3 Norm;
};
//バーテックスバッファーの入力
struct VSSkinIn
{
	float3 Pos	: POSITION;//位置   
	float3 Norm : NORMAL;//頂点法線
	float2 Tex	: TEXCOORD;//テクスチャー座標
	uint4  Bones : BONE_INDEX;//ボーンのインデックス
	float4 Weights : BONE_WEIGHT;//ボーンの重み
};
//ピクセルシェーダーの入力（バーテックスバッファーの出力）　
struct PSSkinIn
{
	float4 Pos	: SV_Position;//位置
	float3 Norm : NORMAL;//頂点法線
	float2 Tex	: TEXCOORD;//テクスチャー座標
	float4 Color : COLOR0;//最終カラー（頂点シェーダーにおいての）
};

//
//matrix FetchBoneMatrix( uint iBone )
//指定した番号のボーンのポーズ行列を返す　サブ関数（バーテックスシェーダーで使用）
matrix FetchBoneMatrix( uint iBone )
{
	return g_mConstBoneWorld[iBone];
}

//
// Skin SkinVert( VSSkinIn Input )
//頂点をスキニング（ボーンにより移動）する。サブ関数（バーテックスシェーダーで使用）
Skin SkinVert( VSSkinIn Input )
{
	Skin Output = (Skin)0;

	float4 Pos = float4(Input.Pos,1);
	float3 Norm = Input.Norm;
	//ボーン0
	uint iBone=Input.Bones.x;
	float fWeight=Input.Weights.x;
	matrix m=FetchBoneMatrix(iBone);
	Output.Pos+=fWeight * mul(Pos,m);
	Output.Norm+=fWeight * mul(Norm,(float3x3)m);
	//ボーン1
	iBone=Input.Bones.y;
	fWeight=Input.Weights.y;
	m=FetchBoneMatrix(iBone);
	Output.Pos+=fWeight*mul(Pos,m);
	Output.Norm+=fWeight*mul(Norm,(float3x3)m);
	//ボーン2
	iBone=Input.Bones.z;
	fWeight=Input.Weights.z;
	m=FetchBoneMatrix(iBone);
	Output.Pos+=fWeight*mul(Pos,m);
	Output.Norm+=fWeight*mul(Norm,(float3x3)m);
	//ボーン3
	iBone=Input.Bones.w;
	fWeight=Input.Weights.w;
	m=FetchBoneMatrix(iBone);
	Output.Pos+=fWeight*mul(Pos,m);
	Output.Norm+=fWeight*mul(Norm,(float3x3)m);

	return Output;
}
//
//PSSkinIn VSSkin(VSSkinIn input )
//バーテックスシェーダー
PSSkinIn VSSkin(VSSkinIn input )
{
	PSSkinIn output;
	Skin vSkinned = SkinVert( input);

	output.Pos = mul( vSkinned.Pos, g_mWVP );
	output.Norm = normalize( mul( vSkinned.Norm, (float3x3)g_mW ) );
	output.Tex = input.Tex;
	float3 LightDir = normalize(g_vLight);
	float3 PosWorld = mul(vSkinned.Pos,g_mW);
	float3 ViewDir = normalize(g_vEye-PosWorld);
	float3 Normal = normalize(output.Norm);
	float4 NL = saturate(dot(Normal, LightDir));

	float3 Reflect = normalize(2 * NL * Normal - LightDir);
	float4 specular = pow(saturate(dot(Reflect, ViewDir)), 4); 

	output.Color= g_Diffuse * NL + specular*g_Specular;

	return output;
}

/// <summary>
/// 透明かどうか
/// </summary>
bool IsOpacity(float color) {
	return (!color <= 0);
}

//
// float4 PSSkin(PSSkinIn input) : SV_Target
//ピクセルシェーダー
float4 PSSkin(PSSkinIn input) : SV_Target
{	
	//キャンパスカラー
	float4 campusColor = texInk.Sample(g_samLinear, input.Tex);
	float4 modelColor = g_texColor.Sample(g_samLinear, input.Tex);


	return campusColor;
	//新規インクが不透明か
	const bool IsOpacityModelColor = IsOpacity(modelColor.a);
	//キャンパスが不透明か
	const bool IsOpacityCampusColor = IsOpacity(campusColor.a);

	if (!IsOpacityModelColor && !IsOpacityCampusColor)
		discard;

	if (!IsOpacityModelColor && IsOpacityCampusColor)
		return  campusColor;

	if (IsOpacityModelColor && !IsOpacityCampusColor)
		return modelColor;

	float blend = 0.5f;
	return campusColor * (1 - blend * modelColor.a) + modelColor * modelColor.a * blend;

	return  input.Color/2+ campusColor /2;
}