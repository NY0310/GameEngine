texture2D texCampus : register(t0);
SamplerState samLinear : register(s0);



struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
};


/// <summary>
/// 透明かどうか
/// </summary>
bool IsOpacity(float color) {
	return (!color <= 0);
}


/// <summary>
/// バーテックスシェーダー
/// </summary>
VS_OUTPUT VS(float4 Pos : POSITION, float2 Tex : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Tex = Tex;
	output.Pos = Pos;
	return output;
}



float4 PS(VS_OUTPUT input) : SV_TARGET
{
	float4 campusColor = texCampus.Sample(samLinear, input.Tex);


	float2 _FlowDirection =float2(0, -1);//垂れる方向
	float2 _MainTex_TexelSize = float2(0.05, 0.05);
	float _ScaleFactor =1;
	float _Viscosity = 0.08f;//粘り気



	const float VITIATE_X = 0.3;//どのくらい横の液体を考慮するかの係数。
	float2 shiftZ = float2(_FlowDirection.x * _MainTex_TexelSize.x, _FlowDirection.y * _MainTex_TexelSize.y) * _ScaleFactor * _Viscosity;
	float2 shiftX = float2(_FlowDirection.y * _MainTex_TexelSize.x, _FlowDirection.x * _MainTex_TexelSize.y) * _ScaleFactor * _Viscosity *VITIATE_X;
	float2 shiftx = -shiftX;

	//上、左上、右上のインクの高さを見る
	float2  ShiftTex = clamp(input.Tex + shiftZ, 0, 1);
	float4 texZ = texCampus.Sample(samLinear, ShiftTex);
	ShiftTex = clamp(input.Tex + shiftx + shiftZ, 0, 1);
	float4 texx = texCampus.Sample(samLinear,ShiftTex);
	ShiftTex = clamp(input.Tex + shiftX + shiftZ,0,1) ;
	float4 texX = texCampus.Sample(samLinear,ShiftTex);

	//ピクセルの液体付着量
	float amountUp = texZ.a * 0.5 + texx.a * 0.25 + texX.a * 0.25;

	//ピクセルにインクが塗られなかった
	if (amountUp <= 0) {
		discard;
	}


	//垂れてきた液体を加算した合計の液体付着量
	float resultAmount = (campusColor.a  + amountUp) * 0.5;
	//垂れた液体の色を計算
	//上、左上、右上の色で一番明るい色
	float3 maxRGB = max(campusColor.rgb, max(texZ.rgb, max(texx.rgb, texX.rgb)));
	//float3 maxRGB = max(texZ.rgb, b, texX.rgb));

	float time = clamp(amountUp - _Viscosity, 0, 1);
	//垂れてくる液体との補間
	float3 resultRGB = lerp(maxRGB, texZ.rgb, time);

	campusColor = float4(resultRGB, resultAmount);

	return campusColor;
}