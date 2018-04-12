//深度テクスチャ用のグローバル
cbuffer ZTexglobal:register(b0)
{
	matrix wlp; //ワールド・ライトビュー・プロジェクション行列
};

//バーテックスシェーダー出力構造体
struct VsOutput//深度テクスチャーレンダー用
{
	float4 pos : SV_POSITION;
	float4 depth :POSITION;
};


/// /// <summary>
/// 深度テクスチャ用バーテックスシェーダー
/// </summary>
VsOutput VS_Depth(float4 pos : POSITION)
{
	VsOutput output = (VsOutput)0;
	output.pos = mul(pos, wlp);
	output.depth = output.pos;
	return  output;
}


/// <summary>
/// 深度テクスチャ用ピクセルシェーダ
/// </summary>
float4 PS_Depth(VsOutput input) :SV_Target
{
	float a = input.depth.z / input.depth.w;
	//a = (a - 0.9) * 10;
	return float4(a,0,0,1);
}
