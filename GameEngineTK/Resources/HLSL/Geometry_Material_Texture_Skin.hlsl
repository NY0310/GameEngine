//定義
#define MAX_BONE_MATRICES 255

//グローバル
Texture2D texColor: register(t0);//テクスチャーは レジスターt(n)
SamplerState samLinear : register(s0);//サンプラーはレジスターs(n)

cbuffer global_0:register(b0)
{
	float4 vLight;  //ライトの方向ベクトル
	float4 vEye;//カメラ位置
};

cbuffer global_1:register(b1)
{
	matrix mW;//ワールド行列
	matrix mWVP; //ワールドから射影までの変換行列
	float4 Ambient=float4(0,0,0,0);//アンビエント光
	float4 Diffuse=float4(1,0,0,0); //拡散反射(色）
	float4 Specular=float4(1,1,1,1);//鏡面反射
};

cbuffer bones:register(b2)//ボーンのポーズ行列が入る
{
	matrix mConstBoneWorld[MAX_BONE_MATRICES];
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
//指定した番号のボーンのポーズ行列を返す
matrix FetchBoneMatrix( uint iBone )
{
	return mConstBoneWorld[iBone];
}



//頂点をスキニング（ボーンにより移動）する。

Skin SkinVert( VSSkinIn Input )
{
	Skin Output = (Skin)0;

	float4 Pos = float4(Input.Pos,1);
	float3 Norm = Input.Norm;
	//ボーンx
	uint iBone=Input.Bones.x;
	float fWeight=Input.Weights.x;
	matrix m=FetchBoneMatrix(iBone);
	Output.Pos+=fWeight * mul(Pos,m);
	Output.Norm+=fWeight * mul(Norm,(float3x3)m);
	//ボーンy
	iBone=Input.Bones.y;
	fWeight=Input.Weights.y;
	m=FetchBoneMatrix(iBone);
	Output.Pos+=fWeight*mul(Pos,m);
	Output.Norm+=fWeight*mul(Norm,(float3x3)m);
	//ボーンz
	iBone=Input.Bones.z;
	fWeight=Input.Weights.z;
	m=FetchBoneMatrix(iBone);
	Output.Pos+=fWeight*mul(Pos,m);
	Output.Norm+=fWeight*mul(Norm,(float3x3)m);
	//ボーンw
	iBone=Input.Bones.w;
	fWeight=Input.Weights.w;
	m=FetchBoneMatrix(iBone);
	Output.Pos+=fWeight*mul(Pos,m);
	Output.Norm+=fWeight*mul(Norm,(float3x3)m);

	return Output;
}

/// <summary>
/// バーテックスシェーダー
/// </summary>
PSSkinIn VSSkin(VSSkinIn input )
{
	PSSkinIn output;
	Skin vSkinned = SkinVert(input);

	output.Pos = mul( vSkinned.Pos, mWVP );
	output.Norm = normalize( mul( vSkinned.Norm, (float3x3)mW ) );
	output.Tex = input.Tex;
	float3 LightDir = normalize(vLight);
	float3 PosWorld = mul(vSkinned.Pos,mW);
	float3 ViewDir = normalize(vEye-PosWorld);
	float3 Normal = normalize(output.Norm);
	float4 NL = saturate(dot(Normal, LightDir));

	float3 Reflect = normalize(2 * NL * Normal - LightDir);
	float4 specular = pow(saturate(dot(Reflect, ViewDir)), 4); 

	output.Color= Diffuse * NL + specular*Specular;

	return output;
}

/// <summary>
/// ピクセルシェーダ
/// </summary>
float4 PSSkin(PSSkinIn input) : SV_Target
{	
	float4 TexDiffuse = texColor.Sample( samLinear, input.Tex ); 

	return  input.Color/2+TexDiffuse/2;
}