/// <summary>
///	オブジェクトに貼るインクをテクスチャを作成
/// </summary>

//グローバル
Texture2D texNewInk: register(t0);//インクテクスチャ
Texture2D texCampus: register(t1);//インクテクスチャ
texture2D normalMap: register(t2);//ノーマルマップ

SamplerState samLinear : register(s0);


cbuffer global_0:register(b0)
{
	float4 inkColor;//インクの基本色
	//float inkScale;//インクの大きさ
};


struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
};





/// <summary>
/// インクを塗るか
/// </summary>
bool IsPaint(float2 inkuv, float2 inputuv, float scale)
{
	return (inkuv.x - scale < inputuv.x && inputuv.x < inkuv.x + scale &&
		inkuv.y - scale < inputuv.y &&
		inputuv.y < inkuv.y + scale);

}




/// <summary>
/// 透明かどうか
/// </summary>
bool IsOpacity(float color) {
	return (!color <= 0);
}


/// <summary>
/// 二色をブレンドする
/// </summary>
float4 Blend(float4 color, float4 color1, float brushAlpha) {
	return color * (1 - brushAlpha * color.a) + color1 * color1.a * brushAlpha;
}


/// <summary>
/// 正規デバイス座標系からUv値へ変換する
/// </summary>
float2 RegularDeviceChangeUv(float2 uv)
{
	return float2((uv.x + 1) / 2, (uv.y + 1) / 2);
}


/// <summary>
/// バーテックスシェーダー
/// </summary>
VS_OUTPUT VS(float4 Pos : POSITION ,float2 Tex : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Tex = Tex;
	output.Pos =  Pos;	
	return output;
}



/// <summary>
/// ピクセルシェーダー
/// </summary>
float4 PS(VS_OUTPUT input) : SV_Target
{

	//新規インクのカラー
	float4 newColor = texNewInk.Sample(samLinear,input.Tex);
	//キャンパスカラー
	float4 campusColor = texCampus.Sample(samLinear, clamp(RegularDeviceChangeUv(input.Pos.xy),0,1));
	//新規インクが不透明か
	const bool IsOpacitytNewColor = IsOpacity(newColor.a);
	//キャンパスが不透明か
	const bool IsOpacityCampusColor = IsOpacity(campusColor.a);


	//新規インクのカラーキャンパスカラー共に透明の場合出力なし
	if (!IsOpacitytNewColor&& !IsOpacityCampusColor)discard;	

	

	//新規インクが不透明か
	if (IsOpacitytNewColor)
	{
		//ノーマルマップから高さ情報を読み取る
		float4 hight = normalMap.Sample(samLinear, input.Tex);
		newColor.a = 0.299 * hight.x + 0.587 * hight.y + 0.114 * hight.z;
		//インクの色を格納
		newColor.xyz = inkColor.xyz;
	}

	//新規インクのカラーキャンパスカラー共に不透明の場合色をブレンドし高さを足し出力
	if (IsOpacitytNewColor && IsOpacityCampusColor)
		//return float4((newColor.xyz + campusColor.xyz) / 2, newColor.a + campusColor.a);
		return newColor;

	//新規インクのカラーのみ不透明の場合新規インクの情報のみ出力
	if (IsOpacitytNewColor && !IsOpacityCampusColor)
		return newColor;

	//キャンパスカラーのみ出力
	return campusColor;

}


