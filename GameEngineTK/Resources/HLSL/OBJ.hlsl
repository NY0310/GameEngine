Texture2D texColor: register(t0);//シェーディングするモデルテクスチャ
Texture2D texInk: register(t1);//インクテクスチャ
Texture2D texDepth: register(t2);//深度テクスチャ

//サンプラー
SamplerState samLinear : register(s0);


//グローバル
cbuffer global_0:register(b0)
{
	matrix mW;//ワールド行列
	matrix mWVP; //ワールド・ビュー・プロジェクション行列
	matrix mWLP; //ワールド・ライトビュー・プロジェクション行列
	matrix mWLPT; //ワールド・ライトビュー・プロジェクション・UV行列
	float4 vLightDir;  //ライトの視点
	float4 vEye;//カメラ位置
};

cbuffer global_1:register(b1)
{
	float4 ambient = float4(0, 0, 0, 1.0f);//アンビエント光
	float4 diffuse = float4(0.78f, 0.78f, 0.78f, 1); //拡散反射(色）
	float4 specular = float4(0.4f, 0.4f, 0.4f, 1.0f);//鏡面反射

};


//バーテックスシェーダー出力構造体
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float3 Light : TEXCOORD0;
	float3 Normal : TEXCOORD1;
	float3 EyeVector : TEXCOORD2;
	float4 PosWorld : TEXCOORD3;
	float2 Tex : TEXCOORD4;
	float4 LightTexCoord : TEXCOORD5;
	float4 LighViewPos : TEXCOORD56;
};






//
//バーテックスシェーダー
//
VS_OUTPUT VS(float4 Pos : POSITION, float4 Norm : NORMAL, float2 Tex : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	//射影変換
	output.Pos = mul(Pos, mWVP);
	//法線をモデルの回転に合わせる
	output.Normal = (float3)mul((float3)Norm, (float3x3)mW);
	//ライト方向
	output.Light = normalize((float3)vLightDir);
	//ワールド座標
	output.PosWorld = mul(Pos, mW);
	//視線ベクトル	
	output.EyeVector = normalize((float3)(vEye - output.PosWorld));
	//UV値
	output.Tex = Tex;
	//ライトビューを参照するとき、手がかりとなるテクスチャ座標
	output.LightTexCoord = mul(Pos, mWLPT);
	//ライトビューの座標
	output.LighViewPos = mul(Pos, mWLP);

	return output;
}


//
//バーテックスシェーダー
//
VS_OUTPUT VS_NoTex(float4 Pos : POSITION, float4 Norm : NORMAL)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	//射影変換
	output.Pos = mul(Pos, mWVP);
	//法線をモデルの回転に合わせる
	output.Normal = (float3)mul((float3)Norm, (float3x3)mW);
	//ライト方向
	output.Light = (float3)normalize(vLightDir);
	//ワールド座標
	output.PosWorld = mul(Pos, mW);
	//視線ベクトル	
	output.EyeVector = (float3)normalize(vEye - output.PosWorld);
	//ライトビューを参照するとき、手がかりとなるテクスチャ座標
	output.LightTexCoord = mul(Pos, mWLPT);
	//ライトビューの座標
	output.LighViewPos = mul(Pos, mWLP);


	return output;
}


/// <summary>
/// 透明かどうか
/// </summary>
bool IsOpacity(float color) {
	return (!color <= 0);
}

//
//ピクセルシェーダー
//
float4 PS(VS_OUTPUT input) : SV_Target
{

	//キャンパスの色
	float4 campusColor = texInk.Sample(samLinear, input.Tex);
	//モデルの色
	float4 modelColor = texColor.Sample(samLinear, input.Tex);



	///// <summary>
	///// シャドウマップ
	///// </summary>
	//input.LightTexCoord /= input.LightTexCoord.w;
	//float TexValue = texDepth.Sample(samLinear,input.LightTexCoord.xy).r;
	//float LightLength = input.LighViewPos.z / input.LighViewPos.w;
	////ライトビューでの長さが短いので障害物あり
	//if (TexValue + 0.001<LightLength)
	//{
	//	//モデルとキャンパスの色に光沢と影を適応させる
	//	modelColor.rgb /= 3;
	//	campusColor.rgb /= 3;
	//}


	//新規インクが不透明か
	const bool IsOpacityModelColor = IsOpacity(modelColor.a);
	//キャンパスが不透明か
	const bool IsOpacityCampusColor = IsOpacity(campusColor.a);



	if (!IsOpacityModelColor && !IsOpacityCampusColor)
		discard;

	if (IsOpacityModelColor && !IsOpacityCampusColor)
	{
		/// <summary>
		/// フォンシェーディング
		/// </summary>
		////環境光　
		//float NL = saturate(dot(input.Normal, input.Light));
		////拡散反射光　
		//float4 diffuse_ = (diffuse * 0.1 + modelColor * 0.9)*NL;
		////鏡面反射光　
		//float3 reflect = normalize(2 * NL*input.Normal - input.Light);
		//float4 specular_ = pow(saturate(dot(reflect, input.EyeVector)), 2)*specular;
		////フォンモデル最終色
		//return  ambient + diffuse_ + specular_;
		return modelColor;
	}


	return campusColor;

}

//
//ピクセルシェーダー
//
float4 PS_NoTex(VS_OUTPUT input) : SV_Target
{
	//float4 color = input.Color;

	return float4(1,1,1,1);
}