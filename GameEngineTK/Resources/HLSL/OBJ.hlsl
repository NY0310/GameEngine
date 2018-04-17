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
	float4 speed = float4(0,0,0,0);//速度
	float frame;//フレーム数
};

cbuffer global_1:register(b1)
{
	float4 ambient = float4(0, 0, 0, 1.0f);//アンビエント光
	float4 diffuse = float4(0.78f, 0.78f, 0.78f, 1); //拡散反射(色）
	float4 specular = float4(0.4f, 0.4f, 0.4f, 1.0f);//鏡面反射

};


#define		_PI				3.1415926535f
#define		GRAVITY			0.00f
#define		LIMITPLANE		40.0f
#define		MODELRADIUS		20.0f
#define		FACE_VELOCITY	0.5f
#define		MAT_DIFFUSECOLOR	float3( 1.0f, 1.0f, 0.0f )
#define		MAT_SPECULARCOLOR	float3( 1.0f, 1.0f, 1.0f )


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
	output.Pos = Pos;
	//法線をモデルの回転に合わせる
	output.Normal = Norm;
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


//	回転を計算する関数
void SetRotate(float3 rotang, out float3x3 mat)
{
	float2	roty;
	float2	rotz;
	sincos(rotang.y, roty.x, roty.y);
	sincos(rotang.z, rotz.x, rotz.y);
	mat._11 = rotz.y * roty.y;
	mat._12 = rotz.x * roty.y;
	mat._13 = -roty.x;
	mat._21 = -rotz.x;
	mat._22 = rotz.y;
	mat._23 = 0.0f;
	mat._31 = roty.x;
	mat._32 = 0.0f;
	mat._33 = roty.y;
}

bool IsSpeedZero(float4 speed)
{
	if (speed.x <= 0)
		return true;
	if (speed.y <= 0)
		return true;
	if (speed.z <= 0)
		return true;
	return false;
}


[maxvertexcount(3)]
void GS(triangle VS_OUTPUT In[3],
	inout TriangleStream<VS_OUTPUT> gsstream)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;

	static const float3	cV0 = {
		0.0f,	0.0f,	0.0f,
	};
	static const float3	cAG = {
		0.0f,	GRAVITY,	0.0f,
	};

	//ポリゴンの中央座標を取得
	float3	center = (In[0].Pos.xyz + In[1].Pos.xyz + In[2].Pos.xyz) * (1.0f / 3.0f);


	//正規化法線を求める
	float3	v0 = In[1].Pos.xyz - In[0].Pos.xyz;
	float3	v1 = In[2].Pos.xyz - In[0].Pos.xyz;
	float3	fnormal = normalize(cross(v0, v1));

	//ポリゴンの回転量
	float3	ang = float3(0, 0, 0);
	//重力
	float3	fallSpeed = float3(0, 0, 0);
	
	float3	faceSpeed = float3(0, 0, 0);

	//	時間をずらす
	//float	t = frame - (MODELRADIUS - center.y) * LIMITPLANE;
	//	全部同時に破壊
	float t = frame;

	//0以下は無効
	if (frame > 0) {
		//回転量を計算
		float t2 = t * t;
		fallSpeed = cV0 * t + cAG * t2;
		ang.z = frame * 0.13f;
		ang.y = frame * 0.013f * In[1].Pos.x;
		ang.x = 0.0f;
		if (IsSpeedZero(speed))
		{
			faceSpeed = FACE_VELOCITY * fnormal * t;
		}
		else
		{
			faceSpeed = FACE_VELOCITY * speed * t;
		}
	}

	float3x3 rotmat;
	//回転量から回転行列を
	SetRotate(ang, rotmat);

	VS_OUTPUT CIn[3];

	for (int j = 0; j < 3; j++) {
		CIn[j] = In[j];
	}

	for (int i = 0; i < 3; i++) {
		//中心からの距離
		CIn[i].Pos.xyz -= center;
		//距離に回転行列を適応させる
		float3	newpos = mul(CIn[i].Pos.xyz, rotmat);
		//移動後の座標を求める
		CIn[i].Pos.xyz = center + newpos.xyz + faceSpeed;
		//法線を回転させる
		CIn[i].Normal = mul(CIn[i].Normal, rotmat);
		//ワールド行列を求める
		float4	wpos = mul(CIn[i].Pos, mW);
		//移動量を加味する
		wpos.xyz += fallSpeed;
		Out.PosWorld = wpos;
		//ワールド・ビュー・プロジェクション行列
		Out.Pos = mul(CIn[i].Pos, mWVP);
		Out.Normal = mul(CIn[i].Normal, (float3x3)mW);
		//UV
		Out.Tex = CIn[i].Tex;

		gsstream.Append(Out);
	}

	gsstream.RestartStrip();
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